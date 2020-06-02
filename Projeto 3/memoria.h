/** Struct do quadro de memoria */
typedef struct quadroMemoria{
    int PID; //identificador de processo
    int numPag; //numero de pagina
    int elemento; //elemento da memoria principal
} quadroMemoria;

/** Struct da memoria virtual */
typedef struct memoria{ 
    quadroMemoria *quadros; //quadros de memória
    int tam;
    int *quadrosLivres; //vetor binario com os quadros livres 
    int qtd_quadrosLivres; //qtd de quadros livres
} Memoria;

/** Funcao para criar uma nova estrutura de memoria */
Memoria* criaMemoria(int, int);

/** Destroi a estrutura de memoria */
void destroiMemoria(Memoria*);

/** Insere um quadro na memoria */
int insereQuadro(Memoria*, int, int);

/** Função para atualizar o elemento do quadro */
void atualizaQuadro(Memoria*, int, int);

/** Remove um quadro da memoria */
void removeQuadro(Memoria*, int);