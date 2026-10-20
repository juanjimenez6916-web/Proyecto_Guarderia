#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Infante.h"

class Guarderia {
private:
    std::string nombreGuarderia;

    // Lista de PUNTEROS INTELIGENTES a infantes. ¿Por que no objetos por valor?
    //  1) Un vector puede MOVER sus elementos en memoria al crecer o al borrar,
    //     y un puntero normal a un elemento quedaria apuntando a la nada.
    //  2) Otras clases (Registro) deben referirse al MISMO infante real, no a copias.
    //  3) shared_ptr libera la memoria solo cuando nadie mas lo usa (sin new/delete).
    std::vector<std::shared_ptr<Infante>> infantes;

    int capacidadMaxima;
    int cantidadActual;     // redundante con infantes.size(), pero esta en el UML:
                            // se mantiene SINCRONIZADO en cada cambio.

public:
    Guarderia(const std::string& nombreGuarderia, int capacidadMaxima);

    // Cambio respecto al UML: devuelven bool (antes void) para poder avisar si
    // la operación se hizo (true) o no (false: lleno, repetido, no encontrado,
    // o con ids / telefonos que no cumplen el formato de Validaciones.h).
    bool inscribirInfante(const std::shared_ptr<Infante>& infante);
    bool eliminarInfante(const std::string& idInfante);

    // Devuelve un puntero; si no existe devuelve nullptr ("no apunta a nada").
    // Un Infante por valor no podría representar "no encontrado", y una copia
    // no permitiria modificar al infante real.
    std::shared_ptr<Infante> buscarInfante(const std::string& idInfante) const;

    // Separar a los infantes por categoria ("Lactante" / "Preescolar").
    std::vector<std::shared_ptr<Infante>> obtenerPorCategoria(const std::string& categoria) const;

    // [SOBRECARGA] Sin parametro: total. Con parametro: solo de esa categoria.
    int contarInfantes() const;
    int contarInfantes(const std::string& categoria) const;

    // ----- Tutores (se obtienen a partir de los infantes) -----
    // Todos los tutores SIN repetir (un tutor de dos hermanos aparece una vez).
    std::vector<Tutor> obtenerTutores() const;
    // Los infantes de los que ese tutor es tutor.
    std::vector<std::shared_ptr<Infante>> obtenerInfantesDeTutor(const std::string& idTutor) const;

    bool hayCupo() const;
    std::string getNombreGuarderia() const;
    int getCapacidadMaxima() const;
    int getCantidadActual() const;
    const std::vector<std::shared_ptr<Infante>>& getInfantes() const;
};