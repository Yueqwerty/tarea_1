// main_cliente.cpp
#include "../../include/Cliente.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if(argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <IP_servidor> <puerto>\n";
        return 1;
    }

    std::string ip_servidor = argv[1];
    int puerto = std::stoi(argv[2]);

    Cliente cliente(ip_servidor, puerto);
    cliente.iniciar();

    return 0;
}
