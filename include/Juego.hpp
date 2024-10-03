// include/Juego.hpp
#ifndef JUEGO_HPP
#define JUEGO_HPP

#include "Tablero.hpp"
#include <string>
#include <netinet/in.h>
#include <mutex>

class Juego {
private:
    int cliente1_fd;
    int cliente2_fd;
    sockaddr_in cliente1_addr;
    sockaddr_in cliente2_addr;
    Tablero tablero;
    char turno; // 'S' para Servidor o 'C' para Cliente
    bool juegoTerminado;
    std::mutex &cout_mutex; // Referencia al mutex

public:
    Juego(int c1_fd, sockaddr_in c1_addr, int c2_fd, sockaddr_in c2_addr, std::mutex &mutex);
    void iniciar();

private:
    void enviarMensaje(int socket, const std::string &mensaje);
    std::string recibirMensaje(int socket);
    void manejarTurno(int socket_jugador, char jugador);
    void enviarEstado();
    void finalizarJuego(char ganador); // 'S', 'C' o 'D' para empate
};

#endif // JUEGO_HPP
