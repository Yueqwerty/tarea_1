// common.hpp
#ifndef COMMON_HPP
#define COMMON_HPP

#include <string>

// Definición de tipos de mensajes
enum MessageType {
    START_GAME,      // Inicio del juego
    YOUR_TURN,       // Turno del jugador
    MOVE,            // Movimiento realizado
    UPDATE_BOARD,    // Actualización del tablero
    WIN,             // Victoria del jugador
    LOSE,            // Derrota del jugador
    DRAW,            // Empate
    END_GAME,        // Fin del juego
    ERROR_MESSAGE    // Mensaje de error
};

// Estructura de mensaje para la comunicación
struct Message {
    MessageType type;
    std::string data; // Datos adicionales, como columna de movimiento o estado del tablero
};

#endif // COMMON_HPP
