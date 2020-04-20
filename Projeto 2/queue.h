#include <pthread.h>
#include <stdio.h>

typedef struct fila{
    void **buffer; /** buffer limitado */
    const int capacidade; /** tamanho maximo da fila */
    int tam; /** quantidade de elementos na fila */
    int next_in; /** variavel que representa a posicao dos elementos inseridos na fila */
    int next_out; /** variavel que representa a posicao dos elementos removidos da fila */
    pthread_mutex_t mutex; /** controla o acesso a regiao critica */
    pthread_cond_t empty;  /** conta os lugares vazios no buffer */
    pthread_cond_t full; /** conta os lugares preenchidos no buffer */
} queue_t;

/** Função para inserir elementos na fila */
void enqueue(queue_t *fila, void *value){
    pthread_mutex_lock(&(fila->mutex)); //bloqueia o acesso a regiao critica
    while (fila->tam == fila->capacidade) //se tiver lotada
        pthread_cond_wait(&(fila->full), &(fila->mutex)); //coloca o processo em modo de espera
    printf("enqueue %d\n", *(int *)value);
    fila->buffer[fila->next_in] = value; //insere o valor no buffer
    ++fila->tam; //incrementa quantidade de elementos na fila
    ++fila->next_in; //incrementa a posicao da fila
    fila->next_in %= fila->capacidade; //faz voltar para o inicio se chegou ao fim
    pthread_mutex_unlock(&(fila->mutex)); //desbloqueia o acesso a regiao critica
    pthread_cond_broadcast(&(fila->empty)); //desbloqueia todas as threads bloqueadas na variavel de condição 'empty'
}

/** Função para remover elementos da fila */
void* dequeue(queue_t *fila){
    pthread_mutex_lock(&(fila->mutex)); //bloqueia o acesso a regiao critica
    while (fila->tam == 0) //se tiver vazia
        pthread_cond_wait(&(fila->empty), &(fila->mutex)); //coloca o processo em modo de espera
    void *value = fila->buffer[fila->next_out]; //remove o valor do buffer
    printf("dequeue %d\n", *(int *)value);
    --fila->tam; //decrementa quantidade de elementos na fila
    ++fila->next_out; //incrementa a posicao da fila
    fila->next_out %= fila->capacidade; //faz voltar para o inicio se chegou ao fim
    pthread_mutex_unlock(&(fila->mutex)); //desbloqueia o acesso a regiao critica
    pthread_cond_broadcast(&(fila->full)); //desbloqueia todas as threads bloqueadas na variavel de condição 'empty'
    return value;
}