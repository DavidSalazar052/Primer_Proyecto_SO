#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "contexto.h"

sem_t sem_muelles;
pthread_mutex_t mutex_log = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_control = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_camion = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_planificador = PTHREAD_COND_INITIALIZER;

struct timespec tiempo_inicio_simulacion;
int muelles_ocupados = 0;
int camiones_terminados = 0;
