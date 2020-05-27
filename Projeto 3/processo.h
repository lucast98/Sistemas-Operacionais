#include "pagina.h"
#include "memoria.h"
#include "fila.h"

typedef struct Processo{
    int PID; //identificador de processo
    tabelaPagina *tabPag; //Tabela de paginas
    int pagsUsadas; //qtd de paginas ocupadas
    Swap swap; //indica se o processo sofreu swap-out
    Fila *filaPags; // Fila de páginas na memória para fazer LRU
} Processo;

/** Funcao para criar um novo processo */
Processo* criaProcesso(int, Memoria*, Memoria*, int);

/** Adiciona pagina na memoria */
int inserePaginaMemoria(Processo*, Memoria*, int, Swap, bit_pres_aus);

/** Lê o endereco referenciado pela tabela do processo */
void lerEndereco(Processo*, Memoria*, Memoria*, int);

/** Escreve o endereco referenciado pela tabela do processo */
void escreverEndereco(Processo*, Memoria*, Memoria*, int);