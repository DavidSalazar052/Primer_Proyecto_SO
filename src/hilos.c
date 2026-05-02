#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "terminal.h"
#include "contexto.h"
#include "tiempo.h"
#include "log.h"
#include "estado.h"
#include "hilos.h"

void* rutina_camion(void* arg) {
    Camion* camion = (Camion*)arg;
    char mensaje[180];
    int ocupados;

    cambiar_estado(camion, NUEVO);

    pthread_mutex_lock(&mutex_control);
    while (!camion->autorizado) {
        pthread_cond_wait(&cond_camion, &mutex_control);
    }
    pthread_mutex_unlock(&mutex_control);

    cambiar_estado(camion, BLOQUEADO);
    sem_wait(&sem_muelles);

    pthread_mutex_lock(&mutex_control);
    muelles_ocupados++;
    ocupados = muelles_ocupados;
    camion->inicio = tiempo_actual();
    pthread_mutex_unlock(&mutex_control);

    cambiar_estado(camion, EJECUCION);

    snprintf(mensaje, sizeof(mensaje),
             "[Camion %02d] Tomo muelle. Ocupados: %d/%d",
             camion->id, ocupados, NUM_MUELLES);
    escribir_log(mensaje);

    snprintf(mensaje, sizeof(mensaje),
             "[Camion %02d] Cargando por %d unidad(es)...",
             camion->id, camion->burst);
    escribir_log(mensaje);

    usleep(camion->burst * UNIDAD_TIEMPO_US);

    pthread_mutex_lock(&mutex_control);
    muelles_ocupados--;
    ocupados = muelles_ocupados;
    camion->fin = tiempo_actual();
    camiones_terminados++;
    pthread_cond_signal(&cond_planificador);
    pthread_mutex_unlock(&mutex_control);

    snprintf(mensaje, sizeof(mensaje),
             "[Camion %02d] Libero muelle. Ocupados: %d/%d",
             camion->id, ocupados, NUM_MUELLES);
    escribir_log(mensaje);

    sem_post(&sem_muelles);
    cambiar_estado(camion, TERMINADO);

    return NULL;
}

void crear_hilos(Camion camiones[]) {
    int i;

    for (i = 0; i < NUM_CAMIONES; i++) {
        /* En FIFO, la llegada ocurre cuando el camion entra a la fila. */
        camiones[i].llegada = tiempo_actual();

        if (pthread_create(&camiones[i].hilo, NULL, rutina_camion, &camiones[i]) != 0) {
            perror("Error al crear un camion");
            exit(EXIT_FAILURE);
        }
    }
}

void unir_hilos(Camion camiones[]) {
    int i;

    for (i = 0; i < NUM_CAMIONES; i++) {
        pthread_join(camiones[i].hilo, NULL);
    }
}
