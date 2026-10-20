#include "MenuGuarderia.h"
#include <iostream>
#include <ctime>       // fecha actual
#include "Expediente_Lactante.h"
#include "Expediente_Preescolar.h"
#include "Ficha_Medica.h"
#include "Ingreso_Guarderia.h"
#include "Validaciones.h"


//  1) DETALLES INTERNOS
//  Un namespace SIN nombre hace que todo lo de adentro sea visible solo en
//  este archivo (como si fuera "privado" del .cpp).
// ---------------------------------------------------------------------
namespace {

// Cuantos infantes caben en la guarderia. Cambialo aqui si quieres otro limite.
// (Se muestra siempre en el menú y al inscribir, para que nunca sea un misterio.)
const int CAPACIDAD_MAXIMA = 30;

// ---- Entrada de datos ------------------------------------------------
// Todo se lee con std::getline y luego se convierte con stoi/stof.
// ¿Por qué? Mezclar "cin >> numero" con getline deja un salto de linea
// pendiente en el teclado que hace que se salten preguntas.

// Muestra un mensaje y lee una linea completa de texto.
// recortar() quita los espacios sobrantes del inicio y del final.
std::string leerTexto(const std::string& mensaje) {
    std::cout << mensaje;
    std::string texto;
    std::getline(std::cin, texto);
    return recortar(texto);
}

// Repite la pregunta hasta que el usuario escriba un entero valido.
int leerEntero(const std::string& mensaje) {
    while (true) {
        std::string texto = leerTexto(mensaje);
        if (!std::cin) {
            return 0;                     // la entrada se cerro: salimos del ciclo
        }
        try {
            return std::stoi(texto);      // lanza excepcion si no es un número
        } catch (const std::exception&) {
            std::cout << "Valor invalido. Intenta de nuevo.\n";
        }
    }
}

// Igual, pero para numeros con decimales.
float leerDecimal(const std::string& mensaje) {
    while (true) {
        std::string texto = leerTexto(mensaje);
        if (!std::cin) {
            return 0;
        }
        try {
            return std::stof(texto);
        } catch (const std::exception&) {
            std::cout << "Valor invalido. Intenta de nuevo.\n";
        }
    }
}

// Pregunta de si/no: devuelve true si la respuesta empieza con 's' o 'S'.
bool leerSiNo(const std::string& mensaje) {
    std::string texto = leerTexto(mensaje);
    return !texto.empty() && (texto[0] == 's' || texto[0] == 'S');
}

// ---- Lecturas CON VALIDACIÓN -------------------------------------------
// Las tres siguen la misma idea: repiten la pregunta hasta que el dato tenga
// el formato correcto. Si el usuario deja la línea VACÍA devuelven "" y quien
// las llamó interpreta eso como "cancelar" (así nadie queda atrapado en un
// ciclo). También devuelven "" si la entrada se cerró.

// Id de infante: I-XXXX.  aMayusculas() hace que "i-0001" también se acepte.
std::string leerIdInfante(const std::string& mensaje) {
    while (true) {
        std::string id = aMayusculas(leerTexto(mensaje));
        if (id.empty() || esIdInfanteValido(id)) {
            return id;
        }
        std::cout << "  Formato invalido. Debe ser " << formatoId('I')
                  << " (X = digito, ej. I-0001). Deja vacio para cancelar.\n";
    }
}

// Id de tutor: T-XXXX.
std::string leerIdTutor(const std::string& mensaje) {
    while (true) {
        std::string id = aMayusculas(leerTexto(mensaje));
        if (id.empty() || esIdTutorValido(id)) {
            return id;
        }
        std::cout << "  Formato invalido. Debe ser " << formatoId('T')
                  << " (X = digito, ej. T-0001). Deja vacio para cancelar.\n";
    }
}

// Teléfono: exactamente DIGITOS_TELEFONO dígitos, solo números.
std::string leerTelefono(const std::string& mensaje) {
    while (true) {
        std::string telefono = leerTexto(mensaje);
        if (telefono.empty() || esTelefonoValido(telefono)) {
            return telefono;
        }
        std::cout << "  Telefono invalido. Deben ser exactamente " << DIGITOS_TELEFONO
                  << " digitos, solo numeros. Deja vacio para cancelar.\n";
    }
}

// Gravedad de una irregularidad. Devuelve Ninguna si el usuario escribe 0 (= cancelar)
// o si la entrada se cerró; así este ciclo nunca queda atrapado.
SeveridadIrregularidad leerSeveridad(const std::string& mensaje) {
    while (true) {
        int opcion = leerEntero(mensaje);
        if (opcion == 1) {
            return SeveridadIrregularidad::Advertencia;
        }
        if (opcion == 2) {
            return SeveridadIrregularidad::Grave;
        }
        if (opcion == 0) {
            return SeveridadIrregularidad::Ninguna;
        }
        std::cout << "  Opcion invalida. Escribe 1, 2 o 0.\n";
    }
}

// Captura los datos de un tutor pregunta por pregunta, validando cada uno.
// Devuelve false si el usuario cancela (deja un dato vacío); si no, deja el
// tutor en 'destino' (se recibe por REFERENCIA para poder llenarlo).
bool capturarTutor(Tutor& destino) {
    std::string idTutor = leerIdTutor("  Id del tutor (formato " + formatoId('T') + "): ");
    if (idTutor.empty()) {
        return false;
    }
    std::string nombre = leerTexto("  Nombre: ");
    std::string telefono = leerTelefono("  Telefono (" + std::to_string(DIGITOS_TELEFONO) + " digitos): ");
    if (nombre.empty() || telefono.empty()) {
        return false;
    }
    std::string parentesco = leerTexto("  Parentesco: ");
    destino = Tutor(idTutor, nombre, telefono, parentesco);
    return true;
}

// Fecha de hoy con formato "AAAA-MM-DD" (para los registros).
std::string fechaHoy() {
    std::time_t ahora = std::time(nullptr);          // segundos desde 1970
    std::tm* local = std::localtime(&ahora);         // los convierte a fecha local
    char buffer[11];                                 // "AAAA-MM-DD" + terminador = 11
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", local);
    return buffer;
}

// Muestra un infante y el resumen de su expediente.
// generarResumen() es VIRTUAL: imprime los datos de Lactante o de Preescolar
// según el tipo REAL del expediente (polimorfismo), sin ningún 'if'.
void mostrarInfante(const Infante& inf) {
    std::cout << inf.getIdInfante() << " | " << inf.getNombreCompleto()
              << " | Estatus: " << inf.getEstatusActual() << "\n";
    std::cout << "   " << inf.getExpediente().generarResumen() << "\n";
}

// Muestra los tutores de un infante.
void mostrarTutoresDe(const Infante& inf) {
    std::cout << "Tutores de " << inf.getNombreCompleto() << ":\n";
    for (const Tutor& t : inf.getTutores()) {
        std::cout << "  - " << t.getIdTutor() << " " << t.getNombre() << " (" << t.getParentesco()
                  << ") Tel: " << t.getTelefono() << "\n";
    }
}

}   // fin del namespace sin nombre

