#include <pthread.h> 
 
#define TAMANHOFILA 5
 
typedef struct {
    int fila[TAMANHOFILA];
    long prim, ult;
    int cheio, vazio;
    pthread_mutex_t *mut;
    pthread_cond_t *notFull, *notEmpty;
} TFila;
 
//up libera
void up(pthread_mutex_t *mutex);
//down bloqueia
void down(pthread_mutex_t *mutex);
 
void *produtor(void *args);
 
void *consumidor(void *args);
 
 
 
TFila *InicializaFila(void);
 
void ApagaFila(TFila *q);
 
void AdicionaElemento(TFila *q, int in);
 
void RetiraElemento(TFila *q, int *out);
