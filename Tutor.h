#pragma once
#include <string>

class Tutor
{
private:
    std::string idTutor;
    std::string nombre;
    std::string telefono;
    std::string parentesco;

public:
    Tutor(const std::string &idTutor,
          const std::string &nombre,
          const std::string &telefono,
          const std::string &parentesco);
    // Getters que leen los datos
    std::string getIdTutor() const;
    std::string getNombre() const;
    std::string getTelefono() const;
    std::string getParentesco() const;
    // Setter que solo cambia al telefono
    bool setTelefono(const std::string &nuevoTelefono);
};

// [OPERADORES DE FLUJO] Van FUERA de la clase porque en "cout << tutor"
// el operando de la izquierda es cout, no el tutor (no pueden ser metodos).
std::ostream& operator<<(std::ostream& os, const Tutor& tutor);   // escribir
std::istream& operator>>(std::istream& is, Tutor& tutor);         // leer