// ---------------------------------------------------------------------
//  2) MÉTODOS DE MenuGuarderia
//  Cada uno lleva el prefijo 'MenuGuarderia::' (pertenece a la clase).
//  Dentro de ellos, 'guarderia', 'historial' y 'contadorRegistros' son los
//  atributos del propio objeto: ya no se pasan como parámetros.
// ---------------------------------------------------------------------

// Constructor: crea la guardería con su capacidad y pone el contador en 0.
// (El vector 'historial' empieza vacío por sí solo.)
MenuGuarderia::MenuGuarderia()
    : guarderia("Rayito de Sol", CAPACIDAD_MAXIMA),
      contadorRegistros(0)
{
}

// El menú muestra SIEMPRE cuántos infantes hay y cuántos caben.
void MenuGuarderia::mostrarMenu() const {
    std::cout << "===== GUARDERIA " << guarderia.getNombreGuarderia() << " =====\n"
              << "Infantes: " << guarderia.contarInfantes() << " de " << guarderia.getCapacidadMaxima()
              << " (cupos libres: " << guarderia.getCapacidadMaxima() - guarderia.contarInfantes() << ")\n"
              << "1. Inscribir infante\n"
              << "2. Listar todos los infantes\n"
              << "3. Listar por categoria (lactantes / preescolares)\n"
              << "4. Buscar infante por id\n"
              << "5. Proceso de ingreso (ficha medica + ingreso)\n"
              << "6. Ver historial de registros\n"
              << "7. Eliminar infante\n"
              << "8. Agregar 3 infantes de ejemplo (solo para probar)\n"
              << "9. Administrar tutores (listar / agregar / reemplazar / eliminar)\n"
              << "0. Salir\n";
}

