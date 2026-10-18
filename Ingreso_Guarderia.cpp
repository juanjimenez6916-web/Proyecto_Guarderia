#include "Ingreso_Guarderia.h"

Ingreso_Guarderia::Ingreso_Guarderia(const std::string &idProceso,
                                     const std::string &fecha,
                                     const std::shared_ptr<Infante> &infante) : Registro(idProceso, fecha, infante),
                                                                                ingresoAprobado(false),
                                                                                motivoRechazo(""),
                                                                                advertencia("")
{
}

// [SOBRESCRITURA] Aquí "iniciar el proceso" = abrir el trámite de ingreso.
// (No recibe la ficha porque el UML no tiene parámetros: la decisión se toma
// después, en validarAcceso()).
void Ingreso_Guarderia::iniciarProceso()
{
    ingresoAprobado = false;
    motivoRechazo = "";
    advertencia = "";
    infante->actualizarEstatus("Pendiente"); // modifica al infante REAL (compartido)
    agregarObservaciones("Proceso de ingreso iniciado");
}

// Decide el ingreso. Cada comprobación que falla guarda el motivo, rechaza y sale.
bool Ingreso_Guarderia::validarAcceso(const Ficha_Medica &ficha)
{
    // 1) La ficha debe pertenecer a ESTE infante (la identidad se verifica antes que la salud).
    if (ficha.getInfante()->getIdInfante() != infante->getIdInfante())
    {
        motivoRechazo = "La ficha medica pertenece a otro infante";
        rechazarYActualizarEstatus();
        return false;
    }
    // 2) Una irregularidad GRAVE impide el ingreso. Se revisa ANTES que la salud
    //    general para poder dar un motivo preciso.
    if (ficha.getSeveridad() == SeveridadIrregularidad::Grave)
    {
        motivoRechazo = "Irregularidad grave: " + ficha.getDescripcionIrregularidad();
        rechazarYActualizarEstatus();
        return false;
    }
    // 3) El estado de salud (golpes y temperatura) debe ser adecuado.
    //    (evaluacionSalud() es const: solo consulta.)
    if (!ficha.evaluacionSalud())
    {
        motivoRechazo = "Estado de salud no adecuado";
        rechazarYActualizarEstatus();
        return false;
    }
    // 4) Todo en orden: se aprueba. Si la ficha trae una irregularidad de tipo
    //    Advertencia, se aprueba CON advertencia y queda constancia de ella.
    ingresoAprobado = true;
    motivoRechazo = "";
    infante->actualizarEstatus("Aceptado");
    if (ficha.getSeveridad() == SeveridadIrregularidad::Advertencia)
    {
        advertencia = ficha.getDescripcionIrregularidad();
        agregarObservaciones("Ingreso aprobado CON ADVERTENCIA: " + advertencia);
    }
    else
    {
        advertencia = "";
        agregarObservaciones("Ingreso aprobado");
    }
    return true;
}

// Único punto de rechazo: así no se repite este código en cada comprobación.
void Ingreso_Guarderia::rechazarYActualizarEstatus()
{
    ingresoAprobado = false;
    advertencia = "";
    infante->actualizarEstatus("Rechazado");
    agregarObservaciones("Ingreso rechazado");
}

// [SOBRESCRITURA] Reporte del padre + el resultado del ingreso.
std::string Ingreso_Guarderia::generarReporte() const
{
    std::string resultado = Registro::generarReporte();
    resultado += " | [Ingreso] Aprobado: ";
    resultado += (ingresoAprobado ? "Si" : "No");
    if (!advertencia.empty()) {
        resultado += " (CON ADVERTENCIA: " + advertencia + ")";
    }
    if (!ingresoAprobado && !motivoRechazo.empty())
    {
        resultado += " | Motivo: " + motivoRechazo;
    }
    return resultado;
}

bool Ingreso_Guarderia::getIngresoAprobado() const { return ingresoAprobado; }
std::string Ingreso_Guarderia::getMotivoRechazo() const { return motivoRechazo; }
std::string Ingreso_Guarderia::getAdvertencia() const { return advertencia; }