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

/** Insere pagina na fila */
void push(Fila*, int);

/** Remove pagina da fila */
int pop(Fila*);

/** Verifica se fila esta vazia */
int estaVazia(Fila*);

/** Move pagina para o final da fila */
void moveFim(Fila*, int);

void printFila(Fila*);