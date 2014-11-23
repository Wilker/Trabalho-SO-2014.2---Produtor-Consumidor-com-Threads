#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "prodCons.h"
#define QTDPRODUTOS 100
  
//down bloqueia o mutex
void down(pthread_mutex_t *mutex){
    pthread_mutex_lock(mutex);
}
//up libera o mutex
void up(pthread_mutex_t *mutex){
    pthread_mutex_unlock(mutex);
}
  
//Função a ser executado pela Thread produtora
void *produtor (void *f){
    TFila *filaPC;
    int i;
    /*Recebe o ponteiro da fila iniciada no main*/
    filaPC = (TFila *)f;
    /*Repete o processo 100 vezes adicionando 100 elementos na fila*/
    for (i=0;i<QTDPRODUTOS;i++) {
        //bloqueia fila
        down (filaPC->mut);
        /*
        Verifica se a fila estÃ¡ cheia, se a fila estiver cheia ela desbloqueia condicionalmente a
        variavel mutex, permitinho que seja retirado outro elemento.
        */
        while (filaPC->cheio) {
            printf ("Produtor: FALHOU. Sem espaço para produção.\n");
            pthread_cond_wait (filaPC->notFull, filaPC->mut);
        }
        /*Acessa a fila adicionando um elemento do buffer*/
        AdicionaElemento (filaPC, i);
        /*exibe o elemento adicionado*/
        printf("Produtor: %d\n",i);
        /*Desbloqueia a fila para as demais threads*/
        up (filaPC->mut);
        /*Envia o sinal avizando que a fila nao esta vazia*/
        pthread_cond_signal (filaPC->notEmpty);
        /*Atrasa a thread em 0.3 segundos*/
        usleep (300000);
    }
  
    return (NULL);
  
}
 
//Função a ser executada pela thread consumidora
void *consumidor (void *f){
    TFila *filaPC;
    int i, d;
    filaPC = (TFila *)f;
    for (i=0;i<QTDPRODUTOS;i++) {
        /*bloqueia a fila*/
        down (filaPC->mut);
        while (filaPC->vazio) {
            printf ("Consumidor: FALHOU. Sem produtos a serem consumidos.\n");
            /*desbloqueia a fila sobre a condiÃ§Ã£o de estar vazia*/
            pthread_cond_wait (filaPC->notEmpty, filaPC->mut);
        }
        /*Retira o elemento da fila e o salva na variÑvel d*/
        RetiraElemento (filaPC, &d);
        /*mostra o dado retirado da fila*/
        printf ("Consumidor: consumiu %d.\n", d);
        /*desbloqueia a fila*/
        up (filaPC->mut);
        /*sinaliza que a fila nÑ?o esta mais cheia.*/
        pthread_cond_signal (filaPC->notFull);
        /*Atrasa a thread em segundos aleatÐ·rios (maximo de 0.8s)*/
        srand(time(NULL));
        usleep (rand() % 800000);
    }
    return (NULL);
}
 
TFila *InicializaFila (void){
    /*define f um tipo definido fila*/
    TFila *f;
    /*aloca espaço para a fila*/
    f = (TFila *)malloc (sizeof (TFila));
       if (!f) {
        printf("Falha na alocação da fila\n");
        system("pause");
        exit(1);
    }
    f->vazio = 1; /*vazio=verdadeiro*/
    f->cheio = 0; /*cheio=falso*/
    f->prim = 0; /*primeiro elemento posiÃ§Ã£o 0*/
    f->ult = 0; /*ultimo elemento posiÃ§Ã£o 0*/
    /* Aloca espaço para as variÃ¡veis e inicializa com as funÃ§Ã£o da API "pthread.h" */
    f->mut = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
    pthread_mutex_init (f->mut, NULL);
    f->notFull = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
    pthread_cond_init (f->notFull, NULL);
    f->notEmpty = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
    pthread_cond_init (f->notEmpty, NULL);
    /*retorna a fila f*/
    return (f);
}
  
void ApagaFila (TFila *f){
    /*DestrÃ³i as variÃ¡veis iniciadas na funÃ§Ã£o "InicializaFila" e desaloca seu espaço*/
    pthread_mutex_destroy (f->mut);
    free (f->mut);
    pthread_cond_destroy (f->notFull);
    free (f->notFull);
    pthread_cond_destroy (f->notEmpty);
    free (f->notEmpty);
    free (f);
}
  
void AdicionaElemento (TFila *f, int e){
    /*iguala a ultima posiÑÑ?o com a variÑvel de entrada in*/
    f->fila[f->ult] = e;
    /*Atualiza a ultima posiÑÑ?o*/
    f->ult++;
    /*retorna a posiÃ§Ã£o do ultimo elemento Ð¯ posiÃ§Ã£o do primeiro quando o
    Vetor buf chega ao fim*/
    if (f->ult == TAMANHOFILA)
    f->ult = 0;
    /*se a posiÑÑ?o do ultimo for igual a primeira, significa que o vetor esta cheio*/
    if (f->ult == f->prim)
        f->cheio = 1;
    /*informa que o vetor nÑ?o estÑ vazio apÐ·s adicionar um elemento*/
    f->vazio = 0;
    return;
}
  
void RetiraElemento (TFila *f, int *x){
    /*atribui a variÑvel de saÑda o valor do primeiro elemento da fila*/
    *x = f->fila[f->prim];
    /*atualiza o primeiro elemento*/
    f->prim++;
    /*se a posiÑÑ?o do primeiro elemento Ð¶ a ultima retorna a primeira posiÑÑ?o do buffer*/
    if (f->prim == TAMANHOFILA)
        f->prim = 0;
    /*se a posiÑÑ?o do primeiro elemento Ð¶ igual a posiÑÑ?o do ultimo, atribui fila como vazia*/
    if (f->prim == f->ult)
        f->vazio = 1;
    /*como foi retirado um elemento atribui a fila como nÑ?o cheia*/
    f->cheio = 0;
    return;
}
