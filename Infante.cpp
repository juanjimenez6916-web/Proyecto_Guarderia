#include "Infante.h"
#include "Validaciones.h"   // reglas de formato (ids y telefonos)

// CONSTRUCTOR. El orden de la lista de inicializacion coincide con el orden de
// declaración de los atributos (C++ inicializa SIEMPRE en el orden de declaración).
Infante::Infante(const std::string& idInfante,
                 const std::string& nombreCompleto,
                 const Tutor& tutorPrincipal,
                 const std::shared_ptr<Expediente_Medico>& expediente)
    : idInfante(idInfante),
      nombreCompleto(nombreCompleto),
      estatusActual("Pendiente"),     // todo infante nuevo aun no pasa por el ingreso
      expediente(expediente)          // se copia el puntero (ambos apuntan al mismo expediente)
{
    tutores.push_back(tutorPrincipal);   // cumple la multiplicidad 1..*
}

// Solo acepta los tres estatus validos. Devuelve false (y NO cambia nada) si no.
bool Infante::actualizarEstatus(const std::string& estatus) {
    //Los string distinguen mayusculas: "aceptado" != "Aceptado".
    bool esValido = (estatus == "Pendiente" ||
                     estatus == "Aceptado"  ||
                     estatus == "Rechazado");

    if (!esValido) {
        return false;                  // "guard clause": salimos temprano si algo esta mal
    }
    estatusActual = estatus;
    return true;
}

// '*expediente' convierte el puntero en el objeto al que apunta (desreferenciar).
// Devolvemos referencia (&) para no copiar y poder modificar el original.
Expediente_Medico& Infante::getExpediente() { return *expediente; }
const Expediente_Medico& Infante::getExpediente() const { return *expediente; }

// Busca un tutor por id. Devuelve la direccion del tutor dentro del vector (o
// nullptr si no esta). '&t' = "la dirección de t".
const Tutor* Infante::buscarTutor(const std::string& idTutor) const {
    for (const Tutor& t : tutores) {
        if (t.getIdTutor() == idTutor) {
            return &t;
        }
    }
    return nullptr;
}

bool Infante::tieneTutor(const std::string& idTutor) const {
    return buscarTutor(idTutor) != nullptr;
}

// Agrega un tutor ADICIONAL. Se rechaza si el formato es invalido o si ese id
// ya es tutor de este infante.
bool Infante::agregarTutor(const Tutor& tutor) {
    if (!esIdTutorValido(tutor.getIdTutor()) || !esTelefonoValido(tutor.getTelefono())) {
        return false;
    }
    if (tieneTutor(tutor.getIdTutor())) {
        return false;
    }
    tutores.push_back(tutor);
    return true;
}

// Elimina un tutor. La multiplicidad 1..* del UML exige que SIEMPRE quede al
// menos uno, asi que se rechaza si es el u nico (para eso existe reemplazarTutor).
bool Infante::eliminarTutor(const std::string& idTutor) {
    if (tutores.size() <= 1) {
        return false;
    }
    for (std::size_t i = 0; i < tutores.size(); ++i) {
        if (tutores[i].getIdTutor() == idTutor) {
            tutores.erase(tutores.begin() + i);   // erase pide una POSICION, no un numero
            return true;
        }
    }
    return false;                                 // no estaba
}

// Reemplaza un tutor por otro, en la MISMA posicion de la lista.
// Como no cambia la cantidad de tutores, funciona tambien con el unico tutor.
bool Infante::reemplazarTutor(const std::string& idTutorActual, const Tutor& tutorNuevo) {
    if (!esIdTutorValido(tutorNuevo.getIdTutor()) || !esTelefonoValido(tutorNuevo.getTelefono())) {
        return false;
    }
    // El id nuevo no puede coincidir con OTRO tutor de este infante
    // (si puede ser el mismo id: seria actualizar los datos de ese tutor).
    if (tutorNuevo.getIdTutor() != idTutorActual && tieneTutor(tutorNuevo.getIdTutor())) {
        return false;
    }
    for (Tutor& t : tutores) {                    // 'Tutor&': el tutor REAL, no una copia
        if (t.getIdTutor() == idTutorActual) {
            t = tutorNuevo;                       // la asignacion reemplaza todo su contenido
            return true;
        }
    }
    return false;                                 // el tutor a reemplazar no existe
}
const std::vector<Tutor>& Infante::getTutores() const { return tutores; }

std::string Infante::getIdInfante() const { return idInfante; }
std::string Infante::getNombreCompleto() const { return nombreCompleto; }
std::string Infante::getEstatusActual() const { return estatusActual; }

// [OPERADOR <<] Escribe un registro completo. Se escribe en pantalla con cout.
// Formato (una etiqueta al inicio de cada línea):
//   INFANTE|id|nombre|estatus
//   T|id|nombre|telefono|parentesco      (una por tutor)
//   E|categoria|id|alergias|discap|...   (el expediente)
//   FIN
std::ostream& operator<<(std::ostream& os, const Infante& infante) {
    os << "INFANTE|" << infante.getIdInfante() << '|'
       << infante.getNombreCompleto() << '|'
       << infante.getEstatusActual() << '\n';

    for (const Tutor& t : infante.getTutores()) {
        os << "T|" << t << '\n';                  // usa el operator<< de Tutor
    }

    // Usa el operator<< de Expediente_Medico, que llama al escribir() VIRTUAL:
    // escribe los datos de la hija correcta (Lactante o Preescolar).
    os << "E|" << infante.getExpediente() << '\n';
    os << "FIN\n";
    return os;
}