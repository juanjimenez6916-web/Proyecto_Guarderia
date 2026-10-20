#pragma once
#include <string>
#include <memory>
#include "Infante.h"

class Registro {
protected:      // visible para las hijas, oculto para el resto del programa (el '#' del UML)
    std::string idProceso;
    std::string fecha;            // el UML dice Date; usamos texto "AAAA-MM-DD" por simplicidad
    std::string observaciones;
    std::shared_ptr<Infante> infante;   // apunta al infante REAL de la guarderia (compartido)

public:
    Registro(const std::string& idProceso,
             const std::string& fecha,
             const std::shared_ptr<Infante>& infante);

    // Destructor virtual: obligatorio como buena practica en toda clase para herencia.
    virtual ~Registro() = default;

    // Metodo virtual PURO (= 0): no tiene implementacion aqui. Cada hija DEBE
    // escribir la suya. Esto es lo que hace a Registro ABSTRACTA: no se pueden
    // crear objetos "Registro", solo de sus hijas.
    virtual void iniciarProceso() = 0;

    // [SOBRECARGA] Mismo nombre, distintos parametros.
    void agregarObservaciones(const std::string& nota);
    void agregarObservaciones(const std::string& nota, const std::string& autor);

    // Virtual CON implementacion: el padre da la base y las hijas pueden
    // [SOBRESCRIBIRLO] para extenderla.
    virtual std::string generarReporte() const;

    std::string getIdProceso() const;
    std::string getFecha() const;
    std::string getObservaciones() const;
    std::shared_ptr<Infante> getInfante() const;
};