#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
#include "processo.h"

/** Funcao para criar um novo processo */
Processo* criaProcesso(int pid, Memoria *memPrincipal, Memoria *memVirtual, int qtdPag, int tamProcesso, int tamPag, int *memLivre, char alg){
    int pag, quadro;
    Processo *p = (Processo*) malloc(sizeof(Processo));
    if(p == NULL || tamProcesso <= 0){
        printf("Erro ao criar processo\n");
        return NULL;
    }
    p->tam = tamProcesso;
    p->PID = pid;
    p->pagsUsadas = 0; //processo ainda nao utiliza nenhuma pagina
    p->tabPag = iniciaTabela(tamProcesso/tamPag); //cria tabela de paginas do processo
    p->filaPags = criaFila(); //cria fila para LRU e FIFO

    pag = 0;
    while(tamProcesso > 0){
        if(pag == 0){ //coloca primeira pagina na memoria principal
            quadro = insereQuadro(memPrincipal, p->PID, pag); //insere um novo quadro na memoria fisica
            inserePagina(p->tabPag, presente, pag, quadro); //insere um novo elemento na tabela de paginas e o relaciona com o quadro recem-criado
            push(p->filaPags, pag); //insere paginas na fila
            
            *memLivre -= tamPag; //diminui quantidade de memoria secundaria livre devido ao processo criado ter colocado sua primeira pagina na principal
        }
        else{ //coloca demais paginas na memoria virtual
            quadro = insereQuadro(memVirtual, p->PID, pag); //insere uma nova pagina na memoria virtual
            inserePagina(p->tabPag, ausente, pag, quadro+1); //insere um novo elemento na tabela de paginas
        }
        pag++;
        tamProcesso -= tamPag;
    }
    p->pagsUsadas += pag;
    printf("Processo %d criado!\n", pid);
    printMemoria(memPrincipal, memVirtual, tamPag);
    printProcesso(p, alg);
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
        printMemoria(memPrincipal, memVirtual, tamPag);
        printProcesso(p, alg);
        return;
    }

    if(p->tabPag->paginas[pag].bpa == presente){
        printf("Processo %d acessou pagina %d no quadro %d e leu %d.\n", p->PID, pag, p->tabPag->paginas[pag].quadro, memPrincipal->quadros[p->tabPag->paginas[pag].quadro].elemento);
        if(alg == 'L')
            moveFim(p->filaPags, pag); //move pagina para o final da fila
    }
    else{
        if(*memLivre > 0){
            int quadro = insereQuadro(memPrincipal, p->PID, pag); //insere um novo quadro na memoria fisica
            removePagina(p->tabPag, pag); //remove pagina da tabela
            inserePagina(p->tabPag, presente, pag, quadro); //insere um novo elemento na tabela de paginas e o relaciona com o quadro recem-criado
            push(p->filaPags, pag); //insere nova pagina na fila
            printf("Processo %d acessou pagina %d no quadro %d e leu %d.\n", p->PID, pag, p->tabPag->paginas[pag].quadro, memPrincipal->quadros[p->tabPag->paginas[pag].quadro].elemento);
            if(alg == 'L')
                moveFim(p->filaPags, pag); //move pagina para o final da fila
            int pagina = encontraQuadro(memVirtual, p->PID, tamPag, pag);
            removeQuadro(memVirtual, pagina); //remove da memoria virtual
            *memLivre -= tamPag;
        }
        else{ //aplica o algoritmo de substituicao
            trocaPaginaLRU_FIFO(p, memPrincipal, memVirtual, pag, memVirtual->quadros[pag].elemento, alg);
            printf("Processo %d acessou pagina %d no quadro %d e leu %d.\n", p->PID, pag, p->tabPag->paginas[pag].quadro, memPrincipal->quadros[p->tabPag->paginas[pag].quadro].elemento);
        }
    }
    printMemoria(memPrincipal, memVirtual, tamPag);
    printProcesso(p, alg);
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
        printMemoria(memPrincipal, memVirtual, tamPag);
        printProcesso(p, alg);
        return;
    }

    srand(time(0));
    var = (rand() % 9) + 1; //numero aleatorio entre 1 e 9

    if(p->tabPag->paginas[pag].bpa == presente){ //verifica se a pagina esta na memoria principal
        atualizaQuadro(memPrincipal, pag, var);
        printf("Processo %d acessou pagina %d no quadro %d e escreveu %d.\n", p->PID, pag, p->tabPag->paginas[pag].quadro, memPrincipal->quadros[pag].elemento);
        if(alg == 'L')
            moveFim(p->filaPags, pag); //move pagina para o final da fila
    }
    else{
        if(*memLivre > 0){ //se possui memoria livre, nao precisa usar um algoritmo de substituicao
            //int pagina = p->tabPag->paginas[pag].quadro;
            int quadro = insereQuadro(memPrincipal, p->PID, pag); //insere um novo quadro na memoria fisica
            removePagina(p->tabPag, pag); //remove pagina da tabela
            inserePagina(p->tabPag, presente, pag, quadro); //insere um novo elemento na tabela de paginas e o relaciona com o quadro recem-criado
            push(p->filaPags, pag); //insere nova pagina na fila
            atualizaQuadro(memPrincipal, quadro, var);
            int pagina = encontraQuadro(memVirtual, p->PID, tamPag, pag);
            if(pagina == -1)
                return;
            removeQuadro(memVirtual, pagina); //remove da memoria virtual
            printf("Processo %d acessou pagina %d no quadro %d e escreveu %d.\n", p->PID, pag, p->tabPag->paginas[pag].quadro, memPrincipal->quadros[quadro].elemento);
            if(alg == 'L')
                moveFim(p->filaPags, pag); //move pagina para o final da fila
            *memLivre -= tamPag;
        }
        else{ //aplica algoritmo de substituicao
            trocaPaginaLRU_FIFO(p, memPrincipal, memVirtual, pag, var, alg);
            //atualizaQuadro(memPrincipal, pag, var);
            printf("Processo %d acessou pagina %d no quadro %d e escreveu %d.\n", p->PID, pag, p->tabPag->paginas[pag].quadro, var);
        }
    }
    printMemoria(memPrincipal, memVirtual, tamPag);
    printProcesso(p, alg);
}

