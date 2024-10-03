# Cuatro en Línea

Proyecto Cliente/Servidor para el juego Cuatro en Línea implementado en C++.

## Estructura del Proyecto

- **bin/**: Ejecutables del servidor y cliente.
- **include/**: Archivos de cabecera (.hpp) de las clases.
- **src/**: Archivos de implementación (.cpp) y puntos de entrada (main_servidor.cpp y main_cliente.cpp).
- **docs/**: Documentación adicional.
- **Makefile**: Script para compilar el proyecto.
- **.gitignore**: Especifica qué archivos y directorios ignorar en Git.

## Compilación

Para compilar el proyecto, ejecuta:

```bash
make
```

Esto generará los ejecutables en el directorio `bin/`.

## Ejecución

### Servidor

```bash
./bin/servidor <puerto>
```

### Cliente

```bash
./bin/cliente <IP_servidor> <puerto>
```