// OPCIÓN 1: pide los datos, arma las piezas (Tutor, Expediente, Infante) y las inscribe.
void MenuGuarderia::opcionInscribir() {
    if (!guarderia.hayCupo()) {
        std::cout << "La guarderia esta llena (" << guarderia.contarInfantes() << " de "
                  << guarderia.getCapacidadMaxima() << "). Elimina un infante (opcion 7) para liberar un cupo.\n";
        return;
    }
    std::cout << "Cupos libres: " << guarderia.getCapacidadMaxima() - guarderia.contarInfantes()
              << " de " << guarderia.getCapacidadMaxima() << ". (Deja un dato vacio para cancelar.)\n";

    std::string id = leerIdInfante("Id del infante (formato " + formatoId('I') + "): ");
    if (id.empty()) {
        std::cout << "Inscripcion cancelada.\n";
        return;
    }
    if (guarderia.buscarInfante(id) != nullptr) {    // nullptr = no existe; si NO es nullptr, ya existe
        std::cout << "Ya existe un infante con ese id.\n";
        return;
    }
    std::string nombre = leerTexto("Nombre completo: ");
    if (nombre.empty()) {
        std::cout << "Inscripcion cancelada.\n";
        return;
    }

    // El tutor se puede capturar de DOS formas:
    //   a) en UNA línea con el formato id|nombre|telefono|parentesco, usando el
    //      [OPERADOR >>] sobrecargado de Tutor sobre cin;
    //   b) pregunta por pregunta (cada dato se valida al momento).
    Tutor tutor("", "", "", "");                     // tutor "en blanco" que se llenará
    if (leerSiNo("Capturar el tutor en UNA linea (id|nombre|telefono|parentesco)? (s/n): ")) {
        std::cout << "Tutor: ";
        // 'cin >> tutor' llama a operator>>(istream&, Tutor&). Devuelve el flujo, y un
        // flujo se puede usar como condición: es false si la lectura FALLÓ (failbit).
        if (!(std::cin >> tutor)) {
            std::cin.clear();                        // limpia el estado de error del flujo
            std::cout << "Formato incorrecto. No se inscribio.\n";
            return;
        }
        // operator>> solo comprueba que haya 4 campos. Las reglas de formato las
        // comprobamos aquí, igual que en la captura pregunta por pregunta.
        tutor = Tutor(aMayusculas(tutor.getIdTutor()), tutor.getNombre(),
                      tutor.getTelefono(), tutor.getParentesco());
        if (!esIdTutorValido(tutor.getIdTutor())) {
            std::cout << "Id de tutor invalido (debe ser " << formatoId('T')
                      << "). No se inscribio.\n";
            return;
        }
        if (!esTelefonoValido(tutor.getTelefono())) {
            std::cout << "Telefono invalido (deben ser exactamente " << DIGITOS_TELEFONO
                      << " digitos, solo numeros). No se inscribio.\n";
            return;
        }
    } else {
        // IMPORTANTE: cada dato se lee en su PROPIA variable. Si escribiéramos
        // Tutor(leerTexto(..), leerTexto(..), ...), C++ NO garantiza el orden en que
        // se evalúan los argumentos de una función: las preguntas podrían salir
        // desordenadas.
        std::string idTutor = leerIdTutor("  Id del tutor (formato " + formatoId('T') + "): ");
        if (idTutor.empty()) {
            std::cout << "Inscripcion cancelada.\n";
            return;
        }
        std::string nombreTutor = leerTexto("  Nombre: ");
        std::string telefono = leerTelefono("  Telefono (" + std::to_string(DIGITOS_TELEFONO) + " digitos): ");
        if (nombreTutor.empty() || telefono.empty()) {
            std::cout << "Inscripcion cancelada.\n";
            return;
        }
        std::string parentesco = leerTexto("  Parentesco: ");
        tutor = Tutor(idTutor, nombreTutor, telefono, parentesco);   // reemplaza el tutor en blanco
    }

    int tipo = leerEntero("Tipo de expediente (1 = Lactante, 2 = Preescolar): ");
    std::string idExpediente = "EXP-" + id;

    // Puntero al PADRE: apuntará a una hija u otra según lo que elija el usuario.
    std::shared_ptr<Expediente_Medico> expediente;

    if (tipo == 1) {
        int meses = leerEntero("Edad en meses: ");
        std::string alimentacion = leerTexto("Alimentacion (materna/formula/mixta): ");
        float peso = leerDecimal("Peso en kg: ");
        expediente = std::make_shared<Expediente_Lactante>(idExpediente, meses, alimentacion, peso);
    } else if (tipo == 2) {
        int anios = leerEntero("Edad en anios: ");
        bool esfinteres = leerSiNo("Controla esfinteres? (s/n): ");
        std::string sala = leerTexto("Sala asignada: ");
        expediente = std::make_shared<Expediente_Preescolar>(idExpediente, anios, esfinteres, sala);
    } else {
        std::cout << "Tipo invalido. No se inscribio.\n";
        return;
    }

    // Alergias: el usuario decide si agrega y si indica severidad.
    while (leerSiNo("Agregar una alergia? (s/n): ")) {
        std::string alergia = leerTexto("  Alergia: ");
        if (leerSiNo("  Indicar severidad? (s/n): ")) {
            std::string severidad = leerTexto("  Severidad: ");
            expediente->agregarAlergia(alergia, severidad);   // [SOBRECARGA] versión de 2 parámetros
        } else {
            expediente->agregarAlergia(alergia);              // [SOBRECARGA] versión de 1 parámetro
        }
    }

    // make_shared crea el Infante y su puntero inteligente de una vez.
    std::shared_ptr<Infante> infante = std::make_shared<Infante>(id, nombre, tutor, expediente);
    if (guarderia.inscribirInfante(infante)) {
        std::cout << "Infante inscrito (" << expediente->getCategoria() << "). Cupos libres: "
                  << guarderia.getCapacidadMaxima() - guarderia.contarInfantes() << ".\n";
    } else {
        std::cout << "No se pudo inscribir.\n";
    }
}

