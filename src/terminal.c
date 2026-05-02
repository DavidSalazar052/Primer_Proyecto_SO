#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include "terminal.h"
#include "contexto.h"
#include "log.h"
#include "camiones.h"
#include "hilos.h"
#include "fifo.h"
#include "metricas.h"

void ejecutar_fifo(void) {
    Camion camiones[NUM_CAMIONES];

    inicializar_log();

    if (sem_init(&sem_muelles, 0, NUM_MUELLES) != 0) {
        perror("Error al inicializar semaforo");
        exit(EXIT_FAILURE);
    }

    camiones_terminados = 0;
    muelles_ocupados = 0;

    clock_gettime(CLOCK_MONOTONIC, &tiempo_inicio_simulacion);

    printf("====Terminal de Carga - FIFO====\n");

    inicializar_camiones(camiones);
    imprimir_camiones(camiones);

    crear_hilos(camiones);
    algoritmo_fifo(camiones);
    unir_hilos(camiones);
    imprimir_resultados(camiones);

    sem_destroy(&sem_muelles);
}
