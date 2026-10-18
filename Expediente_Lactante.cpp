#include "Expediente_Lactante.h"
#include <sstream>    // std::ostringstream: un "cout" que escribe dentro de un texto
#include <iomanip>    // std::setprecision

// CONSTRUCTOR de una hija: lo primero en la lista de inicializacion es
// construir la PARTE DEL PADRE llamando a su constructor.
Expediente_Lactante::Expediente_Lactante(const std::string& idExpediente,
                                         int edadMeses,
                                         const std::string& tipoAlimentacion,
                                         float pesoKg)
    : Expediente_Medico(idExpediente),        // 1) se construye el padre
      edadMeses(edadMeses),                   // 2) luego los atributos propios
      tipoAlimentacion(tipoAlimentacion),
      pesoKg(pesoKg)
{
}

// [SOBRESCRITURA] Cada hija responde con su propia categoria.
std::string Expediente_Lactante::getCategoria() const {
    return "Lactante";
}

// [SOBRESCRITURA] Reutiliza el resumen del padre y le agrega lo propio.
// "Expediente_Medico::generarResumen()" llama especificamente a la version del padre;
// sin el prefijo, la funcion se llamaria a si misma infinitamente.
std::string Expediente_Lactante::generarResumen() const {
    std::ostringstream peso;
    peso << std::fixed << std::setprecision(1) << pesoKg;   // fuerza 1 decimal: 7.5 en vez de 7.500000

    return Expediente_Medico::generarResumen() +
           " | Edad: " + std::to_string(edadMeses) + " meses" +
           " | Alimentacion: " + tipoAlimentacion +
           " | Peso: " + peso.str() + " kg";
}

// [SOBRESCRITURA] Formato de texto: lo del padre + |meses|alimentacion|peso
void Expediente_Lactante::escribir(std::ostream& os) const {
    Expediente_Medico::escribir(os);          // categoria|id|alergias|discapacidades
    os << '|' << edadMeses << '|' << tipoAlimentacion << '|' << pesoKg;
}

// [SOBRECARGA 1] Solo actualiza el peso (valida que sea positivo).
void Expediente_Lactante::actualizarPeso(float nuevoPeso) {
    if (nuevoPeso > 0) {
        pesoKg = nuevoPeso;
    }
}

// [SOBRECARGA 2] Peso y edad a la vez. Reutiliza la version de un parametro.
void Expediente_Lactante::actualizarPeso(float nuevoPeso, int nuevaEdadMeses) {
    actualizarPeso(nuevoPeso);
    if (nuevaEdadMeses > 0) {
        edadMeses = nuevaEdadMeses;
    }
}

int Expediente_Lactante::getEdadMeses() const { return edadMeses; }
std::string Expediente_Lactante::getTipoAlimentacion() const { return tipoAlimentacion; }
float Expediente_Lactante::getPesoKg() const { return pesoKg; }