// OPCIÓN 2: lista todos los infantes.
void MenuGuarderia::opcionListar() const {
    if (guarderia.contarInfantes() == 0) {           // [SOBRECARGA] sin parámetros: total
        std::cout << "No hay infantes inscritos.\n";
        return;
    }
    for (const std::shared_ptr<Infante>& inf : guarderia.getInfantes()) {
        mostrarInfante(*inf);                        // '*inf': del puntero al objeto
    }
}

// OPCIÓN 3: SEPARA a los infantes por categoría.
void MenuGuarderia::opcionPorCategoria() const {
    const std::vector<std::string> categorias = {"Lactante", "Preescolar"};
    for (const std::string& categoria : categorias) {
        std::cout << "--- Categoria: " << categoria
                  << " (" << guarderia.contarInfantes(categoria) << ") ---\n";   // [SOBRECARGA] con parámetro
        for (const std::shared_ptr<Infante>& inf : guarderia.obtenerPorCategoria(categoria)) {
            std::cout << "  - " << inf->getIdInfante() << " " << inf->getNombreCompleto()
                      << " (" << inf->getEstatusActual() << ")\n";
        }
    }
    std::cout << "Total: " << guarderia.contarInfantes() << " de " << guarderia.getCapacidadMaxima() << "\n";
}

