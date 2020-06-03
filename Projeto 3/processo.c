#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include "processo.h"

/** Funcao para criar um novo processo */
Processo* criaProcesso(int pid, Memoria *memPrincipal, Memoria *memVirtual, int qtdPag, int tamProcesso, int tamPag, int *memLivre){
    int pag, quadro, pagina;
    Processo *p = (Processo*) malloc(sizeof(Processo));
    if(p == NULL)
        return NULL;
    p->tam = tamProcesso;
    p->PID = pid;
    p->pagsUsadas = 0; //processo ainda nao utiliza nenhuma pagina
    p->swap = not_swaped;
    p->tabPag = iniciaTabela(qtdPag); //cria tabela de paginas do processo
    p->filaPags = criaFila(); //cria fila para LRU

    pag=0;
    //printf("Principal: %d - %d\n", memPrincipal->qtd_quadrosLivres, memPrincipal->tam);
    while(tamProcesso > 0){
        if(pag == 0){ //coloca primeira pagina na memoria principal
            quadro = insereQuadro(memPrincipal, p->PID, pag); //insere um novo quadro na memoria fisica
            insereTabela(p->tabPag, not_swaped, presente, pag, quadro); //insere um novo elemento na tabela de paginas e o relaciona com o quadro recem-criado
            push(p->filaPags, pag); //insere paginas na fila
            *memLivre -= tamPag; //diminui quantidade de memoria secundaria livre devido ao processo criado ter colocado sua primeira pagina na principal
        }
        else{ //coloca demais paginas na memoria virtual
            pagina = insereQuadro(memVirtual, p->PID, pag); //insere uma nova pagina na memoria virtual
            insereTabela(p->tabPag, not_swaped, ausente, pag, pagina+1); //insere um novo elemento na tabela de paginas
        }
        pag++;
        tamProcesso -= tamPag;
    }
    p->pagsUsadas += pag;

    return p;
}

/** Lê o endereco logico e o elemento que esta no quadro associado a ele */
void lerEndereco(Processo *p, Memoria *memPrincipal, Memoria *memVirtual, int endereco, int tamPag, int *memLivre, char alg){
    int pag = endereco / tamPag;
    
    if(p == NULL){
        printf("Processo nao existe.\n");
        return;
    }
    if(endereco > p->tam){
        printf("Processo tentou ler em pagina que estava fora da memoria.\n");
        return;
    }

    if(p->tabPag->paginas[pag].bpa == presente){
        printf("Processo %d acessou pagina %d no quadro %d e leu %d.\n", p->PID, pag, p->tabPag->paginas[pag].quadro, memPrincipal->quadros[p->tabPag->paginas[pag].quadro].elemento);
        moveFim(p->filaPags, pag); //move pagina para o final da fila
    }
    else{
        if(*memLivre > 0){
            int pagina = p->tabPag->paginas[pag].quadro;
            int quadro = insereQuadro(memPrincipal, p->PID, pagina); //insere um novo quadro na memoria fisica
            removePagina(p->tabPag, pag); //remove pagina da tabela
            insereTabela(p->tabPag, not_swaped, presente, pag, quadro); //insere um novo elemento na tabela de paginas e o relaciona com o quadro recem-criado
            push(p->filaPags, pag); //insere nova pagina na fila
            printf("Processo %d acessou pagina %d no quadro %d e leu %d.\n", p->PID, pag, p->tabPag->paginas[pag].quadro, memPrincipal->quadros[p->tabPag->paginas[pag].quadro].elemento);
            moveFim(p->filaPags, pag); //move pagina para o final da fila
            *memLivre -= tamPag;
        }
        else{ //aplica o algoritmo de substituicao
            if(alg == 'L'){ //usa o LRU
                trocaPaginaLRU(p, memPrincipal, memVirtual, pag);
            }
            else{ //usa o ...

            }
            printf("Processo %d acessou pagina %d no quadro %d e leu %d.\n", p->PID, pag, p->tabPag->paginas[pag].quadro, memPrincipal->quadros[p->tabPag->paginas[pag].quadro].elemento);
        }
    }
        
}

