#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include "Cabecalho.h"

/** Executar com ./nome_exec arquivoSimular.txt */


void splitString(char *str, char *pNumber, char *mode, char *op){
    char *split;
    split = strtok(str, " "); //pega o numero do processo do arquivo
    strcpy(pNumber, split);
    split = strtok(NULL, " ");
    *mode = *split;
    split = strtok(NULL, " ");
    strcpy(op, split);
}

int main(int argc, char const *argv[]){
    // abrir o arquivo 
    // ler a primeira linha
    // esperado: P1 C (1024)2 ou ##### início do arquivo ######
    
    //startPage(&pag[0]);
    
    
    FILE *fp;
    char str[60]; //armazena a linha do arquivo
    char pNumber[5]; //numero do processo
    char mode; //tipo de operação
    char op[10]; //tamanho/operando/dispositivo
    const char *fileName = argv[1]; //nome do arquivo de entrada
    
    /** Memoria logica */
    int p; //numero de pagina: usado como indice de uma tabela de paginas
    int d; //deslocamento de pagina

    /** Variaveis de configuracao de mecanismos associados à memoria virtual */
    int page_size; //representa o tamanho das paginas e quadros de paginas
    int logic_size; //representa o tamanho em bits do endereço logico
    int real_size; //representa o tamanho da memoria fisica
    int sec_size; //representa o tamanho maximo da memoria secundaria
    int img_size; //representa o tamanho da imagem de cada processo
    char subs_alg; //representa o algoritmo de substituicao a ser utilizado

    /** Abre o arquivo e verifica se está OK */
    fp = fopen(fileName, "r");
    if(fp == NULL){
        printf("Erro\n");
        return -1;
    }

    /** Opções de configuração de mecanismos associados à memória virtual */
    printf("Digite o tamanho das páginas e quadros de página: ");
    scanf("%d", &page_size);
    printf("Digite o tamanho em bits do endereço lógico: ");
    scanf("%d", &logic_size);
    printf("Digite o tamanho da memória física que deve ser múltiplo de tamanho da moldura (quadro): ");
    scanf("%d", &real_size);
    printf("Digite o tamanho máximo da memória secundária: ");
    scanf("%d", &sec_size); 
    printf("Digite o tamanho da imagem de cada processo a ser executado: ");
    scanf("%d", &img_size);
    printf("Digite o algoritmo de substituicao a ser utilizado (L para LRU ou R para relogio): ");
    scanf(" %c", &subs_alg);
    subs_alg = toupper(subs_alg); //deixa sempre maisculo

    printf("Iniciando simulador de gerenciamento de memoria virtual.\n");
    printf("Arquivo de entrada: %s\n", fileName);

    /** Parte em que o arquivo será lido e realizará a simulação */
    while(fgets(str, 60, fp) != NULL) {
        /* lendo arquivo */
        if(str[0] == '#')
            continue; //evita comentario
        splitString(str, pNumber, &mode, op); //divide linha em tres partes (numero de processo-modo-tamanho/operando)
        //printf("%s - %c - %s", pNumber, mode, op);
        switch (mode){
            case 'C':
                // Criar o processo lido antes desse do tamanho especificado logo em seguida em binário
                break;
            case 'R':
                // Lê o endereço de memoria especificado logo após
                break;
            case 'W':
                // Escrita no endereço especificado logo após
                break;
            case 'P':
                // Indicando instrução a ser executada pela CPU
                break;
            case 'I':
                // Indicando instrução de I/O
                break;
            default:
                break;
        }
    }
    fclose(fp);

    return 0;
}
