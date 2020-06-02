#include <stdlib.h>
#include <stdio.h>
#include "memoria.h"

/** Funcao para criar uma nova estrutura de memoria */
Memoria* criaMemoria(int tam, int tamPag){
    int qtdQuad = tam/tamPag;
    Memoria *mem = (Memoria*) malloc(sizeof(Memoria));
    mem->quadros = (quadroMemoria*) calloc(tam, sizeof(quadroMemoria));
    mem->tam = tam;
    mem->quadrosLivres = (int*) malloc(tam*sizeof(int));
    for(int i = 0; i < tam; i++){
        mem->quadrosLivres[i] = 1;
        mem->quadros[i].numPag = -1; //nao tem nada
    }
    mem->qtd_quadrosLivres = qtdQuad;
    return mem;
}

/** Destroi a estrutura de memoria */
void destroiMemoria(Memoria *mem){
    free(mem->quadros);
    free(mem->quadrosLivres);
    free(mem);
}

/** Insere um quadro na memoria */
int insereQuadro(Memoria *mem, int pid, int numPag){
    int i;
    for(i = 0; i < mem->tam; i++){
        if(mem->quadrosLivres[i] == 1)
            break;
    }
    mem->quadros[i].PID = pid;
    mem->quadros[i].numPag = numPag;
    mem->quadros[i].elemento = 0;
    mem->quadrosLivres[i] = 0;
    mem->qtd_quadrosLivres--; //decrementa o numero total de quadros
    
    return i;
}

/** Função para atualizar o elemento do quadro */
void atualizaQuadro(Memoria *mem, int pag, int elem){
    mem->quadros[pag].elemento = elem;
}

/** Remove um quadro da memoria */
void removeQuadro(Memoria *mem, int quadro){
    //retorna o pid e o numPag para o estado inicial
    mem->quadros[quadro].PID = 0;
    mem->quadros[quadro].numPag = -1;
    mem->quadrosLivres[quadro] = 1; //o quadro esta livre
    mem->qtd_quadrosLivres++; //incrementa o numero total de quadros
}