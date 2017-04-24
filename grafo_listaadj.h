/************************************
	ACH2024 - Algoritmos e Estruturas de Dados II
 
	Implementacao de Grafos utilizando Listas de Adjacencia
	(vetor de ponteiros no qual cada posicao indexa um vertice e
 contem o ponteiro para a cabeca de sua lista de adjacencia)
 
 *************************************/

#include <stdbool.h>   /* variaveis bool assumem valores "true" ou "false" */

typedef float TipoPeso;

/*
 tipo estruturado taresta:
 vertice destino, peso, ponteiro p/ prox. aresta
 */
typedef struct taresta {
    int vdest;
    TipoPeso peso;
    struct taresta * prox;
} TipoAresta;

typedef TipoAresta* TipoApontador;

/*
 tipo estruturado grafo:
 vetor de listas de adjacencia (cada posicao contem o ponteiro
 para o inicio da lista de adjacencia do vertice)
 numero de vertices
 */
typedef struct {
    TipoApontador *listaAdj;
    int numVertices;
    int numArestas;
    int direcionado;
    int componentesConexos;
    int **listaConexo;
} TipoGrafo;

/********************
 
 Prototipos dos metodos sobre grafos
 
 *********************/

/*
 inicializaGrafo(TipoGrafo* grafo, int nv): Cria um grafo com n vertices.
 Aloca espaco para o vetor de apontadores de listas de adjacencias e,
 para cada vertice, inicializa o apontador de sua lista de adjacencia.
 Retorna true se inicializou com sucesso e false caso contrario.
 Vertices vao de 0 a nv-1.
 */
bool inicializaGrafo(TipoGrafo *grafo, int nv);


/*
 void insereAresta(int v1, int v2, TipoPeso peso, TipoGrafo *grafo):
 Insere a aresta (v1, v2) com peso "peso" no grafo.
 Nao verifica se a aresta ja existe.
 */
void insereAresta(int v1, int v2, TipoPeso peso, TipoGrafo *grafo);


/*
 bool existeAresta(int v1, int v2, TipoGrafo *grafo):
 Retorna true se existe a aresta (v1, v2) no grafo e false caso contrário
 */
bool existeAresta(int v1, int v2, TipoGrafo *grafo);

/*
 bool removeAresta(int v1, int v2, TipoPeso* peso, TipoGrafo *grafo);
 Remove a aresta (v1, v2) do grafo.
 Se a aresta existia, coloca o peso dessa aresta em "peso" e retorna true,
 caso contrario retorna false (e "peso" é inalterado).
 */
bool removeAresta(int v1, int v2,int * pesoDosRemovidos, TipoGrafo *grafo , int * verticesRemovidos);

/*
 bool listaAdjVazia(int v, TipoGrafo* grafo):
 Retorna true se a lista de adjacencia (de vertices adjacentes) do vertice v é vazia, e false caso contrário.
 */
bool listaAdjVazia(int v, TipoGrafo *grafo);

bool articulacao(TipoGrafo * grafo , int verticeInicial, int * verticesRemovidos, int *pesoDosRemovidos);
void removeVertice(TipoGrafo * grafo , int vertice, int * verticesRemovidos , int * pesoDosRemovidos , int * i);
/*
 TipoApontador primeiroListaAdj(int v, TipoGrafo* grafo):
 Retorna o endereco do primeiro vertice da lista de adjacencia de v
 ou NULL se a lista de adjacencia estiver vazia.
 */
TipoApontador primeiroListaAdj(int v, TipoGrafo *grafo);

void componentesConexos(TipoGrafo *grafo, int verticeInicial , bool dfsPath , bool articulacao);
/*
 TipoApontador proxListaAdj(int v, TipoGrafo* grafo):
 Retorna o proximo vertice adjacente a v, partindo do vertice "prox" adjacente a v
 ou NULL se a lista de adjacencia tiver terminado sem um novo proximo.
 */
TipoApontador proxListaAdj(int v, TipoGrafo *grafo, TipoApontador prox);


/*
 void imprimeGrafo(TipoGrafo* grafo):
 Imprime os vertices e arestas do grafo no seguinte formato:
 v1: (adj11, peso11); (adj12, peso12); ...
 v2: (adj21, peso21); (adj22, peso22); ...
 Assuma que cada vértice é um inteiro de até 2 dígitos.
 */
void imprimeGrafo(TipoGrafo *grafo);

/*
 void liberaGrafo (TipoGrafo *grafo): Libera o espaco ocupado por um grafo.
 */
void liberaGrafo (TipoGrafo *grafo);

void adicionaComponente(int * caminho ,  int numeroVertices , int * componente , int * resposta , int * iterador);

/*
 LeGrafo(nomearq, Grafo)
 Le o arquivo nomearq e armazena na estrutura Grafo
 Lay-out:
 A 1a linha deve conter o número de vertices e o numero de arestas do grafo,
 separados por espaço.
 A 2a linha em diante deve conter a informacao de cada aresta, que consiste
 no indice do vertice de origem, indice do vertice de destino e o peso da
 aresta, tambem separados por espacos.
 Observações:
 Os vertices devem ser indexados de 0 a |V|-1
 Os pesos das arestas sao numeros racionais nao negativos.
 
 Exemplo: O arquivo abaixo contem um grafo com 4 verticennnns (0,1,2,3) e
 7 arestas.
 
 4 7
 0 3 6.3
 2 1 5.0
 2 0 9
 1 3 1.7
 0 1 9
 3 1 5.6
 0 2 7.2
 
 Codigo de saida:
 1: leitura bem sucedida
 0: erro na leitura do arquivo
 */
int leGrafo(char* nomearq, TipoGrafo *grafo);

void visitaBP(int v, TipoGrafo *grafo, int *tempo,  int *cor,  int *tdesc,  int *tterm,  int * antecessor, int *caminho , int *i , bool dfsPath);


void buscaProfundidade(TipoGrafo *grafo, int verticeInicial, bool dfsPath);

void imprimeCaminho(int * caminho, int u);



/*
 fimListaAdj(v, p, Grafo): indica se o ponteiro atual p  chegou ao
 fim da lista de adjacencia de v (p == NULL).
 */
bool fimListaAdj(int v, TipoApontador p, TipoGrafo *grafo);

/*
 recuperaAdj(v, p, u, peso, grafo): dado um vertice v e um
 ponteiro para uma aresta da lista de adjacencia de v,
 devolve nas variaveis "peso" e "u" respectivamente o peso
 da aresta e o numero do vertice adjacente
 */
void recuperaAdj(int v, TipoApontador p, int *u, TipoPeso *peso,
                 TipoGrafo *grafo);

/*
 removePrimAresta(v, Grafo): retira a aresta do 1o vertice adjacente a v e libera seu espaco
 */
//void removePrimAresta(int v, TipoGrafo *grafo);



void dfsR( TipoGrafo *G, int v, int *pre, int *conta);

/********************************************************************************/

/*
 TipoApontador existeERetornaAresta(int v1, int v2, TipoGrafo *grafo):
 Retorna um apontador para a aresta (v1,v2) se ela existir e NULL caso Contrario.
 */
TipoApontador existeERetornaAresta(int v1, int v2, TipoGrafo *grafo);


