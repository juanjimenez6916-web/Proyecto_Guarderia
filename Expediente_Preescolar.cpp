#include "Expediente_Preescolar.h"
#include "Validaciones.h"
#include <stdexcept>  // std::invalid_argument

Expediente_Preescolar::Expediente_Preescolar(const std::string& idExpediente,
                                             int edadAnios,
                                             bool controlaEsfinteres,
                                             const std::string& salaAsignada)
    : Expediente_Medico(idExpediente),        // primero se construye el padre
      edadAnios(edadAnios),
      controlaEsfinteres(controlaEsfinteres),
      salaAsignada(salaAsignada)
{
    if (!esEdadPreescolarValida(edadAnios)) {
        throw std::invalid_argument("Edad en anios fuera de rango para preescolar.");
    }
}

// [SOBRESCRITURA]
std::string Expediente_Preescolar::getCategoria() const {
    return "Preescolar";
}

// [SOBRESCRITURA] Resumen del padre + datos del preescolar.
// (controlaEsfinteres ? "Si" : "No") elige el texto segun el bool.
std::string Expediente_Preescolar::generarResumen() const {
    return Expediente_Medico::generarResumen() +
           " | Edad: " + std::to_string(edadAnios) + " anios" +
           " | Controla esfinteres: " + (controlaEsfinteres ? "Si" : "No") +
           " | Sala: " + salaAsignada;
}

// [SOBRESCRITURA] Formato de texto. El bool se guarda como 1 o 0.
void Expediente_Preescolar::escribir(std::ostream& os) const {
    Expediente_Medico::escribir(os);
    os << '|' << edadAnios << '|' << (controlaEsfinteres ? 1 : 0) << '|' << salaAsignada;
}

// [SOBRECARGA 1]
void Expediente_Preescolar::actualizarEdad(int nuevaEdad) {
    if (esEdadPreescolarValida(nuevaEdad)) {
        edadAnios = nuevaEdad;
    }
}

// [SOBRECARGA 2] Al cambiar de edad tambien puede cambiar de sala.
void Expediente_Preescolar::actualizarEdad(int nuevaEdad, const std::string& nuevaSala) {
    if (esEdadPreescolarValida(nuevaEdad) && !recortar(nuevaSala).empty()) {
        edadAnios = nuevaEdad;
        salaAsignada = recortar(nuevaSala);
    }
}

int Expediente_Preescolar::getEdadAnios() const { return edadAnios; }
bool Expediente_Preescolar::getControlaEsfinteres() const { return controlaEsfinteres; }
std::string Expediente_Preescolar::getSalaAsignada() const { return salaAsignada; }