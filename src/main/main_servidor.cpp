// main_servidor.cpp
#include "../../include/Servidor.hpp"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <puerto>\n";
        return 1;
    }

    int puerto = std::stoi(argv[1]);

    Servidor servidor(puerto);
    servidor.iniciar();

    return 0;
}
