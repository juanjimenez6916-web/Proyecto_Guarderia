#pragma once // #pragma once evita que el header se incluya dos veces y cause errores de redefinicion.

#include <string>
#include <vector>
#include <ostream>

class Expediente_Medico {
private:
    std::string idExpediente;                 // identidad del expediente
    std::vector<std::string> alergias;        // vector: lista que crece sola
    std::vector<std::string> discapacidades;

public:
    Expediente_Medico(const std::string& idExpediente);

    // DESTRUCTOR VIRTUAL: obligatorio como buena practica en toda clase pensada
    // para herencia. Garantiza que al destruir un objeto mediante un puntero al
    // padre se destruya el objeto COMPLETO (la parte hija incluida).
    virtual ~Expediente_Medico() = default;

    // No es virtual a proposito: el constructor la llama, y un constructor del
    // padre NUNCA ejecuta versiones sobrescritas de las hijas (la parte hija
    // todavia no existe cuando se construye el padre).
    void crearExpediente();                               // deja el expediente en blanco
    std::vector<std::string> consultarCondiciones() const; // alergias + discapacidades juntas

    // [SOBRECARGA] Mismo nombre, distintos parametros. El compilador elige
    // cual usar mirando los argumentos (decision en tiempo de compilacion).
    void agregarAlergia(const std::string& alergia);
    void agregarAlergia(const std::string& alergia, const std::string& severidad);

    void agregarDiscapacidad(const std::string& discapacidad);
    bool tieneCondiciones() const;                        // true si hay al menos una
    std::string getIdExpediente() const;
    const std::vector<std::string>& getAlergias() const;           // devuelve sin copiar y sin permitir modificar
    const std::vector<std::string>& getDiscapacidades() const;

    // [POLIMORFISMO] Metodos que las hijas sobrescriben.
    // El enlace es DINAMICO: C++ elige la version mirando el tipo REAL del objeto
    // en tiempo de ejecucion, no el tipo del puntero.
    virtual std::string getCategoria() const = 0;     // "= 0": virtual PURO. Cada hija DEBE implementarlo.
    virtual std::string generarResumen() const;       // texto legible; las hijas lo extienden
    virtual void escribir(std::ostream& os) const;    // formato de texto; las hijas lo extienden
};

// [OPERADOR DE FLUJO] Un operador NO puede ser virtual, asi que este se delega
// en el metodo virtual escribir(). Asi "cout << expediente" funciona
// polimorficamente: cada hija escribe sus propios datos.
std::ostream& operator<<(std::ostream& os, const Expediente_Medico& expediente);