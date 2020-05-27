//#include "processo.h"

/** swaped */
typedef enum Swap{
    not_swaped, swaped
}Swap;

/** bit presente/ausente */
typedef enum pres_aus{
    ausente, presente
} bit_pres_aus;

typedef struct item_tabelaPagina{
    Swap swap;
    bit_pres_aus bpa;
    int quadro;
}item_tabelaPagina;

/** Struct que representa uma tabela de paginas */
typedef struct tabelaPagina{
    int tam; //tamanho da tabela
    item_tabelaPagina *paginas;
}tabelaPagina;

/** Cria uma nova tabela de paginas */
tabelaPagina* iniciaTabela(int);

/** Insere elemento na tabela de paginas */
void insereTabela(tabelaPagina*, Swap, bit_pres_aus, int, int);

/** Remove elemento na tabela de paginas */
void removeTabela(tabelaPagina*, int);