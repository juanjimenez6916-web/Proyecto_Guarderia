#pragma once
#include "Expediente_Medico.h"

class Expediente_Preescolar : public Expediente_Medico {
private:
    int edadAnios;
    bool controlaEsfinteres;
    std::string salaAsignada;

public:
    Expediente_Preescolar(const std::string& idExpediente,
                          int edadAnios,
                          bool controlaEsfinteres,
                          const std::string& salaAsignada);

    // [SOBRESCRITURA]
    std::string getCategoria() const override;
    std::string generarResumen() const override;
    void escribir(std::ostream& os) const override;

    // [SOBRECARGA] Solo la edad, o la edad junto con la nueva sala.
    void actualizarEdad(int nuevaEdad);
    void actualizarEdad(int nuevaEdad, const std::string& nuevaSala);

    int getEdadAnios() const;
    bool getControlaEsfinteres() const;
    std::string getSalaAsignada() const;
};