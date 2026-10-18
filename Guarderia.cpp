#include "Guarderia.h"
#include "Validaciones.h"   // reglas de formato (ids y teléfonos)

Guarderia::Guarderia(const std::string& nombreGuarderia, int capacidadMaxima)
    : nombreGuarderia(nombreGuarderia),
      capacidadMaxima(capacidadMaxima),
      cantidadActual(0)               // al inicio no hay infantes
{
}

// GUARD CLAUSES: primero se revisa todo lo que IMPEDIRÍA la operación y se sale
// temprano con 'return false'. El caso feliz queda al final, sin if anidados.
bool Guarderia::inscribirInfante(const std::shared_ptr<Infante>& infante) {
    if (!infante) {                   // puntero vacío (nullptr)
        return false;
    }
    if (cantidadActual >= capacidadMaxima) {   // sin cupo
        return false;
    }
    // RED DE SEGURIDAD: el menú ya valida lo que escribe el usuario, pero la
    // guardería también rechaza datos con mal formato (así nadie puede colarlos
    // creando infantes por otro camino).
    if (!esIdInfanteValido(infante->getIdInfante())) {
        return false;
    }
    for (const Tutor& t : infante->getTutores()) {
        if (!esIdTutorValido(t.getIdTutor()) || !esTelefonoValido(t.getTelefono())) {
            return false;
        }
    }
    if (buscarInfante(infante->getIdInfante()) != nullptr) {   // id repetido
        return false;
    }
    // Ya NO se copia el infante: la guardería y quien lo creó COMPARTEN el mismo
    // objeto. (Copiar un Infante copiaría el puntero del expediente, no el
    // expediente: dos infantes "distintos" compartirían uno solo.)
    infantes.push_back(infante);
    // static_cast<int>: size() devuelve un tipo sin signo (size_t); convertimos
    // a int de forma explícita porque cantidadActual es int.
    cantidadActual = static_cast<int>(infantes.size());
    return true;
}

bool Guarderia::eliminarInfante(const std::string& idInfante) {
    // size_t es el tipo correcto para recorrer un vector por índice.
    for (std::size_t i = 0; i < infantes.size(); ++i) {
        if (infantes[i]->getIdInfante() == idInfante) {   // '->' accede a un miembro mediante un puntero
            // erase necesita una POSICIÓN (iterador), no un número:
            // begin() es la posición del primero; sumarle i avanza hasta el elemento i.
            infantes.erase(infantes.begin() + i);
            cantidadActual = static_cast<int>(infantes.size());
            return true;
        }
    }
    return false;                     // recorrimos todo y no estaba
}

std::shared_ptr<Infante> Guarderia::buscarInfante(const std::string& idInfante) const {
    for (const std::shared_ptr<Infante>& inf : infantes) {
        if (inf->getIdInfante() == idInfante) {
            return inf;               // devolvemos el puntero (no una copia del infante)
        }
    }
    return nullptr;                   // señal de "no encontrado"
}

// POLIMORFISMO en acción: no preguntamos "¿eres de tal clase?". Le pedimos al
// expediente su categoría (getCategoria es virtual) y comparamos el texto.
std::vector<std::shared_ptr<Infante>> Guarderia::obtenerPorCategoria(const std::string& categoria) const {
    std::vector<std::shared_ptr<Infante>> resultado;
    for (const std::shared_ptr<Infante>& inf : infantes) {
        if (inf->getExpediente().getCategoria() == categoria) {
            resultado.push_back(inf);
        }
    }
    return resultado;
}

// [SOBRECARGA 1] Total de infantes.
int Guarderia::contarInfantes() const {
    return cantidadActual;
}

// [SOBRECARGA 2] Infantes de una categoría.
int Guarderia::contarInfantes(const std::string& categoria) const {
    return static_cast<int>(obtenerPorCategoria(categoria).size());
}

// Junta los tutores de TODOS los infantes sin repetir (se compara por idTutor).
// Cada Infante guarda sus propios tutores, por eso hay que recorrerlos todos.
std::vector<Tutor> Guarderia::obtenerTutores() const {
    std::vector<Tutor> resultado;
    for (const std::shared_ptr<Infante>& inf : infantes) {
        for (const Tutor& t : inf->getTutores()) {
            bool yaEsta = false;
            for (const Tutor& agregado : resultado) {
                if (agregado.getIdTutor() == t.getIdTutor()) {
                    yaEsta = true;
                    break;                        // ya lo teníamos: dejamos de buscar
                }
            }
            if (!yaEsta) {
                resultado.push_back(t);
            }
        }
    }
    return resultado;
}

// "¿De quién es tutor?": los infantes cuya lista de tutores incluye ese id.
std::vector<std::shared_ptr<Infante>> Guarderia::obtenerInfantesDeTutor(const std::string& idTutor) const {
    std::vector<std::shared_ptr<Infante>> resultado;
    for (const std::shared_ptr<Infante>& inf : infantes) {
        if (inf->tieneTutor(idTutor)) {
            resultado.push_back(inf);
        }
    }
    return resultado;
}

bool Guarderia::hayCupo() const {
    return cantidadActual < capacidadMaxima;
}

std::string Guarderia::getNombreGuarderia() const { return nombreGuarderia; }
int Guarderia::getCapacidadMaxima() const { return capacidadMaxima; }
int Guarderia::getCantidadActual() const { return cantidadActual; }

const std::vector<std::shared_ptr<Infante>>& Guarderia::getInfantes() const {
    return infantes;
}