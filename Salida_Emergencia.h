#pragma once
#include "Registro.h"

// [NUEVA CLASE]: Salida_Emergencia hereda de Registro.
// Permite documentar la evacuacion o retiro de emergencia de un infante
// especificando el motivo explicito de su salida.
class Salida_Emergencia : public Registro {
private:
    std::string motivoSalida;       // Explicacion detallada del motivo de salida
    std::string fechaHoraSalida;    // Registro de fecha y hora exacta de la salida

public:
    Salida_Emergencia(const std::string& idProceso,
                      const std::string& fecha,
                      const std::shared_ptr<Infante>& infante,
                      const std::string& motivoSalida);

    // [SOBRESCRITURA]
    void iniciarProceso() override;
    std::string generarReporte() const override;

    std::string getMotivoSalida() const;
    std::string getFechaHoraSalida() const;
};