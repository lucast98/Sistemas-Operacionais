/** bit presente/ausente */
typedef enum pres_aus{
    ausente, presente
} bit_pres_aus;

typedef struct item_tabelaPagina{
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
void inserePagina(tabelaPagina*, bit_pres_aus, int, int);

/** Remove elemento na tabela de paginas */
void removePagina(tabelaPagina*, int);