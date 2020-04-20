#include <stdlib.h>
#include <pthread.h>
#include "queue.h"

const int tam_buffer = 10; /** Tamanho do buffer da fila */
const int produtores = 10; /** Numero de produtores */
const int qtd_prod = 20; /** Quantidade a ser produzida por cada produtor */
const int consumidores = 5; /** Numero de consumidores */

/** Função que representa as operações dos produtores */
void *produtor(void *arg){
    for (int i = 0; i < qtd_prod; ++ i){
        int *value = malloc(sizeof(*value)); //aloca um espaço de memoria para o valor a ser inserido na fila
        *value = i; //associa o numero da interacao ao valor a ser inserido na fila
        enqueue(arg, value); //insere o valor na fila
    }
}

/** Função que representa as operações dos consumidores */
void *consumidor(void *arg){
    for (int i = 0; i < qtd_prod * produtores / consumidores; ++ i){
        int *value = dequeue(arg); //remove o elemento e retorna o valor que foi retirado
        free(value); //libera o valor da memoria
    }
}

int main(){
    int i;
    void *buffer[tam_buffer]; //buffer limitado
    queue_t fila = { buffer, sizeof(buffer) / sizeof(buffer[0]), 0, 0, 0, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER }; //condicoes de inicio da fila

    pthread_t produtores[produtores]; //thread dos produtores
    for (i = 0; i < sizeof(produtores) / sizeof(produtores[0]); ++ i){
        pthread_create(&produtores[i], NULL, produtor, &fila); //cria os produtores
    }
    pthread_t consumidores[consumidores]; //thread dos consumidores
    for (i = 0; i < sizeof(consumidores) / sizeof(consumidores[0]); ++ i){
        pthread_create(&consumidores[i], NULL, consumidor, &fila); //cria os consumidores
    }
    for (i = 0; i < sizeof(produtores) / sizeof(produtores[0]); ++ i){
        pthread_join(produtores[i], NULL); //permite que uma thread espere outra terminar sua execução
    }
    for (i = 0; i < sizeof(consumidores) / sizeof(consumidores[0]); ++ i){
        pthread_join(consumidores[i], NULL); //permite que uma thread espere outra terminar sua execução
    }
}