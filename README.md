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

## Separacion de archivos

```txt
include/terminal.h   -> constantes, estructura Camion, enum Estado y funcion principal
src/main.c           -> entrada del programa
src/terminal.c       -> ejecuta y conecta toda la simulacion FIFO
src/contexto.c       -> variables globales compartidas: semaforos, mutex y contadores
src/tiempo.c         -> calculo del tiempo actual de la simulacion
src/log.c            -> creacion y escritura del log
src/estado.c         -> manejo de estados del camion
src/camiones.c       -> creacion e impresion de camiones de prueba
src/hilos.c          -> rutina del camion, creacion y union de hilos
src/fifo.c           -> planificador FIFO
src/metricas.c       -> calculo e impresion de resultados
logs/operaciones.log -> log generado al ejecutar
```
