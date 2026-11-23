#include "Salida_Emergencia.h"
#include "Utilidades.h"   // fechaHoraActual()

Salida_Emergencia::Salida_Emergencia(const std::string& idProceso,
                                     const std::string& fecha,
                                     const std::shared_ptr<Infante>& infante,
                                     const std::string& motivoSalida)
    : Registro(idProceso, fecha, infante),
      motivoSalida(motivoSalida),
      fechaHoraSalida("")
{
}

// [SOBRESCRITURA] Inicia la salida de emergencia, guardando la hora exacta
// y actualizando el estatus del infante a "Salida de Emergencia".
void Salida_Emergencia::iniciarProceso() {
    fechaHoraSalida = fechaHoraActual();
    infante->actualizarEstatus("Salida de Emergencia");
    agregarObservaciones("Salida de emergencia efectuada. Motivo: " + motivoSalida);
}

// [SOBRESCRITURA] Genera el reporte extendiendo la base de Registro.
std::string Salida_Emergencia::generarReporte() const {
    std::string resultado = Registro::generarReporte();
    resultado += " | [Salida Emergencia] Hora: " + fechaHoraSalida;
    resultado += " | Motivo: " + motivoSalida;
    return resultado;
}

std::string Salida_Emergencia::getMotivoSalida() const { return motivoSalida; }
std::string Salida_Emergencia::getFechaHoraSalida() const { return fechaHoraSalida; }