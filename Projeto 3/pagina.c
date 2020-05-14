/** bit presente/ausente */
typedef enum pres_aus{
    ausente, presente
} bit_pres_aus;

/** bit para indicar se a pagina foi removida por swap */
typedef enum Swaped{
    not_swaped, swaped
}Swaped;

typedef struct item_tabelaPagina{
    bit_pres_aus bpa;
    Swaped bs;
    int quadro;
}item_tabelaPagina;

/** Struct que representa uma tabela de paginas */
typedef struct tabelaPagina{
    int tam; //tamanho da tabela
    item_tabelaPagina *paginas;
}tabelaPagina;

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

/** Destroi tabela de paginas */
void destroiTabela(tabelaPagina *tabPag){
    free(tabPag->paginas);
    free(tabPag);
}

/** Insere elemento na tabela de paginas */
void insereTabela(tabelaPagina *tabPag, bit_pres_aus bpa, Swaped bs, int pagina, int quadro){
    tabPag->paginas[pagina].bpa = bpa;
    tabPag->paginas[pagina].bs = bs;
    tabPag->paginas[pagina].quadro = quadro;
}

/** Remove elemento na tabela de paginas */
void removeTabela(tabelaPagina *tabPag, int pagina){
    tabPag->paginas[pagina].bpa = 0;
    tabPag->paginas[pagina].bs = 0;
    tabPag->paginas[pagina].quadro = -1;
}