// OPCIÓN 4: busca un infante y muestra todo sobre él.
void MenuGuarderia::opcionBuscar() const {
    std::string id = aMayusculas(leerTexto("Id a buscar: "));
    std::shared_ptr<Infante> inf = guarderia.buscarInfante(id);
    if (!inf) {                                      // puntero vacío = no se encontró
        std::cout << "No se encontro ese infante.\n";
        return;
    }
    mostrarInfante(*inf);
    std::cout << "   Tutores:\n";
    for (const Tutor& t : inf->getTutores()) {
        std::cout << "   - " << t.getNombre() << " (" << t.getParentesco()
                  << ") Tel: " << t.getTelefono() << "\n";
    }
    // [OPERADOR <<] Los operadores sobrecargados escriben el infante completo en cout.
    // El del expediente llama a escribir() (VIRTUAL): sale el formato de la hija correcta.
    std::cout << "   Formato de texto (operador <<):\n" << *inf;
}

// OPCIÓN 5: el PROCESO DE INGRESO. Usa herencia, polimorfismo y la dependencia
// entre Ingreso_Guarderia y Ficha_Medica.
void MenuGuarderia::opcionIngreso() {
    std::string id = aMayusculas(leerTexto("Id del infante: "));
    std::shared_ptr<Infante> inf = guarderia.buscarInfante(id);
    if (!inf) {
        std::cout << "No se encontro ese infante.\n";
        return;
    }
    bool golpes = leerSiNo("Presenta golpes? (s/n): ");
    float temperatura = leerDecimal("Temperatura en C: ");

    // IRREGULARIDADES: se pregunta si presenta alguna y, si es así, cuál y qué tan seria.
    // Se captura ANTES de crear los registros: si el usuario cancela, no se crea nada
    // (así nunca se aprueba por error a un infante con una irregularidad sin registrar).
    SeveridadIrregularidad severidad = SeveridadIrregularidad::Ninguna;
    std::string descripcion;
    if (leerSiNo("Presenta alguna irregularidad? (s/n): ")) {
        descripcion = leerTexto("  Cual es la irregularidad?: ");
        if (descripcion.empty()) {
            std::cout << "Proceso cancelado.\n";
            return;
        }
        severidad = leerSeveridad("  Gravedad (1 = Advertencia: pasa con advertencia, 2 = Grave: no pasa, 0 = cancelar): ");
        if (severidad == SeveridadIrregularidad::Ninguna) {
            std::cout << "Proceso cancelado.\n";
            return;
        }
    }

    ++contadorRegistros;                             // número consecutivo para los ids de los registros
    std::string fecha = fechaHoy();
    // Ambos registros apuntan al MISMO infante de la guardería.
    std::shared_ptr<Ficha_Medica> ficha = std::make_shared<Ficha_Medica>(
        "F-" + std::to_string(contadorRegistros), fecha, inf, golpes, temperatura);
    std::shared_ptr<Ingreso_Guarderia> ingreso = std::make_shared<Ingreso_Guarderia>(
        "G-" + std::to_string(contadorRegistros), fecha, inf);

    if (severidad != SeveridadIrregularidad::Ninguna) {
        ficha->registrarIrregularidad(descripcion, severidad);
    }

    // [POLIMORFISMO] Un vector de punteros al PADRE (Registro) que contiene
    // objetos de las HIJAS. Un shared_ptr<Hija> se convierte solo en shared_ptr<Padre>.
    std::vector<std::shared_ptr<Registro>> proceso;
    proceso.push_back(ficha);
    proceso.push_back(ingreso);

    // Misma llamada, comportamiento distinto: la ficha evalúa la salud y el
    // ingreso abre el trámite. C++ elige la versión según el objeto REAL.
    for (const std::shared_ptr<Registro>& r : proceso) {
        r->iniciarProceso();
    }

    // Dependencia: el ingreso usa la ficha como parámetro para decidir.
    // '*ficha' convierte el puntero en el objeto Ficha_Medica.
    bool aprobado = ingreso->validarAcceso(*ficha);
    std::cout << (aprobado ? "INGRESO APROBADO" : "INGRESO RECHAZADO");
    if (aprobado && !ingreso->getAdvertencia().empty()) {
        std::cout << " CON ADVERTENCIA - " << ingreso->getAdvertencia();
    }
    if (!aprobado) {
        std::cout << " - " << ingreso->getMotivoRechazo();
    }
    std::cout << "\n";

    // generarReporte() también es virtual: cada hija agrega lo suyo.
    for (const std::shared_ptr<Registro>& r : proceso) {
        historial.push_back(r);                      // se conserva para la opción 6
        std::cout << r->generarReporte() << "\n";
    }
}

