#include <stdio.h>
#include <pthread.h>
#include "terminal.h"
#include "contexto.h"
#include "log.h"
#include "estado.h"
#include "fifo.h"

void planificar_fifo(Camion camiones[]) {
    int siguiente = 0;
    int activos = 0;
    int terminados_anteriores = 0;
    char mensaje[160];

    while (camiones_terminados < NUM_CAMIONES) {
        while (activos < NUM_MUELLES && siguiente < NUM_CAMIONES) {
            cambiar_estado(&camiones[siguiente], LISTO);

            snprintf(mensaje, sizeof(mensaje),
                     "[Planificador FIFO] Autoriza al Camion %02d",
                     camiones[siguiente].id);
            escribir_log(mensaje);

            pthread_mutex_lock(&mutex_control);
            camiones[siguiente].autorizado = 1;
            pthread_cond_broadcast(&cond_camion);
            pthread_mutex_unlock(&mutex_control);

            activos++;
            siguiente++;
        }

        pthread_mutex_lock(&mutex_control);
        terminados_anteriores = camiones_terminados;
        while (camiones_terminados == terminados_anteriores) {
            pthread_cond_wait(&cond_planificador, &mutex_control);
        }
        activos -= (camiones_terminados - terminados_anteriores);
        pthread_mutex_unlock(&mutex_control);
    }
}
