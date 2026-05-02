*EIF 212 --- Sistemas Operativos \| I Ciclo 2026*

Documentación Técnica del Proyecto

**Sistema de Gestión de una Terminal de Carga Automatizada**

*Primer Proyecto · Programación Concurrente en C (pthreads)*

Fecha de entrega: 03 de mayo de 2026

Modalidad: Grupal

# 1. Descripción General del Sistema

El proyecto implementa en lenguaje C un simulador de terminal logística
donde múltiples hilos POSIX (Camiones) compiten por un recurso limitado
(Muelles de Carga). El proceso principal actúa como Servidor de la
Terminal: inicializa el entorno, crea los hilos y coordina el acceso
mediante semáforos y un algoritmo de planificación.

  ----------------------- -----------------------------------------------
  **Parámetro**           **Valor**

  **Camiones (hilos)**    8 (NUM_CAMIONES)

  **Muelles disponibles** 3 (NUM_MUELLES)

  **Unidad de tiempo**    300 000 µs por unidad de burst
                          (UNIDAD_TIEMPO_US)

  **Algoritmos**          FIFO (implementado)

  **Bibliotecas clave**   \<pthread.h\> \<semaphore.h\> \<time.h\>
                          \<unistd.h\>
  ----------------------- -----------------------------------------------

# 2. Manejo de Hilos (pthread)

## Ciclo de vida e implementación

Cada camión se representa con un struct Camion que agrupa: id, burst,
prioridad, tipo_carga, estado (enum), pthread_t hilo, autorizado
(bandera), y los timestamps llegada, inicio, fin para calcular métricas.
La creación registra el tiempo de llegada antes de pthread_create; la
unión se realiza con pthread_join al finalizar todos los hilos.

La rutina principal rutina_camion() en hilos.c implementa el ciclo
completo:

> cambiar_estado(camion, NUEVO);
>
> pthread_cond_wait(&cond_camion, &mutex_control); // espera
> autorización
>
> cambiar_estado(camion, BLOQUEADO);
>
> sem_wait(&sem_muelles); // compite por muelle
>
> cambiar_estado(camion, EJECUCION);
>
> usleep(camion-\>burst \* UNIDAD_TIEMPO_US); // simula carga
>
> sem_post(&sem_muelles);
>
> cambiar_estado(camion, TERMINADO); // pthread_join recolecta

# 3. Sincronización --- Semáforos y Mutex

Todos los primitivos de sincronización se definen en contexto.c y se
exponen en contexto.h. La separación en dos mutex independientes
(mutex_control y mutex_log) es deliberada: evita que el orden de
adquisición genere un ciclo de espera circular.

  ----------------------- ------------------ -----------------------------------
  **Primitivo**           **Recurso          **Función y uso**
                          protegido**        

  **sem_muelles**         Muelles físicos    sem_init(\..., 3). Limita a 3 hilos
                          (cap. 3)           en la sección crítica
                                             simultáneamente. sem_wait /
                                             sem_post en cada acceso.

  **mutex_control**       Variables de       Protege muelles_ocupados,
                          estado compartidas camiones_terminados,
                                             camion-\>autorizado y los
                                             timestamps de inicio/fin.

  **mutex_log**           Archivo de log     Protege la escritura en
                          (Log de            logs/operaciones.log. Sin este
                          Operaciones)       mutex, múltiples hilos corromperían
                                             el archivo con escrituras
                                             intercaladas.

  **cond_camion**         Señal planificador Los hilos bloquean aquí con
                          → hilo             pthread_cond_wait hasta que
                                             planificar_fifo() les asigna
                                             autorizado = 1.

  **cond_planificador**   Señal hilo →       El planificador espera aquí hasta
                          planificador       que un camión termina y decrementa
                                             muelles_ocupados.
  ----------------------- ------------------ -----------------------------------

# 4. Módulo de Estados del Hilo

El módulo estado.c implementa el modelo clásico de cinco estados. Cada
transición es registrada en el log con cambiar_estado(), que adquiere
mutex_control antes de modificar el campo para garantizar atomicidad.

  --------------- ------------------- ----------------------------------------
  **Estado**      **Activado por**    **Descripción**

  **NUEVO**       pthread_create()    Primera instrucción de rutina_camion().
                                      El hilo existe pero aún no compite por
                                      recursos.

  **LISTO**       planificar_fifo()   El planificador activa cond_camion y
                  autoriza            pone autorizado = 1. El hilo está en
                                      cola para sem_wait.

  **BLOQUEADO**   sem_wait() con      Todos los muelles ocupados. El hilo
                  semáforo en 0       queda suspendido en sem_wait() hasta que
                                      otro libere.

  **EJECUCION**   sem_wait() retorna  Obtiene muelle. Ejecuta usleep(burst ×
                                      UNIDAD_TIEMPO_US). Escribe en el log de
                                      operaciones.

  **TERMINADO**   sem_post() + señal  Libera muelle, señala al planificador
                  cond                con cond_planificador y retorna NULL.
                                      pthread_join recolecta.
  --------------- ------------------- ----------------------------------------

# 5. Sección Crítica y Exclusión Mutua

