#ifndef TERMINAL_H
#define TERMINAL_H

#include <pthread.h>

#define NUM_MUELLES 3
#define NUM_CAMIONES 8
#define UNIDAD_TIEMPO_US 300000
#define LOG_FILE "logs/operaciones.log"

typedef enum {
    NUEVO,
    LISTO,
    BLOQUEADO,
    EJECUCION,
    TERMINADO
} Estado;

typedef struct {
    int id;
    int burst;
    int prioridad;
    char tipo_carga[40];
    Estado estado;
    pthread_t hilo;
    int autorizado;
    double llegada;
    double inicio;
    double fin;
    double espera;
    double retorno;
} Camion;

void ejecutar_fifo(void);

#endif
