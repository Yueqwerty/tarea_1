// Tablero.cpp
#include "Tablero.hpp"
#include <iostream>

// Constructor: inicializa el tablero vacío
Tablero::Tablero() : matriz(6, std::vector<char>(7, ' ')) {}

// Inserta una ficha en la columna especificada para el jugador ('S' o 'C')
bool Tablero::insertarFicha(int columna, char jugador) {
    if(columna < 0 || columna >= 7)
        return false; // Columna inválida
    for(int i = 5; i >= 0; --i) { // Desde la fila inferior hacia arriba
        if(matriz[i][columna] == ' ') {
            matriz[i][columna] = jugador;
            return true;
        }
    }
    return false; // Columna llena
}

// Verifica si el jugador ha alineado cuatro fichas
bool Tablero::verificarGanador(char jugador) {
    // Verificar horizontalmente
    for(int i = 0; i < 6; ++i)
        for(int j = 0; j < 4; ++j)
            if(matriz[i][j] == jugador && matriz[i][j+1] == jugador &&
               matriz[i][j+2] == jugador && matriz[i][j+3] == jugador)
                return true;

    // Verificar verticalmente
    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 7; ++j)
            if(matriz[i][j] == jugador && matriz[i+1][j] == jugador &&
               matriz[i+2][j] == jugador && matriz[i+3][j] == jugador)
                return true;

    // Verificar diagonalmente (izquierda a derecha)
    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 4; ++j)
            if(matriz[i][j] == jugador && matriz[i+1][j+1] == jugador &&
               matriz[i+2][j+2] == jugador && matriz[i+3][j+3] == jugador)
                return true;

    // Verificar diagonalmente (derecha a izquierda)
    for(int i = 0; i < 3; ++i)
        for(int j = 3; j < 7; ++j)
            if(matriz[i][j] == jugador && matriz[i+1][j-1] == jugador &&
               matriz[i+2][j-2] == jugador && matriz[i+3][j-3] == jugador)
                return true;

    return false;
}

// Verifica si el tablero está completamente lleno
bool Tablero::estaLleno() const {
    for(int j = 0; j < 7; ++j)
        if(matriz[0][j] == ' ')
            return false;
    return true;
}

// Obtiene una representación en cadena del estado del tablero
std::string Tablero::obtenerEstado() const {
    std::string estado = "";
    for(int i = 0; i < 6; ++i) {
        for(int j = 0; j < 7; ++j) {
            estado += matriz[i][j];
            if(j < 6) estado += ",";
        }
        estado += ";";
    }
    return estado;
}

// Muestra el tablero en la terminal
void Tablero::mostrar() const {
    std::cout << "-------------\n";
    for(int i = 0; i < 6; ++i) {
        std::cout << "|";
        for(int j = 0; j < 7; ++j)
            std::cout << matriz[i][j] << "|";
        std::cout << "\n";
    }
    std::cout << "-------------\n";
    std::cout << "1 2 3 4 5 6 7\n";
}
