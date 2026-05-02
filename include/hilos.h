#ifndef HILOS_H
#define HILOS_H

#include "terminal.h"

void* rutina_camion(void* arg);
void crear_hilos(Camion camiones[]);
void unir_hilos(Camion camiones[]);

#endif
