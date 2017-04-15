
#include <stdio.h>
#include <stdlib.h>
#include "grafo_listaadj.h"

/*
 inicializaGrafo(TipoGrafo* grafo, int nv): Cria um grafo com n vertices.
 Aloca espaco para o vetor de apontadores de listas de adjacencias e,
 para cada vertice, inicializa o apontador de sua lista de adjacencia.
 Retorna true se inicializou com sucesso e false caso contrario.
 Vertices vao de 0 a nv-1.
 */

bool inicializaGrafo(TipoGrafo *grafo, int nv){
    if(nv <= 0) return false;
    grafo->listaAdj = (TipoApontador*)malloc( nv * sizeof (TipoAresta));
    int v;
    grafo->numArestas = 0;
    grafo->numVertices = nv;
    for (v = 0; v < 7; v++)
        grafo->listaAdj[v] = NULL;
    return true;
}
/*
 void insereAresta(int v1, int v2, TipoPeso peso, TipoGrafo *grafo):
 Insere a aresta (v1, v2) com peso "peso" no grafo.
 Nao verifica se a aresta ja existe.
 */
TipoApontador novaAresta(int v1, TipoApontador prox, float peso){
    TipoApontador novo = (TipoApontador)malloc(sizeof(TipoAresta));
    novo->vdest = v1;
    novo->prox = prox;
    novo->peso = peso;
    return novo;
}

void insereAresta(int v1, int v2, TipoPeso peso, TipoGrafo *grafo,int direcionado){
    //condicoes
    grafo->listaAdj[v1] = novaAresta(v2,grafo->listaAdj[v1],peso);
    if(direcionado == 0)
    grafo->listaAdj[v2] = novaAresta(v1, grafo->listaAdj[v2], peso);
}
/*
 bool existeAresta(int v1, int v2, TipoGrafo *grafo):
 Retorna true se existe a aresta (v1, v2) no grafo e false caso contr·rio
 */
bool existeAresta(int v1, int v2, TipoGrafo *grafo){
    TipoApontador atual;
    atual = grafo->listaAdj[v1];
    while (atual && atual->vdest != v2) {
        atual = atual->prox;
    }
    if(atual == NULL || atual->vdest != v2)
        return false;
    return true;
}

/*
 bool removeAresta(int v1, int v2, TipoPeso* peso, TipoGrafo *grafo);
 Remove a aresta (v1, v2) do grafo.
 Se a aresta existia, coloca o peso dessa aresta em "peso" e retorna true,
 caso contrario retorna false (e "peso" È inalterado).
 */
bool removeAresta(int v1, int v2, TipoPeso* peso, TipoGrafo *grafo);

/*
 bool listaAdjVazia(int v, TipoGrafo* grafo):
 Retorna true se a lista de adjacencia (de vertices adjacentes) do vertice v È vazia, e false caso contr·rio.
 */
bool listaAdjVazia(int v, TipoGrafo *grafo);

/*
 TipoApontador primeiroListaAdj(int v, TipoGrafo* grafo):
 Retorna o endereco do primeiro vertice da lista de adjacencia de v
 ou NULL se a lista de adjacencia estiver vazia.
 */
TipoApontador primeiroListaAdj(int v, TipoGrafo *grafo);

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
 Assuma que cada vÈrtice È um inteiro de atÈ 2 dÌgitos.
 */
void imprimeGrafo(TipoGrafo *grafo){
    TipoApontador atual;
    int i;
    for (i = 0; i < grafo->numVertices - 1; i++) {
        atual = grafo->listaAdj[i];
        printf("v%i:",i);
        while(atual){
        printf("(adj: %i , peso: %f)",atual->vdest,atual->peso);
         atual = atual->prox;
        }
        printf("\n");
    }
}

/*
 void liberaGrafo (TipoGrafo *grafo): Libera o espaco ocupado por um grafo.
 */
void liberaGrafo (TipoGrafo *grafo);


