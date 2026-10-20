#pragma once
#include <string>
#include <cctype>      // std::toupper

// --- Reglas ---
const int DIGITOS_ID = 4;          // I-0001 y T-0001 llevan 4 digitos
const int DIGITOS_TELEFONO = 10;   // telefono de 10 digitos

// true si el texto NO esta vacio y TODOS sus caracteres son digitos '0'..'9'.
inline bool soloDigitos(const std::string& texto) { //inline: es una palabra clave que se coloca antes de una función
                                                   // para indicarle al compilador que inserte el cuerpo de la función directamente en cada lugar donde sea llamada, 
                                                   //en vez de hacer una llamada a función tradicional.
    if (texto.empty()) {
        return false;
    }
    for (char c : texto) {
        if (c < '0' || c > '9') {          // los caracteres son numeros: '0'..'9' van seguidos
            return false;
        }
    }
    return true;
}

// Comprueba el formato "<letra>-<N digitos>". Ejemplo: ("I-0001", 'I', 4) -> true
inline bool tieneFormatoId(const std::string& texto, char letra, int digitos) {
    // Largo esperado: letra + guion + los dígitos.
    if (texto.size() != static_cast<std::size_t>(2 + digitos)) {
        return false;
        //static_cast es el operador oficial de C++ para realizar conversiones explicitas de tipo de datos en tiempo de compilacion de forma segura y controlada.
        //Convierte el int (con signo) al tipo sin signo que devuelve size().
        //Comparar ambos tipos sin convertir provoca una advertencia del compilador 
        //y puede dar resultados no esperados con numeros negativos
    }
    if (texto[0] != letra || texto[1] != '-') {
        return false;
    }
    return soloDigitos(texto.substr(2));   // substr(2): todo lo que sigue a "I-"
}

inline bool esIdInfanteValido(const std::string& id) {
    return tieneFormatoId(id, 'I', DIGITOS_ID);
}

inline bool esIdTutorValido(const std::string& id) {
    return tieneFormatoId(id, 'T', DIGITOS_ID);
}

inline bool esTelefonoValido(const std::string& telefono) {
    return telefono.size() == static_cast<std::size_t>(DIGITOS_TELEFONO) && soloDigitos(telefono);
}

// Texto de ayuda para mostrar el formato esperado: "I-XXXX" o "T-XXXX".
inline std::string formatoId(char letra) {
    return std::string(1, letra) + "-" + std::string(DIGITOS_ID, 'X');
}

// ---- Ayudas para limpiar lo que escribe el usuario --------------------

// Convierte a MAYUSCULAS (asi "i-0001" tambien se acepta como "I-0001").
// Recibe el texto POR VALOR (una copia) para poder modificarlo y devolverlo.
inline std::string aMayusculas(std::string texto) {
    for (char& c : texto) {                // 'char&': modifica cada caracter del texto original
        // toupper pide un unsigned char; el doble cast evita problemas con caracteres raros.
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    return texto;
}

// Quita los espacios (y tabuladores) del inicio y del final: "  I-0001 " -> "I-0001".
inline std::string recortar(const std::string& texto) {
    const std::string espacios = " \t\r\n";
    std::size_t inicio = texto.find_first_not_of(espacios);   // primer caracter que NO es espacio
    if (inicio == std::string::npos) {
        return "";                                            // era todo espacios (o vacio)
    }
    std::size_t fin = texto.find_last_not_of(espacios);       // ultimo caracter que NO es espacio
    return texto.substr(inicio, fin - inicio + 1);
}