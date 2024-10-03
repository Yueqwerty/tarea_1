// src/Juego.cpp
#include "Juego.hpp"
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <arpa/inet.h> // Añadido para inet_ntop

// Constructor
Juego::Juego(int c1_fd, sockaddr_in c1_addr, int c2_fd, sockaddr_in c2_addr, std::mutex &mutex)
    : cliente1_fd(c1_fd), cliente2_fd(c2_fd),
      cliente1_addr(c1_addr), cliente2_addr(c2_addr),
      turno('S'), juegoTerminado(false), cout_mutex(mutex) {}

// Iniciar la partida
void Juego::iniciar() {
    // Seleccionar al azar quién comienza
    srand(time(NULL));
    if(rand() % 2 == 0) {
        turno = 'S';
        std::lock_guard<std::mutex> lock(cout_mutex);
        char ip1[INET_ADDRSTRLEN];
        char ip2[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(cliente1_addr.sin_addr), ip1, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(cliente2_addr.sin_addr), ip2, INET_ADDRSTRLEN);
        std::cout << "Juego [" << ip1 << ":" << ntohs(cliente1_addr.sin_port) 
                  << " vs " << ip2 << ":" << ntohs(cliente2_addr.sin_port) 
                  << "]: inicia el servidor.\n";
    } else {
        turno = 'C';
        std::lock_guard<std::mutex> lock(cout_mutex);
        char ip1[INET_ADDRSTRLEN];
        char ip2[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(cliente1_addr.sin_addr), ip1, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(cliente2_addr.sin_addr), ip2, INET_ADDRSTRLEN);
        std::cout << "Juego [" << ip1 << ":" << ntohs(cliente1_addr.sin_port) 
                  << " vs " << ip2 << ":" << ntohs(cliente2_addr.sin_port) 
                  << "]: inicia el cliente.\n";
    }

    // Enviar mensaje de inicio a ambos clientes
    std::string start_msg1 = "START " + std::string(turno == 'S' ? "SERVER" : "CLIENT");
    std::string start_msg2 = "START " + std::string(turno == 'C' ? "SERVER" : "CLIENT");

    enviarMensaje(cliente1_fd, start_msg1);
    enviarMensaje(cliente2_fd, start_msg2);

    // Enviar estado inicial del tablero
    enviarEstado();

    // Bucle del juego
    while(!juegoTerminado) {
        if(turno == 'S') {
            manejarTurno(cliente1_fd, 'S');
            turno = 'C';
        } else {
            manejarTurno(cliente2_fd, 'C');
            turno = 'S';
        }
    }

    // Cerrar conexiones
    close(cliente1_fd);
    close(cliente2_fd);
}

// Enviar un mensaje a un cliente
void Juego::enviarMensaje(int socket, const std::string &mensaje) {
    send(socket, mensaje.c_str(), mensaje.length(), 0);
}

// Recibir un mensaje de un cliente
std::string Juego::recibirMensaje(int socket) {
    char buffer[1024] = {0};
    int valread = recv(socket, buffer, 1024, 0);
    if(valread > 0) {
        return std::string(buffer, valread);
    }
    return "";
}

// Manejar el turno de un jugador
void Juego::manejarTurno(int socket_jugador, char jugador) {
    // Enviar mensaje para indicar que es su turno
    enviarMensaje(socket_jugador, "YOUR_TURN");

    // Recibir el movimiento
    std::string mensaje = recibirMensaje(socket_jugador);
    if(mensaje.find("MOVE") == 0) {
        int columna = std::stoi(mensaje.substr(5));
        bool exito = tablero.insertarFicha(columna, jugador);
        if(!exito) {
            // Enviar mensaje de error y repetir turno
            enviarMensaje(socket_jugador, "ERROR Columna inválida o llena.");
            manejarTurno(socket_jugador, jugador);
            return;
        }

        // Loggear el movimiento en el servidor
        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            char ip1[INET_ADDRSTRLEN];
            char ip2[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(cliente1_addr.sin_addr), ip1, INET_ADDRSTRLEN);
            inet_ntop(AF_INET, &(cliente2_addr.sin_addr), ip2, INET_ADDRSTRLEN);
            std::cout << "Juego [" << ip1 << ":" << ntohs(cliente1_addr.sin_port) 
                      << " vs " << ip2 << ":" << ntohs(cliente2_addr.sin_port) 
                      << "]: jugador " << (jugador == 'S' ? "Servidor" : "Cliente") 
                      << " juega columna " << (columna +1) << ".\n";
        }

        // Enviar estado actualizado del tablero a ambos clientes
        enviarEstado();

        // Verificar si hay un ganador
        if(tablero.verificarGanador(jugador)) {
            finalizarJuego(jugador);
            return;
        }

        // Verificar si el tablero está lleno (empate)
        if(tablero.estaLleno()) {
            finalizarJuego('D'); // 'D' para empate
            return;
        }
    }
}

// Enviar el estado actual del tablero a ambos clientes
void Juego::enviarEstado() {
    std::string estado = "BOARD " + tablero.obtenerEstado();
    enviarMensaje(cliente1_fd, estado);
    enviarMensaje(cliente2_fd, estado);
}

// Finalizar el juego y notificar a los clientes
void Juego::finalizarJuego(char ganador) {
    juegoTerminado = true;
    if(ganador == 'D') {
        enviarMensaje(cliente1_fd, "DRAW");
        enviarMensaje(cliente2_fd, "DRAW");
        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            char ip1[INET_ADDRSTRLEN];
            char ip2[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(cliente1_addr.sin_addr), ip1, INET_ADDRSTRLEN);
            inet_ntop(AF_INET, &(cliente2_addr.sin_addr), ip2, INET_ADDRSTRLEN);
            std::cout << "Juego [" << ip1 << ":" << ntohs(cliente1_addr.sin_port) 
                      << " vs " << ip2 << ":" << ntohs(cliente2_addr.sin_port) 
                      << "]: empate.\n";
        }
    } else {
        std::string win_msg = ganador == 'S' ? "WIN" : "LOSE";
        std::string lose_msg = ganador == 'S' ? "LOSE" : "WIN";
        enviarMensaje(cliente1_fd, ganador == 'S' ? win_msg : lose_msg);
        enviarMensaje(cliente2_fd, ganador == 'C' ? win_msg : lose_msg);
        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            char ip1[INET_ADDRSTRLEN];
            char ip2[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(cliente1_addr.sin_addr), ip1, INET_ADDRSTRLEN);
            inet_ntop(AF_INET, &(cliente2_addr.sin_addr), ip2, INET_ADDRSTRLEN);
            std::cout << "Juego [" << ip1 << ":" << ntohs(cliente1_addr.sin_port) 
                      << " vs " << ip2 << ":" << ntohs(cliente2_addr.sin_port) 
                      << "]: gana " << (ganador == 'S' ? "Servidor" : "Cliente") << ".\n";
        }
    }

    // Enviar mensaje de fin de juego
    enviarMensaje(cliente1_fd, "END_GAME");
    enviarMensaje(cliente2_fd, "END_GAME");
}
