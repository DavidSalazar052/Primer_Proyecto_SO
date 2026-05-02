# Sistema de Gestion de una Terminal de Carga Automatizada - FIFO

Version reducida del proyecto usando solamente el algoritmo FIFO.

En esta version, las funciones que estaban juntas en `src/terminal.c` fueron separadas en archivos por responsabilidad.

## Requisitos

- Ubuntu o Ubuntu en WSL
- gcc
- make

Instalacion de herramientas:

```bash
sudo apt update
sudo apt install build-essential make
```

## Compilar

```bash
make
```

## Ejecutar

```bash
make run
```

O directamente:

```bash
./bin/terminal_carga
```

## Limpiar

```bash
make clean
```

