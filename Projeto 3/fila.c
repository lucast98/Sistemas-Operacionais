#include <stdlib.h>
#include <stdio.h>
#include "fila.h"

/** Cria uma nova fila */
Fila *criaFila(){
    Fila *fila = (Fila *)malloc(sizeof(Fila));
    fila->tam = 0; //nao tem nada ainda
    fila->inicio = NULL; //vazia 
    fila->fim = NULL; //vazia
    return fila;
}

/** Insere pagina na fila */
void push(Fila *fila, int pag){
    Elemento *elem = (Elemento *)malloc(sizeof(Elemento));
    elem->pag = pag;
    elem->prox = NULL; //indica que é o fim
    fila->tam++;
    
    if(fila->inicio == NULL){
        fila->inicio = elem;
        fila->fim = fila->inicio;
    }
    else{
        fila->fim->prox = elem; //novo elemento será o proximo do atual ultimo
        fila->fim = elem; //novo elemento é o novo ultimo
    }
}

/** Remove pagina da fila (primeiro elemento) */
int pop(Fila *fila){
    int pag;
    Elemento *inicio = fila->inicio;
    if(inicio == NULL) //nao tem nada na fila
        return -1;
    else{
        pag = inicio->pag;
        fila->inicio = inicio->prox; //atualiza o inicio
        fila->tam--;
        free(inicio);
    }
    return pag;
}

/** Verifica se fila esta vazia */
int estaVazia(Fila *fila){
    if(fila->inicio == NULL)
        return 1; //vazio
    else
        return 0; //nao vazio
}

void moveFim(Fila *fila, int pag){
    Elemento *elem = fila->inicio;
    Elemento *ant;
    if(elem == NULL)
        return;

    //loop para procurar o elemento a ser movido
    while (elem != NULL){
        if(elem->pag == pag)
            break; //indica que achou o elemento certo
        ant = elem;
        elem = elem->prox;
    }

    //indica que já é o ultimo elemento
    if(elem == fila->fim)
        return;

    if(elem == fila->inicio){
        fila->inicio = elem->prox;
        elem->prox = NULL; //o ultimo nao tem proximo
        fila->fim->prox = elem; //indica que o atual fim tem um proximo
        fila->fim = elem; //atualiza o fim da fila
    }
    else{
        ant->prox = elem->prox; //o anterior conecta com o seguinte ao que sera movido
        elem->prox = NULL; //o ultimo nao tem proximo
        fila->fim->prox = elem; //indica que o atual fim tem um proximo
        fila->fim = elem; //atualiza o fim da fila
    }
}