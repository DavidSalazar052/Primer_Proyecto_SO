#include <time.h>
#include "contexto.h"
#include "tiempo.h"

double tiempo_actual(void) {
    struct timespec ahora;
    clock_gettime(CLOCK_MONOTONIC, &ahora);

    return (double)(ahora.tv_sec - tiempo_inicio_simulacion.tv_sec) +
           (double)(ahora.tv_nsec - tiempo_inicio_simulacion.tv_nsec) / 1000000000.0;
}
