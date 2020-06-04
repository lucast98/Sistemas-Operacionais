typedef struct elemento{
    int pag;
    struct elemento *prox;   
}Elemento;

typedef struct fila{
    int tam;
    Elemento *inicio, *fim;
}Fila;

/** Cria uma nova fila */
Fila *criaFila();

/** Insere pagina no fim da fila */
void push(Fila*, int);

/** Remove pagina da fila (primeiro elemento)*/
int pop(Fila*);

/** Move elemento para o final da fila */
void moveFim(Fila*, int);

/** Printa todos os elementos da fila */
void printFila(Fila*);

/** Verifica se a fila está vazia */
int estaVazia(Fila*);