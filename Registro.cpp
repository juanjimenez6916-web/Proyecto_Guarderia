#include "Registro.h"

Registro::Registro(const std::string& idProceso,
                   const std::string& fecha,
                   const std::shared_ptr<Infante>& infante)
    : idProceso(idProceso),
      fecha(fecha),
      observaciones(""),          // empieza sin observaciones
      infante(infante)
{
    // Supuesto: 'infante' no es nulo (ver "limitaciones" en la recopilación).
}

// [SOBRECARGA 1] Solo la nota. Las notas se separan con "; ".
void Registro::agregarObservaciones(const std::string& nota) {
    if (!observaciones.empty()) {
        observaciones += "; ";    // '+=' agrega al final del texto
    }
    observaciones += nota;
}

// [SOBRECARGA 2] Nota + autor. REUTILIZA la primera versión
// (arma "Dra. Lopez: Sin sintomas" y se lo pasa).
void Registro::agregarObservaciones(const std::string& nota,
                                    const std::string& autor) {
    agregarObservaciones(autor + ": " + nota);
}

// Reporte BASE (parte común a todos los registros). Las hijas lo llaman con
// Registro::generarReporte() y le agregan lo suyo.
std::string Registro::generarReporte() const {
    std::string reporte = "Proceso " + idProceso +
                          " | Fecha: " + fecha +
                          " | Infante: " + infante->getNombreCompleto();
    if (!observaciones.empty()) {
        reporte += " | Observaciones: " + observaciones;
    }
    return reporte;
}

std::string Registro::getIdProceso() const { return idProceso; }
std::string Registro::getFecha() const { return fecha; }
std::string Registro::getObservaciones() const { return observaciones; }
std::shared_ptr<Infante> Registro::getInfante() const { return infante; }