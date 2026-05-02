#include <stdio.h>
#include <string.h>
#include "terminal.h"
#include "camiones.h"

void inicializar_camiones(Camion camiones[]) {
    int bursts[NUM_CAMIONES] = {7, 3, 5, 2, 6, 4, 8, 3};
    int prioridades[NUM_CAMIONES] = {2, 1, 3, 1, 2, 3, 2, 1};
    const char* cargas[NUM_CAMIONES] = {
        "Electrodomesticos",
        "Perecederos",
        "Materiales",
        "Perecederos",
        "Textiles",
        "Quimicos",
        "Maquinaria",
        "Perecederos"
    };
    int i;

    for (i = 0; i < NUM_CAMIONES; i++) {
        camiones[i].id = i + 1;
        camiones[i].burst = bursts[i];
        camiones[i].prioridad = prioridades[i];
        strncpy(camiones[i].tipo_carga, cargas[i], sizeof(camiones[i].tipo_carga) - 1);
        camiones[i].tipo_carga[sizeof(camiones[i].tipo_carga) - 1] = '\0';
        camiones[i].estado = NUEVO;
        camiones[i].autorizado = 0;
        camiones[i].llegada = 0.0;
        camiones[i].inicio = 0.0;
        camiones[i].fin = 0.0;
        camiones[i].espera = 0.0;
        camiones[i].retorno = 0.0;
    }
}

void imprimir_camiones(Camion camiones[]) {
    int i;

    printf("\nCamiones de prueba:\n");
    printf("%-8s %-8s %-10s %-20s\n", "Camion", "Burst", "Prioridad", "Carga");
    printf("----------------------------------------------------\n");

    for (i = 0; i < NUM_CAMIONES; i++) {
        printf("%-8d %-8d %-10d %-20s\n",
               camiones[i].id,
               camiones[i].burst,
               camiones[i].prioridad,
               camiones[i].tipo_carga);
    }
}
