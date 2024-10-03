// include/Servidor.hpp
#ifndef SERVIDOR_HPP
#define SERVIDOR_HPP

#include <vector>
#include <thread>
#include <netinet/in.h>
#include <mutex>

class Servidor {
private:
    int puerto;
    int server_fd;
    std::vector<std::thread> hilos;
    std::mutex cout_mutex; // Mutex para sincronización

public:
    Servidor(int puerto);
    ~Servidor();
    void iniciar();
};

#endif // SERVIDOR_HPP
