#pragma once
#include <string>
#include <vector>
#include <memory>        // std::shared_ptr
#include <ostream>
#include "Expediente_Medico.h"
#include "Tutor.h"

class Infante {
private:
    std::string idInfante;
    std::string nombreCompleto;
    std::string estatusActual;     // "Pendiente", "Aceptado" o "Rechazado"

    // Puntero inteligente al PADRE: puede apuntar a un Expediente_Lactante o a
    // un Expediente_Preescolar. NO puede ser un objeto por valor porque:
    //   1) Expediente_Medico es abstracta (no se pueden crear objetos de ella).
    //   2) Guardar una hija por valor "recortaría" sus datos propios.
    std::shared_ptr<Expediente_Medico> expediente;

    std::vector<Tutor> tutores;    // 1..*: siempre habrá al menos uno.

public:
    // El constructor EXIGE un tutor: así la regla "1..*" del UML es imposible de
    // romper: no se puede crear un infante sin tutor.
    Infante(const std::string& idInfante,
            const std::string& nombreCompleto,
            const Tutor& tutorPrincipal,
            const std::shared_ptr<Expediente_Medico>& expediente);

    // Devuelve true si el estatus es válido y se aplicó; false si no.
    bool actualizarEstatus(const std::string& estatus);

    // Devuelve una REFERENCIA (&) al expediente real: lo que modifiques con
    // ella modifica el expediente del infante (una copia no serviría).
    // La versión 'const' permite consultar el expediente de un infante de solo lectura.
    Expediente_Medico& getExpediente();
    const Expediente_Medico& getExpediente() const;

        // ----- Tutores (multiplicidad 1..*: siempre debe quedar al menos uno) -----
    // Todas devuelven bool: true si se hizo el cambio, false si se rechazo.
    bool agregarTutor(const Tutor& tutor);        // rechaza ids repetidos y formatos inválidos
    bool eliminarTutor(const std::string& idTutor);   // rechaza si es el unico tutor
    bool reemplazarTutor(const std::string& idTutorActual, const Tutor& tutorNuevo);

    // Devuelve un puntero al tutor con ese id, o nullptr si este infante no lo tiene.
    // NOTA: el puntero apunta DENTRO del vector, por ende deja de ser valido si agregas o
    // eliminas tutores. 
    const Tutor* buscarTutor(const std::string& idTutor) const;
    bool tieneTutor(const std::string& idTutor) const;

    const std::vector<Tutor>& getTutores() const;

    std::string getIdInfante() const;
    std::string getNombreCompleto() const;
    std::string getEstatusActual() const;
};

// [OPERADOR DE FLUJO] Escribe el infante completo (datos, tutores y expediente).
std::ostream& operator<<(std::ostream& os, const Infante& infante);