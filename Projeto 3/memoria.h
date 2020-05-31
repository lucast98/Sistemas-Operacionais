/** Struct do quadro de memoria */
typedef struct quadroMemoria{
    int PID; //identificador de processo
    int numPag; //numero de pagina
} quadroMemoria;

/** Struct da memoria virtual */
typedef struct memoria{ 
    quadroMemoria *quadros; //quadros de memória
    int tam;
    int *quadrosLivres; //vetor binario com os quadros livres 
    int qtd_quadrosLivres; //qtd de quadros livres
} Memoria;

/** Funcao para criar uma nova estrutura de memoria */
Memoria* criaMemoria(int);

/** Destroi a estrutura de memoria */
void destroiMemoria(Memoria*);

/** Insere um quadro na memoria */
int insereQuadro(Memoria*, int, int);

/** Remove um quadro da memoria */
void removeQuadro(Memoria*, int);