// OPCIÓN 6: imprime el historial de todos los registros del programa.
void MenuGuarderia::opcionHistorial() const {
    if (historial.empty()) {
        std::cout << "Aun no hay registros.\n";
        return;
    }
    for (const std::shared_ptr<Registro>& r : historial) {
        std::cout << r->generarReporte() << "\n";    // polimorfismo otra vez
    }
}

// OPCIÓN 7: elimina un infante (pidiendo confirmación, porque no se puede deshacer).
// Nota: los registros que ya estén en el historial se CONSERVAN (el historial
// guarda su propio puntero compartido al infante), igual que en una guardería
// real el historial no se borra cuando un niño se da de baja.
void MenuGuarderia::opcionEliminar() {
    if (guarderia.contarInfantes() == 0) {
        std::cout << "No hay infantes inscritos.\n";
        return;
    }
    std::string id = aMayusculas(leerTexto("Id del infante a eliminar (formato " + formatoId('I') + "): "));
    std::shared_ptr<Infante> inf = guarderia.buscarInfante(id);
    if (!inf) {
        std::cout << "No se encontro ese infante.\n";
        return;
    }
    mostrarInfante(*inf);                            // se muestra a quién se va a eliminar
    if (!leerSiNo("Seguro que quieres eliminarlo? No se puede deshacer (s/n): ")) {
        std::cout << "No se elimino nada.\n";
        return;
    }
    if (guarderia.eliminarInfante(id)) {             // devuelve true si lo encontró y lo borró
        std::cout << "Infante eliminado. Cupos libres: "
                  << guarderia.getCapacidadMaxima() - guarderia.contarInfantes() << ".\n";
    } else {
        std::cout << "No se pudo eliminar.\n";
    }
}

// Ayuda de la opción 8: inscribe un infante de ejemplo y avisa qué pasó.
void MenuGuarderia::inscribirEjemplo(const std::shared_ptr<Infante>& infante) {
    if (guarderia.inscribirInfante(infante)) {
        std::cout << "  + " << infante->getIdInfante() << " " << infante->getNombreCompleto()
                  << " (" << infante->getExpediente().getCategoria() << ")\n";
    } else {
        std::cout << "  - " << infante->getIdInfante() << " no se agrego (ya existe o no hay cupo)\n";
    }
}

// OPCIÓN 8: como los datos no se guardan en disco, esta opción crea tres infantes
// de ejemplo para poder probar el resto del menú sin teclear todo.
void MenuGuarderia::opcionDemo() {
    std::cout << "Agregando infantes de ejemplo:\n";

    // Lactante con una alergia CON severidad ([SOBRECARGA] de 2 parámetros).
    std::shared_ptr<Expediente_Medico> exp1 =
        std::make_shared<Expediente_Lactante>("EXP-I-0001", 8, "materna", 7.5f);
    exp1->agregarAlergia("Polen", "alta");
    std::shared_ptr<Infante> sofia = std::make_shared<Infante>(
        "I-0001", "Sofia Perez", Tutor("T-0001", "Luis Perez", "3312345678", "Padre"), exp1);
    sofia->agregarTutor(Tutor("T-0004", "Marta Lopez", "3344455566", "Madre"));   // segundo tutor
    inscribirEjemplo(sofia);

    // Preescolar con una discapacidad.
    std::shared_ptr<Expediente_Medico> exp2 =
        std::make_shared<Expediente_Preescolar>("EXP-I-0002", 4, true, "Sala A");
    exp2->agregarDiscapacidad("Hipoacusia leve");
    std::shared_ptr<Infante> mateo = std::make_shared<Infante>(
        "I-0002", "Mateo Gomez", Tutor("T-0002", "Ana Gomez", "3398765432", "Madre"), exp2);
    // T-0003 también es tutor de Lucia (abajo): un mismo tutor puede serlo de varios infantes.
    mateo->agregarTutor(Tutor("T-0003", "Carlos Ruiz", "3311112222", "Tio"));
    inscribirEjemplo(mateo);

    // Lactante con una alergia SIN severidad ([SOBRECARGA] de 1 parámetro).
    std::shared_ptr<Expediente_Medico> exp3 =
        std::make_shared<Expediente_Lactante>("EXP-I-0003", 5, "formula", 6.2f);
    exp3->agregarAlergia("Lactosa");
    inscribirEjemplo(std::make_shared<Infante>(
        "I-0003", "Lucia Ruiz", Tutor("T-0003", "Carlos Ruiz", "3311112222", "Tio"), exp3));
}

