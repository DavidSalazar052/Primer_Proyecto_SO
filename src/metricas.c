#include <stdio.h>
#include "terminal.h"
#include "metricas.h"

void calcular_metricas(Camion camiones[]) {
    int i;

    for (i = 0; i < NUM_CAMIONES; i++) {
        camiones[i].espera = camiones[i].inicio - camiones[i].llegada;
        camiones[i].retorno = camiones[i].fin - camiones[i].llegada;
    }
}

void imprimir_resultados(Camion camiones[]) {
    int i;
    double suma_espera = 0.0;
    double suma_retorno = 0.0;

    calcular_metricas(camiones);

    printf("\n================ Resultados FIFO ================\n");
    printf("%-8s %-8s %-12s %-12s %-12s %-12s\n",
           "Camion", "Burst", "Inicio", "Fin", "Espera", "Retorno");
    printf("-------------------------------------------------------------\n");

    for (i = 0; i < NUM_CAMIONES; i++) {
        suma_espera += camiones[i].espera;
        suma_retorno += camiones[i].retorno;

        printf("%-8d %-8d %-12.2f %-12.2f %-12.2f %-12.2f\n",
               camiones[i].id,
               camiones[i].burst,
               camiones[i].inicio,
               camiones[i].fin,
               camiones[i].espera,
               camiones[i].retorno);
    }

    printf("-------------------------------------------------------------\n");
    printf("Espera promedio:  %.2f segundos\n", suma_espera / NUM_CAMIONES);
    printf("Retorno promedio: %.2f segundos\n", suma_retorno / NUM_CAMIONES);
    printf("-------------------------------------------------------------\n");
}
