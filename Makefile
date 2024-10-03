# Makefile para Cuatro en Línea

CXX = g++
CXXFLAGS = -std=c++11 -Wall -pthread
INCLUDES = -I./include
LDFLAGS =

SRC_DIR = src
INC_DIR = include
BIN_DIR = bin

TARGETS = $(BIN_DIR)/servidor $(BIN_DIR)/cliente

SERVER_SRC = $(SRC_DIR)/Servidor.cpp $(SRC_DIR)/Juego.cpp $(SRC_DIR)/Tablero.cpp $(SRC_DIR)/main/main_servidor.cpp
CLIENT_SRC = $(SRC_DIR)/Cliente.cpp $(SRC_DIR)/Juego.cpp $(SRC_DIR)/Tablero.cpp $(SRC_DIR)/main/main_cliente.cpp

all: $(TARGETS)

$(BIN_DIR)/servidor: $(SERVER_SRC)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

$(BIN_DIR)/cliente: $(CLIENT_SRC)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean
