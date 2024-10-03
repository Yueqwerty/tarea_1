// src/Servidor.cpp
#include "Servidor.hpp"
#include "Juego.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <utility>

Servidor::Servidor(int puerto) : puerto(puerto), server_fd(-1) {}

// Destructor: cerrar el socket del servidor si está abierto
Servidor::~Servidor() {
    if(server_fd != -1)
        close(server_fd);
}

// Iniciar el servidor
void Servidor::iniciar() {
    // Crear socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configurar dirección del servidor
    sockaddr_in direccion;
    std::memset(&direccion, 0, sizeof(direccion));
    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(puerto);

    // Enlazar socket
    if(bind(server_fd, (struct sockaddr*)&direccion, sizeof(direccion)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones
    if(listen(server_fd, 10) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Servidor esperando conexiones en el puerto " << puerto << "...\n";

    // Lista para almacenar clientes esperando para emparejar
    std::vector<std::pair<int, sockaddr_in>> clientes_pendientes;

    while(true) {
        sockaddr_in cliente_addr;
        socklen_t cliente_len = sizeof(cliente_addr);
        int cliente_fd = accept(server_fd, (struct sockaddr*)&cliente_addr, &cliente_len);
        if(cliente_fd < 0) {
            perror("accept");
            continue;
        }

        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            char ip_str[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(cliente_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
            std::cout << "Juego nuevo[" << ip_str << ":" << ntohs(cliente_addr.sin_port) << "]\n";
        }

        // Añadir cliente a la lista de pendientes
        clientes_pendientes.emplace_back(cliente_fd, cliente_addr);

        // Si hay al menos dos clientes, iniciar una partida
        if(clientes_pendientes.size() >= 2) {
            int c1_fd = clientes_pendientes[0].first;
            sockaddr_in c1_addr = clientes_pendientes[0].second;
            int c2_fd = clientes_pendientes[1].first;
            sockaddr_in c2_addr = clientes_pendientes[1].second;

            // Remover los dos primeros clientes de la lista
            clientes_pendientes.erase(clientes_pendientes.begin(), clientes_pendientes.begin()+2);

            // Crear un nuevo hilo para manejar la partida
            hilos.emplace_back([this, c1_fd, c1_addr, c2_fd, c2_addr]() {
                Juego juego(c1_fd, c1_addr, c2_fd, c2_addr, cout_mutex); // Pasar el mutex
                juego.iniciar();
            });
        }
    }

    // Unir hilos antes de salir (aunque en este caso el servidor corre indefinidamente)
    for(auto &t : hilos)
        if(t.joinable())
            t.join();
}
