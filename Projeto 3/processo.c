#include <stdlib.h>
#include <stdio.h>
#include "processo.h"

/** Funcao para criar um novo processo */
Processo* criaProcesso(int pid, Memoria *memPrincipal, Memoria *memVirtual, int qtdPag, int tamProcesso, int tamPag){
    int pag, quadro;
    Processo *p = (Processo*) malloc(sizeof(Processo));
    if(p == NULL)
        return NULL;
    p->PID = pid;
    //p->pagsUsadas = 0; //processo ainda nao utiliza nenhuma pagina
    p->swap = not_swaped;
    p->tabPag = iniciaTabela(qtdPag); //cria tabela de paginas do processo
    
    //Cria fila para LRU
    p->filaPags = criaFila();

    pag=0;
    while(tamProcesso > 0){
        quadro = insereQuadro(memPrincipal, p->PID, pag); //insere um novo quadro na memoria fisica
        insereTabela(p->tabPag, p->swap, presente, pag, quadro); //insere um novo elemento na tabela de paginas e o relaciona com o quadro recem-criado
        pag++;
        tamProcesso -= tamPag;
    }
    /*printf("Processo %d\n", pid);
    for(int i = 0; i < pag; i++){
        printf("Pag %d - Quadro %d - Nro %d\n", i , p->tabPag->paginas[i].quadro, memPrincipal->quadros[i].numPag);
    }*/

    /*//Coloca primeira pagina na memoria principal
    inserePaginaMemoria(p, memPrincipal, 0, not_swaped, presente);

    //Coloca outras paginas na memoria virtual
    printf("Tam: %d\n", p->tabPag->tam);
    for(int i = 1; i < p->tabPag->tam; i++)
        inserePaginaMemoria(p, memVirtual, i, not_swaped, ausente);   
    */
    return p;
}

/** Adiciona pagina na memoria */
int inserePaginaMemoria(Processo *p, Memoria *mem, int pagina, Swap swap, bit_pres_aus bpa){
    int quadro = insereQuadro(mem, p->PID, pagina); //insere um novo quadro na memoria fisica
    insereTabela(p->tabPag, swap, bpa, pagina, quadro); //insere um novo elemento na tabela de paginas e o relaciona com o quadro recem-criado
    if(bpa == presente)
        push(p->filaPags, pagina); //insere na fila do LRU
    return quadro;
}

void lerEndereco(Processo *p, int endereco, int tamPag){
    int pag = endereco / tamPag;
    if(endereco % tamPag != 0)
        pag++;

    if(p->tabPag->paginas[pag].bpa == presente){
        printf("Processo %d acessou pagina %d no quadro %d.\n", p->PID, pag, p->tabPag->paginas[pag].quadro);
        moveFim(p->filaPags, pag); //move pagina para o final da fila
    }
    else{
        printf("Processo %d tentou acessar pagina %d e ocorreu page fault.\n", p->PID, pag);
    }
}

void escreverEndereco(Processo *p, int endereco, int tamPag){
    int pag = endereco / tamPag;
    if(endereco % tamPag != 0)
        pag++;
    
    if(p->pagsUsadas < p->tabPag->tam){
        insereTabela(p->tabPag, not_swaped, presente, p->pagsUsadas, p->tabPag->paginas[pag].quadro);
        printf("Processo %d escreveu na pagina %d no quadro %d.\n", p->PID, p->tabPag->paginas[pag].quadro);
        p->pagsUsadas++;
    }
    else{
        printf("aplicar algoritmo de substituicao\n");
    }
}
