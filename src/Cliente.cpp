// Cliente.cpp
#include "Cliente.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <sstream>

Cliente::Cliente(const std::string &ip, int puerto) : ip(ip), puerto(puerto), socket_fd(-1) {
    conectarServidor();
}

Cliente::~Cliente() {
    if(socket_fd != -1)
        close(socket_fd);
}

void Cliente::conectarServidor() {
    // Crear socket
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configurar dirección del servidor
    sockaddr_in direccion;
    std::memset(&direccion, 0, sizeof(direccion));
    direccion.sin_family = AF_INET;
    direccion.sin_port = htons(puerto);

    if(inet_pton(AF_INET, ip.c_str(), &direccion.sin_addr) <= 0) {
        std::cerr << "Dirección IP inválida o formato no soportado.\n";
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    // Conectar al servidor
    if(connect(socket_fd, (struct sockaddr*)&direccion, sizeof(direccion)) < 0) {
        perror("connect");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Conectado al servidor " << ip << ":" << puerto << "\n";
}

void Cliente::iniciar() {
    manejarJuego();
}

void Cliente::manejarJuego() {
    while(true) {
        char buffer[2048] = {0};
        int valread = recv(socket_fd, buffer, sizeof(buffer), 0);
        if(valread <= 0) {
            std::cout << "Conexión cerrada por el servidor.\n";
            break;
        }

        std::string mensaje(buffer, valread);

        if(mensaje.find("START") == 0) {
            std::string quien = mensaje.substr(6);
            std::cout << "Juego iniciado. " << quien << " comienza.\n";
        }
        else if(mensaje.find("YOUR_TURN") == 0) {
            // Solicitar al usuario que ingrese una columna
            int columna;
            while(true) {
                std::cout << "Introduce la columna (1-7): ";
                std::cin >> columna;
                if(std::cin.fail() || columna < 1 || columna > 7) {
                    std::cin.clear(); // Limpiar estado de error
                    std::cin.ignore(10000, '\n'); // Ignorar el resto de la línea
                    std::cout << "Columna inválida. Inténtalo de nuevo.\n";
                }
                else {
                    break;
                }
            }
            // Enviar el movimiento al servidor
            std::string move_msg = "MOVE " + std::to_string(columna-1); // Columnas 0-6
            send(socket_fd, move_msg.c_str(), move_msg.length(), 0);
        }
        else if(mensaje.find("BOARD") == 0) {
            // Obtener el estado del tablero
            std::string estado = mensaje.substr(6);
            // Parsear el estado y mostrarlo
            // Asumiendo que el estado está en formato "cell1,cell2,...;cell1,cell2,...;..."
            std::vector<std::vector<char>> matriz(6, std::vector<char>(7, ' '));
            int row = 0;
            int col = 0;
            std::string cell;
            for(char c : estado) {
                if(c == ',') {
                    if(col < 7)
                        matriz[row][col++] = cell.empty() ? ' ' : cell[0];
                    cell.clear();
                }
                else if(c == ';') {
                    if(col < 7)
                        matriz[row][col++] = cell.empty() ? ' ' : cell[0];
                    row++;
                    col = 0;
                    cell.clear();
                }
                else {
                    cell += c;
                }
            }
            // Mostrar el tablero
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
        else if(mensaje.find("WIN") == 0) {
            std::cout << "¡Has ganado!\n";
        }
        else if(mensaje.find("LOSE") == 0) {
            std::cout << "Has perdido.\n";
        }
        else if(mensaje.find("DRAW") == 0) {
            std::cout << "El juego terminó en empate.\n";
        }
        else if(mensaje.find("END_GAME") == 0) {
            std::cout << "Fin del juego.\n";
            break;
        }
        else if(mensaje.find("ERROR") == 0) {
            std::string error_msg = mensaje.substr(6);
            std::cout << "Error: " << error_msg << "\n";
        }
        else {
            std::cout << "Mensaje desconocido: " << mensaje << "\n";
        }
    }
}
