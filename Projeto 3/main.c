#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>
//#include "pagina.h"
//#include "memoria.h"
#include "processo.h"

#define QTD_PROCESSOS 10

/** Executar com ./nome_exec arquivoSimular.txt */

char* decTobin(int n){
    int i, j;
    int a[20];
    char* bin;

    for(i=0;n>0;i++){
        a[i]=n%2;
        n=n/2;    
    }
    if(i==0)//caso especial
        return "0";

    bin = (char *)calloc(i,sizeof(char));
    j=0;
    for(i=i-1;i>=0;i--){
        bin[j] = a[i] + 48; //transformar numero inteiro em char
        j++;
    }
    return bin;
}

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
    int qtdQuad;
    int maxEnd; //quantidade maxima de enderecos possiveis
    int memLivre; //memoria secundaria livre

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
    printf("Digite o tamanho das paginas e quadros de pagina: ");
    scanf("%d", &page_size);
    printf("Digite o tamanho em bits do endereco logico: ");
    scanf("%d", &logic_size);
    do{
        printf("Digite o tamanho da memoria fisica que deve ser multiplo de tamanho da moldura (quadro): ");
        scanf("%d", &real_size);
        if(real_size % page_size != 0)
            printf("Digite um tamanho de memoria que seja multiplo do quadro.\n");
    }while(real_size % page_size != 0);
    printf("Digite o tamanho maximo da memoria secundaria: ");
    scanf("%d", &sec_size); //tamanho da memoria virtual
    do{
        printf("Digite o algoritmo de substituicao a ser utilizado (L para LRU ou R para relogio): ");
        scanf(" %c", &subs_alg);
        subs_alg = toupper(subs_alg); //deixa sempre maiusculo
        if(subs_alg != 'L' && subs_alg != 'R')
            printf("Digite um algoritmo de substituicao valido.\n");
    }while(subs_alg != 'L' && subs_alg != 'R');
    qtdPag = sec_size/page_size; //unidades de tamanho fixo no dispositivo secundario
    qtdQuad = real_size/page_size; //unidades correspondentes na memoria fisica
    maxEnd = pow(2, logic_size); //endereco maximo permitido (por causa dos bits)

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
        splitString(str, pNumber, &mode, op); //divide linha em tres partes (numero de processo-modo-tamanho/operando)
        //printf("%s - %c - %s", pNumber, mode, op);
        pid = getPID(pNumber);
        //printf("Memoria livre: %d\n", memLivre);
        switch (mode){    
            case 'C':
                // Criar o processo lido antes desse do tamanho especificado logo em seguida em binário
                if(memLivre >= page_size){
                    pro[pid-1] = criaProcesso(pid, memPrincipal, memVirtual, qtdPag, atoi(op), page_size, &memLivre);
                    if(pro[pid-1] == NULL)
                        printf("Erro ao criar processo\n");
                    else{
                        printf("Processo %d criado!\n", pid);
                    }
                }
                else{
                    printf("Nao e possivel criar um processo, pois nao ha espaco na memoria.\n");
                }
                break;
            case 'R':
                // Lê o endereço de memoria especificado logo após
                if(maxEnd > getDec(mode, op)){
                    lerEndereco(pro[pid-1], memPrincipal, memVirtual, getDec(mode, op), page_size, &memLivre, subs_alg);
                }
                else{
                    printf("O endereco logico tem mais bits que o permitido.\n");
                }
                break;
            case 'W':
                // Escrita no endereço especificado logo após
                if(maxEnd > getDec(mode, op))
                    escreverEndereco(pro[pid-1], memPrincipal, memVirtual, getDec(mode, op), page_size, &memLivre, subs_alg);
                else
                    printf("O endereco logico tem mais bits que o permitido.\n");
                break;
            case 'P':
                operacaoCPU(pro[pid-1], memPrincipal, getDec(mode, op), page_size);
                // Indicando instrução a ser executada pela CPU
                break;
            case 'I':
                operacaoIO(pro[pid-1], memPrincipal, getDec(mode, op), page_size);
                // Indicando instrução de I/O
                break;
            default:
                break;
        }
    }
    fclose(fp);

    destroiMemoria(memVirtual);
    destroiMemoria(memPrincipal);

    return 0;
}

/*
    //modelo A
    i=0;
    while(i!=8){
    printf("++--------+--------++--------+--------++--------+--------++--------+--------++\n");
    printf("||  &d  \\  &d  ||  &d  \\  &d  ||  &d  \\  &d  ||  &d  \\  &d  ||\n",p1,e1,p2,e2,p3,e3,p4,e4);
    i++;
    }
    printf("++--------+--------++--------+--------++--------+--------++--------+--------++\n");

    //modelo B
    i=0;
    while(i!=8){
    printf("++--------++--------++--------++--------++--------++--------++--------++--------++\n");
    printf("||  &d  ||  &d  ||  &d  ||  &d  ||  &d  ||  &d  ||  &d  ||  &d  ||\n",p1,p2,p3,p4,p5,p6,p7,p8);
    printf("||--------||--------||--------||--------||--------||--------||--------||--------||\n");
    printf("||  &d  ||  &d  ||  &d  ||  &d  ||  &d  ||  &d  ||  &d  ||  &d  ||\n",e1,e2,e3,e4,e5,e6,e7,e8);
    printf("++--------++--------++--------++--------++--------++--------++--------++--------++\n");
    i++;
    }
*/