// ---------------------------------------------------------------------
//  OPCIÓN 9: ADMINISTRAR TUTORES (submenú)
//  Las operaciones se hacen SOBRE UN INFANTE, porque la regla "cada infante
//  necesita al menos un tutor" (1..*) es de cada infante.
// ---------------------------------------------------------------------

// 9 -> 1: lista cada tutor UNA vez y debajo los infantes de los que es tutor.
void MenuGuarderia::opcionListarTutores() const {
    std::vector<Tutor> tutores = guarderia.obtenerTutores();
    if (tutores.empty()) {
        std::cout << "No hay tutores registrados.\n";
        return;
    }
    for (const Tutor& t : tutores) {
        std::cout << t.getIdTutor() << " | " << t.getNombre() << " | Tel: " << t.getTelefono() << "\n";
        std::cout << "   Tutor de:\n";
        for (const std::shared_ptr<Infante>& inf : guarderia.obtenerInfantesDeTutor(t.getIdTutor())) {
            // El parentesco se guarda por infante (el mismo tutor puede ser "Padre" de
            // uno y "Tio" de otro), así que se lee del tutor de ESE infante.
            const Tutor* relacion = inf->buscarTutor(t.getIdTutor());
            std::cout << "   - " << inf->getIdInfante() << " " << inf->getNombreCompleto()
                      << " (" << relacion->getParentesco() << ")\n";
        }
    }
    std::cout << "Total de tutores: " << tutores.size() << "\n";
}

// 9 -> 2: agrega un tutor ADICIONAL a un infante.
void MenuGuarderia::opcionAgregarTutor() {
    std::string id = aMayusculas(leerTexto("Id del infante (formato " + formatoId('I') + "): "));
    std::shared_ptr<Infante> inf = guarderia.buscarInfante(id);
    if (!inf) {
        std::cout << "No se encontro ese infante.\n";
        return;
    }
    mostrarTutoresDe(*inf);
    std::cout << "Datos del tutor NUEVO (deja un dato vacio para cancelar):\n";
    Tutor nuevo("", "", "", "");
    if (!capturarTutor(nuevo)) {
        std::cout << "Operacion cancelada.\n";
        return;
    }
    if (inf->agregarTutor(nuevo)) {
        std::cout << "Tutor agregado. " << inf->getNombreCompleto() << " ahora tiene "
                  << inf->getTutores().size() << " tutor(es).\n";
    } else {
        std::cout << "No se pudo agregar: ese id ya es tutor de este infante.\n";
    }
}

// 9 -> 3: reemplaza un tutor de un infante por otro (queda en el mismo lugar).
void MenuGuarderia::opcionReemplazarTutor() {
    std::string id = aMayusculas(leerTexto("Id del infante (formato " + formatoId('I') + "): "));
    std::shared_ptr<Infante> inf = guarderia.buscarInfante(id);
    if (!inf) {
        std::cout << "No se encontro ese infante.\n";
        return;
    }
    mostrarTutoresDe(*inf);
    std::string idActual = aMayusculas(leerTexto("Id del tutor a REEMPLAZAR: "));
    const Tutor* actual = inf->buscarTutor(idActual);
    if (actual == nullptr) {                         // nullptr = ese infante no tiene ese tutor
        std::cout << "Ese infante no tiene un tutor con ese id.\n";
        return;
    }
    std::string nombreAnterior = actual->getNombre();   // se guarda ANTES: el puntero deja de ser válido al reemplazar
    std::cout << "Datos del tutor NUEVO (deja un dato vacio para cancelar):\n";
    Tutor nuevo("", "", "", "");
    if (!capturarTutor(nuevo)) {
        std::cout << "Operacion cancelada.\n";
        return;
    }
    if (inf->reemplazarTutor(idActual, nuevo)) {
        std::cout << "Tutor reemplazado: " << nombreAnterior << " -> " << nuevo.getNombre() << ".\n";
    } else {
        std::cout << "No se pudo reemplazar: el id nuevo ya es tutor de este infante.\n";
    }
}

