#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "processo.h"

#define QTD_PROCESSOS 100 //quantidade maxima de processos

/** Executar com ./nome_exec arquivoSimular.txt */

/** Limpa o buffer do teclado e verifica se enter foi apertado */
void pressEnter(FILE* in, int *first){
    int c;
    while((c = fgetc(in)) != EOF && c != '\n');
    if(*first == 1){
        *first = 0;
        while((c = fgetc(in)) != EOF && c != '\n');
    }
}

/** Obtem o valor entre parenteses no arquivo */
int getDec(char mode, char *dec){
    int valor = 0;
    int i, j=0;
    if(mode == 'R')
        i=1;
    else
        i=2;
    
    while(dec[i] != ')'){
        i++;
    }
    i--;
    while(i > 0){
        valor += (dec[i]-48)*pow(10, j);
        i--;
        j++;
    }
    return valor;
}

/** Divide as linhas de comandos em cada uma de suas funcoes */
void splitString(char *str, char *pNumber, char *mode, char *op){
    char *split;
    split = strtok(str, " "); //pega o numero do processo do arquivo
    strcpy(pNumber, split);
    split = strtok(NULL, " ");
    *mode = *split;
    split = strtok(NULL, " ");
    strcpy(op, split);
}

/** Obtem o PID a partir do comando */
int getPID(char *pNumber){
    int pid = 0, i, j;
    if(pNumber[0] != 'P')
        return -1; //erro
    i = strlen(pNumber) - 1;
    j = i;
    while(i != 0){
        pid += (pNumber[i] - 48) * pow(10,(j-i));
        i--;
    }
    return pid;
}

