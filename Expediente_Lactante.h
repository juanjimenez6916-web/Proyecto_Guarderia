#pragma once
#include "Expediente_Medico.h"

// "class Hija : public Padre" = la hija HEREDA todo lo publico del padre
// (id, alergias, agregarAlergia, etc.) y puede agregar lo suyo.
class Expediente_Lactante : public Expediente_Medico {
private:
    int edadMeses;
    std::string tipoAlimentacion;   // "materna", "formula", "mixta"
    float pesoKg;

public:
    Expediente_Lactante(const std::string& idExpediente,
                        int edadMeses,
                        const std::string& tipoAlimentacion,
                        float pesoKg);

    // [SOBRESCRITURA] Misma firma que en el padre. 'override' hace que el
    // compilador avise si te equivocas en la firma (en vez de crear, sin darte
    // cuenta, un metodo distinto).
    std::string getCategoria() const override;
    std::string generarResumen() const override;
    void escribir(std::ostream& os) const override;

    // [SOBRECARGA] Dos versiones con distinta cantidad de parametros.
    void actualizarPeso(float nuevoPeso);
    void actualizarPeso(float nuevoPeso, int nuevaEdadMeses);

    int getEdadMeses() const;
    std::string getTipoAlimentacion() const;
    float getPesoKg() const;
};