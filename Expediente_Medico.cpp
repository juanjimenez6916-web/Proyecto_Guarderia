#include "Expediente_Medico.h"
#include "Utilidades.h"     // unir()

// Constructor: guarda el id y deja el expediente en blanco.
Expediente_Medico::Expediente_Medico(const std::string& idExpediente)
    : idExpediente(idExpediente)
{
    crearExpediente();      // seguro: no es virtual
}

// crearExpediente(): vacia las listas. El UML lo declara 'void' y sin parametros, asi que solo puede PREPARAR el objeto existente.
// Ademas sirve para "reiniciar" un expediente mas adelante.
void Expediente_Medico::crearExpediente() {
    alergias.clear();
    discapacidades.clear();
}

// [SOBRECARGA 1] Solo el nombre de la alergia.
void Expediente_Medico::agregarAlergia(const std::string& alergia) {
    alergias.push_back(alergia);          // push_back agrega al final del vector
}

// [SOBRECARGA 2] Nombre + severidad. REUTILIZA la primera version:
// arma "Polen (alta)" y se lo pasa a la version de un parametro.
void Expediente_Medico::agregarAlergia(const std::string& alergia,
                                       const std::string& severidad) {
    agregarAlergia(alergia + " (" + severidad + ")");
}

void Expediente_Medico::agregarDiscapacidad(const std::string& discapacidad) {
    discapacidades.push_back(discapacidad);
}

// "!" = NO, "||" = O.  Se lee: "alergias NO esta vacio  O  discapacidades NO esta vacio".
// Si la primera mitad es verdadera, C++ ni evalua la segunda (cortocircuito).
bool Expediente_Medico::tieneCondiciones() const {
    return !alergias.empty() || !discapacidades.empty();
}

std::string Expediente_Medico::getIdExpediente() const { return idExpediente; }
const std::vector<std::string>& Expediente_Medico::getAlergias() const { return alergias; }
const std::vector<std::string>& Expediente_Medico::getDiscapacidades() const { return discapacidades; }

// Devuelve UN vector nuevo con alergias y discapacidades juntas (una copia:
// quien lo reciba no puede dañar los datos internos).
std::vector<std::string> Expediente_Medico::consultarCondiciones() const {
    std::vector<std::string> condiciones;
    for (const std::string& a : alergias) {          // "para cada a dentro de alergias"
        condiciones.push_back(a);
    }
    for (const std::string& d : discapacidades) {
        condiciones.push_back(d);
    }
    return condiciones;
}

// Version LEGIBLE para pantalla. Las hijas la llaman con
// Expediente_Medico::generarResumen() y le agregan sus datos propios.
std::string Expediente_Medico::generarResumen() const {
    // Operador ternario:  condición ? valor_si_true : valor_si_false
    // std::string("ninguna") fuerza que ambos lados sean del mismo tipo.
    std::string textoAlergias =
        alergias.empty() ? std::string("ninguna") : unir(alergias, ',');
    std::string textoDiscapacidades =
        discapacidades.empty() ? std::string("ninguna") : unir(discapacidades, ',');

    // getCategoria() es VIRTUAL: devuelve "Lactante" o "Preescolar" segun la hija real.
    return "Expediente " + idExpediente + " [" + getCategoria() + "]" +
           " | Alergias: " + textoAlergias +
           " | Discapacidades: " + textoDiscapacidades;
}

// Version de TEXTO (formato de intercambio). Formato:  categoria|id|alergias(;)|discapacidades(;)
// Las hijas la llaman primero y luego agregan "|dato|dato...".
void Expediente_Medico::escribir(std::ostream& os) const {
    os << getCategoria() << '|' << idExpediente << '|'
       << unir(alergias, ';') << '|' << unir(discapacidades, ';');
}

// [OPERADOR <<] Delega en el metodo virtual escribir() (los operadores no
// pueden ser virtuales). Gracias a eso, "cout << *expediente" escribe
// los datos de la hija correcta aunque solo conozcamos al padre.
std::ostream& operator<<(std::ostream& os, const Expediente_Medico& expediente) {
    expediente.escribir(os);
    return os;
}