El sistema identifica tres secciones críticas independientes, cada una
protegida con el mecanismo más adecuado según la naturaleza del recurso:

  ------------------ --------------- -------------------------------------
  **Sección          **Mecanismo**   **Justificación**
  Crítica**                          

  **Muelles de       Semáforo        Recurso de capacidad N=3. El semáforo
  Carga**            (sem_muelles)   permite acceso concurrente controlado
                                     hasta ese límite, a diferencia del
                                     mutex que lo restringe a 1.

  **Variables de     mutex_control   muelles_ocupados y
  estado globales**                  camiones_terminados son leídas y
                                     escritas por múltiples hilos. El
                                     mutex garantiza operaciones atómicas.

  **Log de           mutex_log       Escritura concurrente al archivo de
  Operaciones**                      texto sin protección produciría
                                     entradas mezcladas e ilegibles (race
                                     condition).
  ------------------ --------------- -------------------------------------

# 6. Lógica de Planificación

## FIFO (implementado --- fifo.c)

La función planificar_fifo() corre en el hilo principal. Mantiene dos
contadores: siguiente (próximo a autorizar) y activos (en muelles).
Autoriza hasta NUM_MUELLES camiones, luego espera en cond_planificador
hasta que alguno termine y libere un slot:

> while (camiones_terminados \< NUM_CAMIONES) {
>
> while (activos \< NUM_MUELLES && siguiente \< NUM_CAMIONES) {
>
> camiones\[siguiente\].autorizado = 1;
>
> pthread_cond_broadcast(&cond_camion);
>
> activos++; siguiente++;
>
> }
>
> pthread_cond_wait(&cond_planificador, &mutex_control);
>
> activos -= (camiones_terminados - terminados_anteriores);
>
> }

Característica principal: sin desalojo. Si el primer camión tiene un
burst alto, los siguientes esperan más (Efecto Convoy).

# 7. Análisis Comparativo de Algoritmos de Planificación

## 7.1 FIFO --- Métricas por camión

  ------------ ----------- ----------- ------------------- ---------- ---------- ---------- -----------
  **ID**       **Burst**   **Prio.**   **Tipo Carga**      **Inicio   **Fin      **Espera   **Retorno
                                                           (s)**      (s)**      (s)**      (s)**

  01           7           2           Electrodomésticos   0.09       2.20       0.09       2.20

  02           3           1           Perecederos         0.11       1.03       0.11       1.03

  03           5           3           Materiales          0.15       1.66       0.15       1.66

  04           2           1           Perecederos         1.06       1.67       1.06       1.67

  05           6           2           Textiles            1.78       3.62       1.77       3.62

  06           4           3           Químicos            2.28       3.52       2.28       3.52

  07           8           2           Maquinaria          3.59       6.04       3.59       6.04

  08           3           1           Perecederos         3.72       4.66       3.72       4.66

  **PROMEDIO                                               1.60       3.05       1.59       3.05
  →**                                                                                       
  ------------ ----------- ----------- ------------------- ---------- ---------- ---------- -----------

## 

## Conclusión del Análisis

FIFO es óptimo cuando los bursts son homogéneos y el convoy no
representa un riesgo. Su ventaja es la simplicidad y el mínimo overhead
de administración.

**8. Prevención de Interbloqueo (Deadlock)**

El sistema neutraliza las cuatro condiciones de Coffman necesarias para
que ocurra un deadlock:

  --------------- --------------------- ---------------------------------
  **Condición**   **Estrategia**        **Implementación**

  **Exclusión     Inherente / aceptada  Los muelles son exclusivos por
  Mutua**                               naturaleza del problema. Se
                                        acepta y se gestiona con
                                        semáforos.

  **Retención y   Adquisición atómica   Ningún hilo retiene mutex_control
  Espera**                              mientras espera sem_muelles.
                                        Secuencia: liberar mutex →
                                        sem_wait → readquirir mutex.

  **Sin           Liberación voluntaria FIFO: sin desalojo forzoso. RR:
  Apropiación**                         el hilo libera el muelle
                                        voluntariamente con sem_post al
                                        agotar su quantum.

  **Espera        Orden global de       Orden siempre: mutex_control →
  Circular**      adquisición           sem_muelles → mutex_log. Nunca se
                                        invierte, eliminando el ciclo de
                                        dependencias.
  --------------- --------------------- ---------------------------------

# 9. Estructura del Proyecto

  -------------------- ----------------------------------------------------
  **Archivo**          **Responsabilidad**

  **src/main.c**       Punto de entrada. Llama a ejecutar_fifo_terminal().

  **src/terminal.c**   Orquesta el flujo completo: init → crear hilos →
                       planificar → unir → resultados.

  **src/hilos.c**      pthread_create, rutina_camion (ciclo de vida),
                       pthread_join.

  **src/fifo.c**       Planificador FIFO: autoriza camiones respetando el
                       orden de llegada y la capacidad de muelles.

  **src/estado.c**     Transiciones de estado con cambiar_estado();
                       impresión textual del estado.

  **src/log.c**        Escritura atómica a logs/operaciones.log protegida
                       por mutex_log.

  **src/metricas.c**   Calcula espera (inicio − llegada) y retorno (fin −
                       llegada); imprime tabla de resultados.

  **src/contexto.c**   Define semáforo, mutex, condiciones y contadores
                       globales.

  **src/tiempo.c**     tiempo_actual() con CLOCK_MONOTONIC --- resolución
                       de nanosegundos.

  **include/\*.h**     Cabeceras con tipos, constantes y prototipos.
                       Definen la interfaz pública de cada módulo.

  **Makefile**         gcc -Wall -Wextra -pthread. Targets: make / make run
                       / make clean.
  -------------------- ----------------------------------------------------