/** Troca uma pagina antiga por uma nova com o LRU e o FIFO */
void trocaPaginaLRU_FIFO(Processo *p, Memoria *memPrincipal, Memoria *memVirtual, int pag, int var, char alg){
    int quadro, pagRemovida, novoQuadro, elemento;
    quadro = p->tabPag->paginas[pag].quadro; //encontra a nova pagina na memoria

    if(estaVazia(p->filaPags)){
        if(alg == 'L')
            printf("Nao foi possivel aplicar o LRU, pois a fila esta vazia.\n");
        else
            printf("Nao foi possivel aplicar o FIFO, pois a fila esta vazia.\n");        
        return;
    }

    pagRemovida = pop(p->filaPags); //pega a pagina utilizada menos recentemente
    novoQuadro = p->tabPag->paginas[pagRemovida].quadro; //encontra essa pagina na memoria principal
    elemento = memPrincipal->quadros[novoQuadro].elemento;

    //insere o registro da nova pagina na memoria principal
    memPrincipal->quadros[novoQuadro].PID = p->PID;
    memPrincipal->quadros[novoQuadro].numPag = pag;
    memPrincipal->quadros[novoQuadro].elemento = var;
    inserePagina(p->tabPag, presente, pag, novoQuadro); 

    //insere pagina antiga na memoria virtual
    memVirtual->quadros[quadro-1].PID = p->PID;
    memVirtual->quadros[quadro-1].numPag = pagRemovida;
    memVirtual->quadros[quadro-1].elemento = elemento;
    inserePagina(p->tabPag, ausente, pagRemovida, quadro); //altera o registro da pagina antiga na tabela
    push(p->filaPags, pag); //insere nova pagina no fim da fila

    if(alg == 'L')
        printf("LRU: Pagina %d foi trocada pela %d.\n", pagRemovida, pag);
    else
        printf("FIFO: Pagina %d foi trocada pela %d.\n", pagRemovida, pag);    
}

/** Indica instrução a ser executada pela CPU */
void operacaoCPU(Processo *p, Memoria *memPrincipal, Memoria *memVirtual, int instrucao, int tamPag, char alg){
    if(p == NULL){
        printf("Processo nao existe.\n");
        return;
    }
    printf("Processo %d executou a instrucao %d, que foi executada pela CPU.\n", p->PID, instrucao);
    printMemoria(memPrincipal, memVirtual, tamPag);
    printProcesso(p, alg);
}

/** Indica instrução a ser executada pelo IO */
void operacaoIO(Processo *p, Memoria *memPrincipal, Memoria *memVirtual, int instrucao, int tamPag, char alg){
    if(p == NULL){
        printf("Processo nao existe.\n");
        return;
    }
    printf("Processo %d executou a instrucao %d, que e de I/O\n", p->PID, instrucao);
    printMemoria(memPrincipal, memVirtual, tamPag);
    printProcesso(p, alg);
}

/** Printa processo na tela */
void printProcesso(Processo *p, char alg){
    printf("-> Processo %d:\n", p->PID);
    printf("--> Tabela de paginas: ");
    printTabela(p->tabPag);
    printFila(p->filaPags);
}