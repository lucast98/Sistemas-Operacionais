#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include "processo.h"

/** Funcao para criar um novo processo */
Processo* criaProcesso(int pid, Memoria *memPrincipal, int qtdPag, int tamProcesso, int tamPag){
    int pag, quadro;
    Processo *p = (Processo*) malloc(sizeof(Processo));
    if(p == NULL)
        return NULL;
    p->PID = pid;
    p->pagsUsadas = 0; //processo ainda nao utiliza nenhuma pagina
    p->swap = not_swaped;
    p->tabPag = iniciaTabela(qtdPag); //cria tabela de paginas do processo
    
    //Cria fila para LRU
    p->filaPags = criaFila();

    pag=0;
    //printf("Principal: %d - %d\n", memPrincipal->qtd_quadrosLivres, memPrincipal->tam);
    while(tamProcesso > 0){
        quadro = insereQuadro(memPrincipal, p->PID, pag); //insere um novo quadro na memoria fisica
        insereTabela(p->tabPag, p->swap, presente, pag, quadro); //insere um novo elemento na tabela de paginas e o relaciona com o quadro recem-criado
        push(p->filaPags, pag); //insere paginas na fila
        pag++;
        tamProcesso -= tamPag;
    }
    p->pagsUsadas += pag;

    //printf("TamProcesso: %d\n", tamProcesso);
    /*printf("Processo %d\n", pid);
    for(int i = 0; i < pag; i++){
        printf("Pag %d - Quadro %d - Nro %d\n", i , p->tabPag->paginas[i].quadro, memPrincipal->quadros[p->tabPag->paginas[i].quadro].numPag);
    }*/

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

/** Lê o endereco logico e o elemento que esta no quadro associado a ele */
void lerEndereco(Processo *p, Memoria *memPrincipal, int endereco, int tamPag){
    int pag = endereco / tamPag;
    if(endereco % tamPag != 0)
        pag++;

    if(p->tabPag->paginas[pag].bpa == presente){
        printf("Processo %d acessou pagina %d no quadro %d e leu %d.\n", p->PID, pag, p->tabPag->paginas[pag].quadro, memPrincipal->quadros[p->tabPag->paginas[pag].quadro].elemento);
        moveFim(p->filaPags, pag); //move pagina para o final da fila
    }
    else{
        printf("Leitura: Processo %d tentou acessar pagina %d e ocorreu page fault.\n", p->PID, pag);
    }
}

/** Escreve em um quadro que está associado a um endereco logico */
void escreverEndereco(Processo *p, Memoria *memPrincipal, int endereco, int tamPag, int memLivre){
    int quadro, var;
    
    int pag = endereco / tamPag;
    if(endereco % tamPag != 0)
        pag++;

    if(pag > p->pagsUsadas){
        printf("Escrita: Nao foi possivel escrever nesse endereco.\n");
        return;
    }

    //printf("\nmemLivre: %d\n\n", memLivre);
    //if(p->tabPag->paginas[pag].bpa == presente){
    if(memLivre > 0){
        srand(time(0));
        var = rand() % 11; //numero aleatorio entre 0 e 10
        
        atualizaQuadro(memPrincipal, pag, var);
        printf("Processo %d acessou pagina %d no quadro %d e escreveu %d.\n", p->PID, pag, p->tabPag->paginas[pag].quadro, memPrincipal->quadros[pag].elemento);
        moveFim(p->filaPags, pag); //move pagina para o final da fila
    }
    else{
        //printf("algoritmo de substituicao\n");
        trocaPaginaLRU(p, memPrincipal, pag);
    }
}

/** Troca uma nova pagina por uma antiga com o LRU */
int trocaPaginaLRU(Processo *p, Memoria *memPrincipal, int pag){
    int quadro, pagRemovida, quadroRemovido;

    quadro = p->tabPag->paginas[pag].quadro; //encontra a pagina na memoria principal

    pagRemovida = pop(p->filaPags); //pega a pagina utilizada menos recentemente
    quadroRemovido = p->tabPag->paginas[pagRemovida].quadro; //encontra essa pagina na memoria principal

    memPrincipal->quadros[quadroRemovido].PID = p->PID;
    memPrincipal->quadros[quadroRemovido].numPag = pag;
    memPrincipal->quadros[quadroRemovido].elemento = memPrincipal->quadros[quadro].elemento;
    insereTabela(p->tabPag, not_swaped, presente, pag, quadroRemovido); //insere o registro da nova pagina na memoria principal
    insereTabela(p->tabPag, not_swaped, ausente, pagRemovida, quadro); //altera o registro da pagina antiga na tabela
    push(p->filaPags, pag); //insere nova pagina no fim da fila

    printf("Pagina %d foi trocada pela %d.\n", quadroRemovido, quadro);
    return quadroRemovido;
}

/** Indica instrução a ser executada pela CPU */
void operacaoCPU(Processo *p, Memoria *memPrincipal, int endereco, int tamPag){

}

/** Indica instrução a ser executada pelo IO */
void operacaoIO(Processo *p, Memoria *memPrincipal, int instrucao, int tamPag){
    
}