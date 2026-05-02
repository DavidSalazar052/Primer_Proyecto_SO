#include <stdio.h>
#include <pthread.h>
#include "terminal.h"
#include "contexto.h"
#include "log.h"
#include "estado.h"

const char* estado_texto(Estado estado) {
    switch (estado) {
        case NUEVO: return "NUEVO";
        case LISTO: return "LISTO";
        case BLOQUEADO: return "BLOQUEADO";
        case EJECUCION: return "EJECUCION";
        case TERMINADO: return "TERMINADO";
        default: return "DESCONOCIDO";
    }
}

void cambiar_estado(Camion* camion, Estado nuevo_estado) {
    char mensaje[160];

    pthread_mutex_lock(&mutex_control);
    camion->estado = nuevo_estado;
    pthread_mutex_unlock(&mutex_control);

    snprintf(mensaje, sizeof(mensaje),
             "[Camion %02d] Estado: %s", camion->id, estado_texto(nuevo_estado));
    escribir_log(mensaje);
}
