#pragma once
#include <string>
#include <vector>
#include <sstream>   // std::istringstream: leer desde un texto como si fuera un flujo

// dividir("a|b|c", '|')  ->  ["a", "b", "c"]
inline std::vector<std::string> dividir(const std::string& texto, char separador) {
    std::vector<std::string> partes;      // aqui acumulamos los pedazos
    std::string parte;                    // el pedazo que estamos leyendo
    std::istringstream flujo(texto);      // convierte el texto en un flujo de lectura

    // getline con 3 parámetros lee hasta encontrar el separador (en vez de leer hasta el salto de linea que es lo normal).
    while (std::getline(flujo, parte, separador)) {
        partes.push_back(parte);
    }

    // getline NO devuelve el campo vacio final de "a|b|" sino que se lo "traga" por asi decirlo.
    // Lo agregamos a mano para no perder el ultimo campo cuando esta vacío
    // (por ejemplo, un tutor al que le falta el ultimo dato).
    if (!texto.empty() && texto.back() == separador) {
        partes.push_back("");
    }
    return partes;
}

//unir(["a","b","c"], ';')  ->  "a;b;c"
inline std::string unir(const std::vector<std::string>& partes, char separador) {
    std::string resultado;
    for (std::size_t i = 0; i < partes.size(); ++i) {
        if (i > 0) {                      // el separador va ENTRE elementos, no al inicio
            resultado += separador;
        }
        resultado += partes[i];
    }
    return resultado;
}