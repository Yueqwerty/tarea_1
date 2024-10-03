// Cliente.hpp
#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include <string>
#include <netinet/in.h>

class Cliente {
private:
    std::string ip;
    int puerto;
    int socket_fd;

    void conectarServidor();
    void manejarJuego();
public:
    Cliente(const std::string &ip, int puerto);
    ~Cliente();
    void iniciar();
};

#endif // CLIENTE_HPP
