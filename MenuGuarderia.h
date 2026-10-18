#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Guarderia.h"      // se guarda una Guarderia por valor: hace falta verla completa
#include "Registro.h"       // el historial guarda punteros a Registro

class MenuGuarderia {
private:
    // ----- ESTADO DEL SISTEMA (antes eran variables locales de main) -----
    // Al ser atributos de la clase, TODAS las opciones del menú los comparten
    // sin tener que pasárselos como parámetros.
    Guarderia guarderia;                                  // la guardería con sus infantes
    std::vector<std::shared_ptr<Registro>> historial;     // todos los registros creados
    int contadorRegistros;                                // para numerar F-1, G-1, F-2...

    // ----- OPCIONES DEL MENÚ (privadas: solo ejecutar() las llama) -----
    // 'const' al final = esa opción solo CONSULTA, no modifica el sistema.
    void mostrarMenu() const;
    void opcionInscribir();             // 1
    void opcionListar() const;          // 2
    void opcionPorCategoria() const;    // 3
    void opcionBuscar() const;          // 4
    void opcionIngreso();               // 5
    void opcionHistorial() const;       // 6
    void opcionEliminar();              // 7
    void opcionDemo();                  // 8
    void menuTutores();                 // 9 (abre un submenú)
    void opcionListarTutores() const;   //   9 -> 1
    void opcionAgregarTutor();          //   9 -> 2
    void opcionReemplazarTutor();       //   9 -> 3
    void opcionEliminarTutor();         //   9 -> 4
    void inscribirEjemplo(const std::shared_ptr<Infante>& infante);   // ayuda de la 8

public:
    MenuGuarderia();                    // deja el sistema listo (guardería vacía)

    // Ciclo del menú: se repite hasta que el usuario elige 0.
    void ejecutar();

    // MÉTODO STATIC: pertenece a la CLASE, no a un objeto concreto. Por eso se
    // llama como MenuGuarderia::ejecutarSistema(), sin crear antes un objeto.
    // Por dentro crea el menú y lo ejecuta, así quien lo llama (main) no necesita
    // saber nada más.
    static void ejecutarSistema();
};