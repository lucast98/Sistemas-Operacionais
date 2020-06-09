#include "pagina.h"
#include "memoria.h"
#include "fila.h"

typedef struct Processo{
    int tam; //tamanho do processo
    int PID; //identificador de processo
    tabelaPagina *tabPag; //Tabela de paginas
    int pagsUsadas; //qtd de paginas ocupadas
    Fila *filaPags; // Fila de páginas na memória para fazer LRU
} Processo;

/** Funcao para criar um novo processo */
Processo* criaProcesso(int, Memoria*, Memoria*, int, int, int, int*, char);

/** Lê o endereco referenciado pela tabela do processo */
void lerEndereco(Processo*, Memoria*, Memoria*, int, int, int*, char);

/** Escreve o endereco referenciado pela tabela do processo */
void escreverEndereco(Processo*, Memoria*, Memoria*, int, int, int*, char);

/** Troca uma pagina antiga por uma nova com o LRU e o FIFO */
void trocaPaginaLRU_FIFO(Processo*, Memoria*, Memoria*, int, int, char);

/** Indica instrução a ser executada pela CPU */
void operacaoCPU(Processo*, Memoria*, Memoria*, int, int, char);
    
/** Indica instrução a ser executada pelo IO */
void operacaoIO(Processo*, Memoria*, Memoria*, int, int, char);

/** Printa processo na tela */
void printProcesso(Processo*, char);