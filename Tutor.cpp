#include "Tutor.h"
#include "Utilidades.h"
#include "Validaciones.h"

Tutor::Tutor(const std::string &idTutor,
             const std::string &nombre,
             const std::string &telefono,
             const std::string &parentesco)
    : idTutor(idTutor),
      nombre(nombre),
      telefono(telefono),
      parentesco(parentesco)
{
    // Cuerpo vacio: no hay nada mas que hacer.
}

//Getters
std::string Tutor::getIdTutor() const { return idTutor; }
std::string Tutor::getNombre() const { return nombre; }
std::string Tutor::getTelefono() const { return telefono; }
std::string Tutor::getParentesco() const { return parentesco; }

//Setter con validacion
bool Tutor::setTelefono(const std::string& nuevoTelefono) {
    if (!esTelefonoValido(nuevoTelefono)) {
        return false;                 // rechazado: 'return' sale y NO llega a la asignacion
    }
    telefono = nuevoTelefono;         // aceptado: se guarda
    return true;
}

// [OPERADOR <<] Escribe el tutor en una sola linea: id|nombre|telefono|parentesco
// Funciona con cualquier std::ostream (cout, un flujo de texto en memoria, etc.).
// Devuelve el flujo (os) para permitir encadenar:  cout << tutor << "\n";
std::ostream& operator<<(std::ostream& os, const Tutor& tutor) {
    os << tutor.getIdTutor() << '|' << tutor.getNombre() << '|'
       << tutor.getTelefono() << '|' << tutor.getParentesco();
    return os;
}

// [OPERADOR >>] Lee UNA linea con el mismo formato que escribe operator<<.
// Si el formato es incorrecto, marca el flujo como fallido: asi quien
// lo usa puede preguntar "if (flujo >> tutor)" y saber si la lectura funciono.
std::istream& operator>>(std::istream& is, Tutor& tutor) {
    std::string linea;
    if (std::getline(is, linea)) {                         // lee la linea completa
        std::vector<std::string> campos = dividir(linea, '|');
        if (campos.size() >= 4) {
            tutor = Tutor(campos[0], campos[1], campos[2], campos[3]);   // reemplaza el contenido
        } else {
            is.setstate(std::ios::failbit);                // faltan campos: lectura fallida
        }
    }
    return is;
}


