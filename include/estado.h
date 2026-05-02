#ifndef ESTADO_H
#define ESTADO_H

#include "terminal.h"

const char* estado_texto(Estado estado);
void cambiar_estado(Camion* camion, Estado nuevo_estado);

#endif