int main(int argc, char const *argv[]){
    // abrir o arquivo 
    // ler a primeira linha
    // esperado: P1 C (1024)2 ou ##### início do arquivo ######

    int pid;
    int qtdPag;
    int maxEnd; //quantidade maxima de enderecos possiveis
    int memLivre; //memoria secundaria livre
    int qtdProc; //quantidade atual de processos
    int first = 1; //indica que é o primeiro comando

    FILE *fp;
    char str[60]; //armazena a linha do arquivo
    char pNumber[5]; //numero do processo
    char mode; //tipo de operação
    char op[10]; //tamanho/operando/dispositivo
    const char *fileName = argv[1]; //nome do arquivo de entrada
    
    /** Memoria logica */
    /*int p; //numero de pagina: usado como indice de uma tabela de paginas
    int d; //deslocamento de pagina
    */

    /** Variaveis de configuracao de mecanismos associados à memoria virtual */
    int page_size; //representa o tamanho das paginas e quadros de paginas
    int logic_size; //representa o tamanho em bits do endereço logico
    int real_size; //representa o tamanho da memoria fisica
    int sec_size; //representa o tamanho maximo da memoria secundaria
    char subs_alg; //representa o algoritmo de substituicao a ser utilizado

    /** Abre o arquivo e verifica se está OK */
    fp = fopen(fileName, "r");
    if(fp == NULL){
        printf("Erro\n");
        return -1;
    }

    /** Opções de configuração de mecanismos associados à memória virtual */
    do{
        printf("Digite o tamanho das paginas e quadros de pagina: ");
        scanf("%d", &page_size);
        if(page_size <= 0)
            printf("Digite um tamanho de pagina maior que zero.\n");
    }while(page_size <= 0);

    do{
        printf("Digite o tamanho em bits do endereco logico: ");
        scanf("%d", &logic_size);
        if(logic_size <= 0)
            printf("Digite um tamanho em bits maior que zero.\n");
    }while(logic_size <= 0);
    
    do{
        printf("Digite o tamanho da memoria fisica que deve ser multiplo de tamanho da moldura (quadro): ");
        scanf("%d", &real_size);
        if(real_size <= 0)
            printf("Digite um tamanho de memoria fisica que seja maior que zero.\n");
        else{
            if(real_size % page_size != 0)
                printf("Digite um tamanho de memoria que seja multiplo do quadro.\n");
        }
    }while(real_size % page_size != 0 || real_size <= 0);

    do{
        printf("Digite o tamanho maximo da memoria secundaria: ");
        scanf("%d", &sec_size); //tamanho da memoria virtual
        if(sec_size <= 0)
            printf("Digite um tamanho de memoria secundaria maior que zero.\n");
        else{
            if(sec_size < real_size)
                printf("Recomenda-se que a memoria secundaria seja maior ou igual a principal.\n");
        }
    }while(sec_size < real_size || sec_size <= 0);
    
    do{
        printf("Digite o algoritmo de substituicao a ser utilizado (L para LRU ou F para FIFO): ");
        scanf(" %c", &subs_alg);
        subs_alg = toupper(subs_alg); //deixa sempre maiusculo
        if(subs_alg != 'L' && subs_alg != 'F')
            printf("Digite um algoritmo de substituicao valido.\n");
    }while(subs_alg != 'L' && subs_alg != 'F');

    qtdPag = sec_size/page_size; //unidades de tamanho fixo no dispositivo secundario
    maxEnd = pow(2, logic_size); //endereco maximo permitido (por causa dos bits)
    qtdProc = 0; //nao tem processo nenhum

    /** Cria memoria virtual e principal */
    Memoria *memVirtual = criaMemoria(sec_size, page_size);
    Memoria *memPrincipal = criaMemoria(real_size, page_size);
    memLivre = real_size;

    /** Processos criados */
    Processo **pro = (Processo**) malloc(QTD_PROCESSOS * sizeof(Processo *));

    printf("Iniciando simulador de gerenciamento de memoria virtual.\n");
    printf("Arquivo de entrada: %s\n", fileName);

    /** Parte em que o arquivo será lido e realizará a simulação */
    while(fgets(str, 60, fp) != NULL) {
        /* lendo arquivo */
        if(str[0] == '#')
            continue; //evita comentario
        printf("\n--> Comando: %s", str);
        splitString(str, pNumber, &mode, op); //divide linha em tres partes (numero de processo-modo-tamanho/operando)
        pid = getPID(pNumber);

        switch (mode){    
            case 'C':
                // Criar o processo lido antes desse do tamanho especificado logo em seguida em binário
                if(memLivre >= page_size && atoi(op) <= memLivre){
                    if(qtdProc <= QTD_PROCESSOS){ //verifica se nao ultrapassou a qtd de processo permitida
                        pro[pid-1] = criaProcesso(pid, memPrincipal, memVirtual, qtdPag, atoi(op), page_size, &memLivre, subs_alg);
                        qtdProc++;
                    }
                    else
                        printf("Nao e possivel criar um processo, pois a quantidade foi excedida.\n");                    
                }
                else
                    printf("Nao e possivel criar um processo, pois nao ha espaco na memoria.\n");
                break;
            case 'R':
                // Lê o endereço de memoria especificado logo após
                if(maxEnd > getDec(mode, op))
                    lerEndereco(pro[pid-1], memPrincipal, memVirtual, getDec(mode, op), page_size, &memLivre, subs_alg);
                else
                    printf("O endereco logico tem mais bits que o permitido.\n");
                break;
            case 'W':
                // Escrita no endereço especificado logo após
                if(maxEnd > getDec(mode, op))
                    escreverEndereco(pro[pid-1], memPrincipal, memVirtual, getDec(mode, op), page_size, &memLivre, subs_alg);
                else
                    printf("O endereco logico tem mais bits que o permitido.\n");
                break;
            case 'P':
                // Indicando instrução a ser executada pela CPU
                operacaoCPU(pro[pid-1], memPrincipal, memVirtual, getDec(mode, op), page_size, subs_alg);
                break;
            case 'I':
                // Indicando instrução de I/O
                operacaoIO(pro[pid-1], memPrincipal, memVirtual, getDec(mode, op), page_size, subs_alg);
                break;
            default:
                break;
        }
        printf("Aperte enter para continuar...\n");
        pressEnter(stdin, &first); //limpa o buffer do teclado e verifica se o enter foi apertado
    }
    fclose(fp);

    destroiMemoria(memVirtual);
    destroiMemoria(memPrincipal);

    return 0;
}