/*
 LeGrafo(nomearq, Grafo)
 Le o arquivo nomearq e armazena na estrutura Grafo
 Lay-out:
 A 1a linha deve conter o n˙mero de vertices e o numero de arestas do grafo,
 separados por espaÁo.
 A 2a linha em diante deve conter a informacao de cada aresta, que consiste
 no indice do vertice de origem, indice do vertice de destino e o peso da
 aresta, tambem separados por espacos.
 ObservaÁıes:
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
int leGrafo(char* nomearq, TipoGrafo *grafo){
        int  x, y;
        float z;
        int ordem , aresta,direcionado;
        
        FILE *arq = fopen(nomearq, "r");
        if(arq == NULL){
            return 0;
        }
    fscanf(arq, "%d",&direcionado);
    grafo->direcionado = direcionado;
    fscanf(arq,"%d %d",&ordem , &aresta);
    ordem++;;
    inicializaGrafo(grafo, ordem);
    grafo->numArestas = aresta;
    
    while(!feof(arq)){
        fscanf(arq,"%i %i %f\n",&x, &y, &z);
        //printf("%d %d %f\n",x, y, z);
        insereAresta(x, y, z, grafo,direcionado);
    }
    fclose(arq);
    return 1;
}
void buscaProfundidade(TipoGrafo *grafo, int verticeInicial){
   /* Aloca vetores cor, tdesc, tterm, antecessor com tamanho grafo->nrVertices tempo ← 0;
    Para cada vertice v
    cor[v] ← branco; tdesc[v] = tterm[v] = 0; antecessor[v] ← -1; Para cada vertice v
    Se cor[v] = branco visitaBP(v, grafo, &tempo, cor, tdesc, tterm, antecessor);
    cor{ 0 = branco, 1= cinza, 2 = preto}
    */
    int cor[grafo->numVertices],tdesc[grafo->numVertices],tterm[grafo->numVertices],tempo = 0;
    int antecessor[grafo->numVertices];
    cor[verticeInicial] = 0;
    tdesc[verticeInicial] = tterm[verticeInicial] = 0;
    antecessor[verticeInicial] = -1;
    if(cor[verticeInicial] == 0)
        visitaBP(verticeInicial, grafo, &tempo, cor, tdesc, tterm, antecessor);
    int j;
    for (j=0; j<grafo->numVertices; j++) {
        if(cor[j] == 0 || cor[j] == 1 || cor[j] == 2){
            printf("vertice : %d ,cor: %d \n", j , cor[j]);
        }
    }
}
void visitaBP(int v, TipoGrafo *grafo, int *tempo,  int *cor,  int *tdesc,  int *tterm,  int * antecessor){
    /*cor[v] ← cinza; tdesc[v] ← ++(*tempo);
     Para cada vertice u da lista de adjacência de v
     Se u é branco
     antecessor[u] ← v;
     visitaBP(u, grafo, &tempo, cor, tdesc, tterm, antecessor);
     tterm ← ++(*tempo);
     cor[v] ← preto;*/
    cor[v] = 1;
    tdesc[v] = ++(*tempo);
    int u; // iterador para proximo
    for (u=0; u<grafo->numVertices; u++) {
        if( existeAresta(v, u, grafo) &&cor[u] == 0){
            antecessor[u] = v;
            visitaBP(u, grafo, tempo, cor, tdesc, tterm, antecessor);
        }
        if(existeAresta(v, u, grafo) && cor[u] == 1) //se o prox ja foi visitado
            cor[u] = 2;
    }
}

/*
 fimListaAdj(v, p, Grafo): indica se o ponteiro atual p  chegou ao
 fim da lista de adjacencia de v (p == NULL).
 */
bool fimListaAdj(int v, TipoApontador p, TipoGrafo *grafo){
    TipoApontador atual;
    atual = grafo->listaAdj[v];
    if(atual != NULL && atual->prox == NULL)
        return true;
    return false;
}

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





/********************************************************************************/

/*
 TipoApontador existeERetornaAresta(int v1, int v2, TipoGrafo *grafo):
 Retorna um apontador para a aresta (v1,v2) se ela existir e NULL caso Contrario.
 */
TipoApontador existeERetornaAresta(int v1, int v2, TipoGrafo *grafo);