/** Escreve em um quadro que está associado a um endereco logico */
void escreverEndereco(Processo *p, Memoria *memPrincipal, Memoria *memVirtual, int endereco, int tamPag, int *memLivre, char alg){
    int quadro, var;
    int pag = endereco / tamPag;
    
    if(p == NULL){
        printf("Processo nao existe.\n");
        return;
    }
    if(endereco > p->tam){
        printf("Processo tentou escrever em pagina que estava fora da memoria.\n");
        return;
    }

    srand(time(0));
    var = rand() % 11; //numero aleatorio entre 0 e 10

    if(p->tabPag->paginas[pag].bpa == presente){ //verifica se a pagina esta na memoria principal
        atualizaQuadro(memPrincipal, pag, var);
        printf("Processo %d acessou pagina %d no quadro %d e escreveu %d.\n", p->PID, pag, p->tabPag->paginas[pag].quadro, memPrincipal->quadros[pag].elemento);
        moveFim(p->filaPags, pag); //move pagina para o final da fila
    }
    else{
        if(*memLivre > 0){ //se possui memoria livre, nao precisa usar um algoritmo de substituicao
            int pagina = p->tabPag->paginas[pag].quadro;
            int quadro = insereQuadro(memPrincipal, p->PID, pagina); //insere um novo quadro na memoria fisica
            removePagina(p->tabPag, pag); //remove pagina da tabela
            insereTabela(p->tabPag, not_swaped, presente, pag, quadro); //insere um novo elemento na tabela de paginas e o relaciona com o quadro recem-criado
            push(p->filaPags, pag); //insere nova pagina na fila
            
            atualizaQuadro(memPrincipal, quadro, var);
            printf("Processo %d acessou pagina %d no quadro %d e escreveu %d.\n", p->PID, pag, p->tabPag->paginas[pag].quadro, memPrincipal->quadros[quadro].elemento);
            moveFim(p->filaPags, pag); //move pagina para o final da fila
            *memLivre -= tamPag;
        }
        else{ //aplica algoritmo de substituicao
            if(alg == 'L'){ //usará o LRU
                //printFila(p->filaPags);
                trocaPaginaLRU(p, memPrincipal, memVirtual, pag);
                //printFila(p->filaPags);
            }
            else{//usará o ...

            }
            atualizaQuadro(memPrincipal, pag, var);
            printf("Processo %d acessou pagina %d no quadro %d e escreveu %d.\n", p->PID, pag, p->tabPag->paginas[pag].quadro, memPrincipal->quadros[pag].elemento);
        }
    }
}

/** Troca uma nova pagina por uma antiga com o LRU */
void trocaPaginaLRU(Processo *p, Memoria *memPrincipal, Memoria *memVirtual, int pag){
    int quadro, pagRemovida, novoQuadro, elemento;
    //printf("pag: %d\n", pag);
    quadro = p->tabPag->paginas[pag].quadro; //encontra a nova pagina na memoria principal

    pagRemovida = pop(p->filaPags); //pega a pagina utilizada menos recentemente
    novoQuadro = p->tabPag->paginas[pagRemovida].quadro; //encontra essa pagina na memoria principal

    //insere o registro da nova pagina na memoria principal
    memPrincipal->quadros[novoQuadro].PID = p->PID;
    memPrincipal->quadros[novoQuadro].numPag = pag;
    memPrincipal->quadros[novoQuadro].elemento = memPrincipal->quadros[quadro].elemento;
    insereTabela(p->tabPag, not_swaped, presente, pag, novoQuadro); 

    //insere pagina antiga na memoria virtual
    memVirtual->quadros[quadro].PID = p->PID;
    memVirtual->quadros[quadro].numPag = pagRemovida;
    memVirtual->quadros[quadro].elemento = memPrincipal->quadros[pagRemovida].elemento;
    insereTabela(p->tabPag, not_swaped, ausente, pagRemovida, quadro); //altera o registro da pagina antiga na tabela
    push(p->filaPags, pag); //insere nova pagina no fim da fila

    printf("LRU: Pagina %d foi trocada pela %d.\n", pagRemovida, pag);
}

/** Indica instrução a ser executada pela CPU */
void operacaoCPU(Processo *p, Memoria *memPrincipal, int endereco, int tamPag){

}

/** Indica instrução a ser executada pelo IO */
void operacaoIO(Processo *p, Memoria *memPrincipal, int instrucao, int tamPag){
    
}