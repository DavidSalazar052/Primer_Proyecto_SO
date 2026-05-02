#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include "terminal.h"
#include "contexto.h"
#include "log.h"

void inicializar_log(void) {
    FILE* archivo;

    mkdir("logs", 0777);

    archivo = fopen(LOG_FILE, "w");
    if (archivo == NULL) {
        perror("No se pudo crear el log");
        exit(EXIT_FAILURE);
    }

    fprintf(archivo, "===== Log de Operaciones - FIFO =====\n");
    fclose(archivo);
}

void escribir_log(const char* mensaje) {
    FILE* archivo;

    pthread_mutex_lock(&mutex_log);

    archivo = fopen(LOG_FILE, "a");
    if (archivo != NULL) {
        fprintf(archivo, "%s\n", mensaje);
        fclose(archivo);
    }

    printf("%s\n", mensaje);

    pthread_mutex_unlock(&mutex_log);
}