// 9 -> 4: elimina un tutor de un infante (siempre debe quedar al menos uno).
void MenuGuarderia::opcionEliminarTutor() {
    std::string id = aMayusculas(leerTexto("Id del infante (formato " + formatoId('I') + "): "));
    std::shared_ptr<Infante> inf = guarderia.buscarInfante(id);
    if (!inf) {
        std::cout << "No se encontro ese infante.\n";
        return;
    }
    mostrarTutoresDe(*inf);
    std::string idTutor = aMayusculas(leerTexto("Id del tutor a ELIMINAR: "));
    const Tutor* tutor = inf->buscarTutor(idTutor);
    if (tutor == nullptr) {
        std::cout << "Ese infante no tiene un tutor con ese id.\n";
        return;
    }
    if (inf->getTutores().size() <= 1) {
        std::cout << "Es el UNICO tutor de este infante y siempre debe haber al menos uno. "
                  << "Usa 'Reemplazar' en su lugar.\n";
        return;
    }
    std::string nombreTutor = tutor->getNombre();    // se guarda ANTES de eliminar (el puntero dejaría de ser válido)
    if (!leerSiNo("Eliminar a " + nombreTutor + " como tutor de " + inf->getNombreCompleto() + "? (s/n): ")) {
        std::cout << "No se elimino nada.\n";
        return;
    }
    if (inf->eliminarTutor(idTutor)) {
        std::cout << "Tutor eliminado. Quedan " << inf->getTutores().size() << " tutor(es).\n";
    } else {
        std::cout << "No se pudo eliminar.\n";
    }
}

// Submenú de tutores: se repite hasta que el usuario elige 0 (volver).
void MenuGuarderia::menuTutores() {
    int opcion = -1;
    while (opcion != 0) {
        std::cout << "--- TUTORES ---\n"
                  << "1. Listar tutores (y de quien son tutores)\n"
                  << "2. Agregar tutor a un infante\n"
                  << "3. Reemplazar tutor de un infante\n"
                  << "4. Eliminar tutor de un infante\n"
                  << "0. Volver al menu principal\n";
        opcion = leerEntero("Elige una opcion: ");
        std::cout << "\n";
        switch (opcion) {
            case 1: opcionListarTutores(); break;
            case 2: opcionAgregarTutor(); break;
            case 3: opcionReemplazarTutor(); break;
            case 4: opcionEliminarTutor(); break;
            case 0: break;
            default: std::cout << "Opcion invalida.\n";
        }
        std::cout << "\n";
    }
}

// EL CICLO DEL MENÚ: se repite hasta que el usuario elige 0.
void MenuGuarderia::ejecutar() {
    int opcion = -1;

    while (opcion != 0) {
        mostrarMenu();
        opcion = leerEntero("Elige una opcion: ");
        std::cout << "\n";

        switch (opcion) {                // switch: elige un caso según el valor de 'opcion'
            case 1: opcionInscribir(); break;
            case 2: opcionListar(); break;
            case 3: opcionPorCategoria(); break;
            case 4: opcionBuscar(); break;
            case 5: opcionIngreso(); break;
            case 6: opcionHistorial(); break;
            case 7: opcionEliminar(); break;
            case 8: opcionDemo(); break;
            case 9: menuTutores(); break;
            case 0: std::cout << "Hasta luego.\n"; break;
            default: std::cout << "Opcion invalida.\n";
        }
        std::cout << "\n";
    }
}

// Punto de entrada que llama main(). Crea el menú (un objeto local) y lo ejecuta.
// Aquí NO se repite 'static': esa palabra solo va en la declaración del .h.
void MenuGuarderia::ejecutarSistema() {
    MenuGuarderia menu;                  // llama al constructor
    menu.ejecutar();
}   // al salir de la función el menú se destruye solo (y con él la guardería)