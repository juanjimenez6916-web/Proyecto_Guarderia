#include "Ficha_Medica.h"
#include <sstream>
#include <iomanip>

Ficha_Medica::Ficha_Medica(const std::string &idProceso,
                           const std::string &fecha,
                           const std::shared_ptr<Infante> &infante,
                           bool tieneGolpes,
                           float temperatura)
    : Registro(idProceso, fecha, infante), // 1) se construye la parte del padre
      tieneGolpes(tieneGolpes),            // 2) atributos propios
      temperatura(temperatura),
      estadoSaludAdecuado(false), // aun no se ha evaluado
      severidad(SeveridadIrregularidad::Ninguna),   // al inicio: sin irregularidades
      descripcionIrregularidad("")
{
}

// [SOBRECARGA 1] Usa el limite por defecto. Delega en la version con parametro.
// 37.5 es un valor de EJEMPLO del proyecto, no un criterio medico real.
bool Ficha_Medica::evaluacionSalud() const
{
    return evaluacionSalud(37.5f); // la 'f' indica que es un float, no un double
}

// [SOBRECARGA 2] Esta sano si NO tiene golpes Y la temperatura es menor al limite.
// "!" = NO,  "&&" = Y.
bool Ficha_Medica::evaluacionSalud(float limiteTemperatura) const
{
   return !tieneGolpes && temperatura < limiteTemperatura
           && severidad != SeveridadIrregularidad::Grave;
}
// Registra una irregularidad: qué es y que tan seria es.
bool Ficha_Medica::registrarIrregularidad(const std::string& descripcion,
                                          SeveridadIrregularidad nuevaSeveridad) {
    if (descripcion.empty() || nuevaSeveridad == SeveridadIrregularidad::Ninguna) {
        return false;                 // datos incompletos: no se registra nada
    }
    descripcionIrregularidad = descripcion;
    severidad = nuevaSeveridad;
    return true;
}
// [SOBRESCRITURA] Para una ficha, "iniciar el proceso" = evaluar la salud
// y dejar constancia en las observaciones.
void Ficha_Medica::iniciarProceso()
{
    estadoSaludAdecuado = evaluacionSalud();
    if (estadoSaludAdecuado)
    {
        agregarObservaciones("Evaluacion de salud: adecuada"); // metodo heredado del padre
    }
    else
    {
        agregarObservaciones("Evaluacion de salud: NO adecuada");
    }

    if (getIrregularidades()) {
        agregarObservaciones("Irregularidad (" + getSeveridadTexto() + "): " + descripcionIrregularidad);
    }
}

// [SOBRESCRITURA] Toma el reporte del padre y le agrega los datos de la ficha.
std::string Ficha_Medica::generarReporte() const
{
    std::ostringstream temp;
    temp << std::fixed << std::setprecision(1) << temperatura; // 36.6 en vez de 36.600000

    std::string resultado = Registro::generarReporte(); // 'Registro::' = version del padre
    resultado += " | [Ficha Medica] Golpes: ";
    resultado += (tieneGolpes ? "Si" : "No");
    resultado += " | Temp: " + temp.str();
    resultado += " | Irregularidades: ";
    if (getIrregularidades()) {
        resultado += "Si (" + getSeveridadTexto() + ": " + descripcionIrregularidad + ")";
    } else {
        resultado += "No";
    }
    resultado += " | Salud adecuada: ";
    resultado += (estadoSaludAdecuado ? "Si" : "No");
    return resultado;
}

bool Ficha_Medica::getTieneGolpes() const { return tieneGolpes; }
float Ficha_Medica::getTemperatura() const { return temperatura; }
bool Ficha_Medica::getEstadoSaludAdecuado() const { return estadoSaludAdecuado; }
// Se DERIVA de la severidad: asi nunca puede contradecirla (una sola fuente de verdad).
bool Ficha_Medica::getIrregularidades() const { return severidad != SeveridadIrregularidad::Ninguna; }
SeveridadIrregularidad Ficha_Medica::getSeveridad() const { return severidad; }
std::string Ficha_Medica::getDescripcionIrregularidad() const { return descripcionIrregularidad; }
// Convierte el enum a texto para mostrarlo en reportes y mensajes.
std::string Ficha_Medica::getSeveridadTexto() const {
    switch (severidad) {
        case SeveridadIrregularidad::Advertencia: return "Advertencia";
        case SeveridadIrregularidad::Grave:       return "Grave";
        default:                                  return "Ninguna";
    }
}