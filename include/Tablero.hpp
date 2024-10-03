// Tablero.hpp
#ifndef TABLERO_HPP
#define TABLERO_HPP

#include <vector>
#include <string>

class Tablero {
private:
    std::vector<std::vector<char>> matriz; // 6 filas x 7 columnas
public:
    Tablero();
    bool insertarFicha(int columna, char jugador); // Inserta una ficha en una columna
    bool verificarGanador(char jugador);          // Verifica si un jugador ha ganado
    bool estaLleno() const;                      // Verifica si el tablero está lleno
    std::string obtenerEstado() const;            // Obtiene una representación del estado del tablero
    void mostrar() const;                         // Muestra el tablero en la terminal
};

#endif // TABLERO_HPP
