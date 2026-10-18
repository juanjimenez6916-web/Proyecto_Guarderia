#pragma once
#include "Registro.h"
#include "Ficha_Medica.h"

class Ingreso_Guarderia : public Registro {
private:
    bool ingresoAprobado;
    std::string motivoRechazo;
    std::string advertencia;

public:
    Ingreso_Guarderia(const std::string& idProceso,
                      const std::string& fecha,
                      const std::shared_ptr<Infante>& infante);

    // [SOBRESCRITURA]
    void iniciarProceso() override;
    std::string generarReporte() const override;

    // Dependencia: recibe la ficha por referencia constante (solo la consulta).
    bool validarAcceso(const Ficha_Medica& ficha);
    void rechazarYActualizarEstatus();

    bool getIngresoAprobado() const;
    std::string getMotivoRechazo() const;
    std::string getAdvertencia() const;
};