# Informe breve - Terminal de Carga Automatizada FIFO

## Descripcion

El programa simula una terminal de carga donde varios camiones intentan utilizar un numero limitado de muelles.
Cada camion se representa con un hilo POSIX (`pthread`). La terminal tiene 3 muelles, controlados con un semaforo.

## Algoritmo utilizado

Se utiliza FIFO, First-In First-Out. Los camiones son autorizados por el planificador en el mismo orden en que fueron creados.
Como existen 3 muelles, se permite que los primeros 3 camiones trabajen al mismo tiempo. Cuando uno termina, el siguiente camion de la fila recibe permiso para entrar.

## Separacion de funciones

Las funciones que estaban dentro de `src/terminal.c` fueron separadas por responsabilidad:

- `src/terminal.c`: funcion principal de ejecucion de la simulacion.
- `src/contexto.c`: variables globales compartidas.
- `src/tiempo.c`: calculo del tiempo actual.
- `src/log.c`: inicializacion y escritura del log.
- `src/estado.c`: conversion y cambio de estados.
- `src/camiones.c`: inicializacion e impresion de camiones.
- `src/hilos.c`: creacion, rutina y union de hilos.
- `src/fifo.c`: planificador FIFO.
- `src/metricas.c`: calculo e impresion de resultados.

## Sincronizacion

- `sem_t sem_muelles`: limita el uso simultaneo de muelles a 3 camiones.
- `pthread_mutex_t mutex_log`: protege el archivo `logs/operaciones.log` para evitar condiciones de carrera.
- `pthread_mutex_t mutex_control`: protege variables compartidas como cantidad de muelles ocupados, estados y contador de camiones terminados.
- `pthread_cond_t`: permite que los camiones esperen hasta ser autorizados por FIFO.

## Estados del hilo

Cada camion reporta los siguientes estados:

- NUEVO: el hilo fue creado.
- LISTO: el camion esta listo y fue autorizado segun FIFO.
- BLOQUEADO: el camion esta esperando acceso al muelle.
- EJECUCION: el camion esta usando un muelle.
- TERMINADO: el camion termino su carga.

## Prevencion de deadlock

El programa evita interbloqueo porque:

1. Los mutex se usan durante secciones cortas.
2. El semaforo siempre se libera con `sem_post` despues de usar el muelle.
3. Los hilos no mantienen un mutex bloqueado mientras esperan indefinidamente por otro recurso.
4. El planificador autoriza camiones de forma ordenada y espera notificaciones de finalizacion.

## Resultados

El programa muestra una tabla con:

- inicio de cada camion,
- fin de cada camion,
- tiempo de espera,
- tiempo de retorno,
- espera promedio,
- retorno promedio.
