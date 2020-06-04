#include <stdlib.h>
#include <stdio.h>
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
void inserePagina(tabelaPagina *tabPag, bit_pres_aus bpa, int pagina, int quadro){
    tabPag->paginas[pagina].bpa = bpa;
    tabPag->paginas[pagina].quadro = quadro;
}

/** Remove elemento na tabela de paginas */
void removePagina(tabelaPagina *tabPag, int pagina){
    tabPag->paginas[pagina].bpa = 0;
    tabPag->paginas[pagina].quadro = -1;
}

/** Printa tabela de paginas */
void printTabela(tabelaPagina *tabPag){
    int i = 0;
    while(i < tabPag->tam){
        if(tabPag->paginas[i].bpa == presente)
            printf("%d ",tabPag->paginas[i].quadro);
        i++;
    }
    printf("\n");
}