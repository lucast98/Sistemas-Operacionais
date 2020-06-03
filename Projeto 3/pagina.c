#include <stdlib.h>
#include <stdio.h>
//#include "processo.h"
//#include "memoria.h"
#include "pagina.h"

/** Cria uma nova tabela de paginas */
tabelaPagina* iniciaTabela(int tam){
    tabelaPagina *tabPag = (tabelaPagina*) malloc(sizeof(tabelaPagina));
    tabPag->tam = tam;
    tabPag->paginas = (item_tabelaPagina*) calloc(tam, sizeof(item_tabelaPagina));
    for(int i = 0; i < tam; i++){
        tabPag->paginas[i].quadro = -1;
    }
    return tabPag;
}

/** Insere elemento na tabela de paginas */
void insereTabela(tabelaPagina *tabPag, Swap swap, bit_pres_aus bpa, int pagina, int quadro){
    tabPag->paginas[pagina].swap = swap;
    tabPag->paginas[pagina].bpa = bpa;
    tabPag->paginas[pagina].quadro = quadro;
}

/** Remove elemento na tabela de paginas */
void removeTabela(tabelaPagina *tabPag, int pagina){
    tabPag->paginas[pagina].swap = 0;
    tabPag->paginas[pagina].bpa = 0;
    tabPag->paginas[pagina].quadro = -1;
}