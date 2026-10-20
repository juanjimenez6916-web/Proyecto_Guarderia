#pragma once
#include "Registro.h"

// 'enum class' crea un tipo con SOLO estos tres valores posibles (se escriben
// SeveridadIrregularidad::Grave). Es mas seguro que usar textos sueltos como
// "grave" o "Grave ", donde un error de dedo pasaria desapercibido.
enum class SeveridadIrregularidad
{
    Ninguna,
    Advertencia,
    Grave
};

class Ficha_Medica : public Registro
{
private:
    bool tieneGolpes;
    float temperatura;
    bool estadoSaludAdecuado;             // resultado guardado por iniciarProceso()
    SeveridadIrregularidad severidad;     // Ninguna = no presenta irregularidades
    std::string descripcionIrregularidad; // que irregularidad es (vacio si no hay)
public:
    Ficha_Medica(const std::string &idProceso,
                 const std::string &fecha,
                 const std::shared_ptr<Infante> &infante,
                 bool tieneGolpes,
                 float temperatura);

    // [SOBRESCRITURA] de los metodos del padre.
    void iniciarProceso() override;              // implementa el virtual puro
    std::string generarReporte() const override; // extiende el reporte del padre

    // [SOBRECARGA] Sin parametros usa el límite por defecto (37.5);
    // con parametro usa el limite que tu elijas.
    // Son 'const': solo CONSULTAN (no modifican la ficha). Quien guarda el
    // resultado es iniciarProceso().
    bool evaluacionSalud() const;
    bool evaluacionSalud(float limiteTemperatura) const;

    // Registra una irregularidad. Devuelve false (y no cambia nada) si la
    // descripción está vacía o la severidad es Ninguna.
    bool registrarIrregularidad(const std::string& descripcion,
                                SeveridadIrregularidad nuevaSeveridad);

    bool getTieneGolpes() const;
    bool getIrregularidades() const;
    SeveridadIrregularidad getSeveridad() const;
    std::string getSeveridadTexto() const;            // "Ninguna", "Advertencia" o "Grave"
    std::string getDescripcionIrregularidad() const;
    float getTemperatura() const;
    bool getEstadoSaludAdecuado() const;
};