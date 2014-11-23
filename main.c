#include <stdio.h>
 
#include <unistd.h>
 
#include <stdlib.h>
 
#include <pthread.h>
 
#include "prodCons.h"
 
int main() {
    //Cria uma fila para serem armazenados os produtos da thread produtora
    TFila *fila;
     
    //variбveis das linhas de execuзгo produtora e consumidora
    pthread_t TProd, TCons;
     
    //Inicializa a fila
    fila = InicializaFila();
    if (fila == NULL) {
        fprintf(stderr, "main: Fila não iniciou.\n");
        exit(1);
    }
 
    pthread_create(&TProd, NULL, produtor, fila);
    pthread_create(&TCons, NULL, consumidor, fila);
    pthread_join(TProd, NULL);
    pthread_join(TCons, NULL);
    /* Apaga a fila liberando as memуrias utilizadas */
    ApagaFila(fila);
    return 0;
 
}
