#ifndef CONTEXTO_H
#define CONTEXTO_H

#include <pthread.h>
#include <semaphore.h>
#include <time.h>

extern sem_t sem_muelles;
extern pthread_mutex_t mutex_log;
extern pthread_mutex_t mutex_control;
extern pthread_cond_t cond_camion;
extern pthread_cond_t cond_planificador;

extern struct timespec tiempo_inicio_simulacion;
extern int muelles_ocupados;
extern int camiones_terminados;

#endif
