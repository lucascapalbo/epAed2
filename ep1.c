
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
    grafo->listaConexo = (int**)malloc(nv * sizeof(int));
    int v;
    grafo->numArestas = 0;
    grafo->numVertices = nv;
    for (v = 0; v < nv; v++)
        grafo->listaAdj[v] = NULL;
    grafo->listaConexo[v] = NULL;
    grafo->componentesConexos = 0;
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

void insereAresta(int v1, int v2, TipoPeso peso, TipoGrafo *grafo){
    //condicoes
    grafo->listaAdj[v1] = novaAresta(v2,grafo->listaAdj[v1],peso);
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
bool removeAresta(int v1, int v2,int * pesoDosRemovidos, TipoGrafo *grafo){
    TipoApontador apagar , anterior;
    apagar =existeERetornaAresta(v1, v2, grafo,&anterior);
    if(apagar == NULL) return false;
    if(anterior == NULL) grafo->listaAdj[v1] = apagar->prox;
    else anterior->prox = apagar->prox;
    free(apagar);
    return true;
}

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
TipoApontador proxListaAdj(int v, TipoGrafo *grafo, TipoApontador prox){
    TipoApontador atual;
    if(prox == NULL)
        return NULL;
    atual = prox->prox;
    if(atual == NULL) return NULL;
    return atual;
}


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
    int ordem , aresta;
    
    FILE *arq = fopen(nomearq, "r");
    if(arq == NULL){
        return 0;
    }
    fscanf(arq,"%d %d",&ordem , &aresta);
    printf("%d %d \n",ordem,aresta);
    ordem++;;
    inicializaGrafo(grafo, ordem);
    grafo->numArestas = aresta;
    
    while(!feof(arq)){
        fscanf(arq,"%i %i %f\n",&x, &y, &z);
        printf("%d %d %f\n",x, y, z);
        insereAresta(x, y, z, grafo);
        insereAresta(y, x, z, grafo);
    }
    printf("\n");
    fclose(arq);
    return 1;
}
void buscaProfundidade(TipoGrafo *grafo, int verticeInicial , bool dfsPath){
    /* Aloca vetores cor, tdesc, tterm, antecessor com tamanho grafo->nrVertices tempo ← 0;
     Para cada vertice v
     cor[v] ← branco; tdesc[v] = tterm[v] = 0; antecessor[v] ← -1; Para cada vertice v
     Se cor[v] = branco visitaBP(v, grafo, &tempo, cor, tdesc, tterm, antecessor);
     cor{ 0 = branco, 1= cinza, 2 = preto}
     */
    int cor[grafo->numVertices],tdesc[grafo->numVertices],tterm[grafo->numVertices],tempo = 0;
    int antecessor[grafo->numVertices];
    int caminho[grafo->numVertices];
    cor[verticeInicial] = 0;
    int resposta[grafo->numVertices];
    int u;
    for (u = 0; u<grafo->numVertices; u++) {
        cor[u] = 0;
    }
    tdesc[verticeInicial] = tterm[verticeInicial] = 0;
    antecessor[verticeInicial] = -1;
    tempo = 0;
    int i = 0;
    int z = 0;
    for (u = verticeInicial; u < grafo->numVertices; u++){
        // quando ele faz o loop, procura por partes nao conexas, adiciona novo componentes pras partes achadas.
        if(cor[u] == 0){
            int novoComponente[grafo->numVertices];
            caminho[i] = u;
            i++;
            if(dfsPath == true)
                imprimeCaminho(caminho, i);
            visitaBP(u, grafo, &tempo, cor, tdesc, tterm, antecessor, caminho , &i,dfsPath);
            if(grafo->componentesConexos >= 0){
                if(dfsPath == false){
                    grafo->componentesConexos++;
                }
                adicionaComponente(caminho, i , novoComponente , resposta , &z);
                i = 0;
            }
        }
    }        if(dfsPath == false)
        imprimeCaminho(resposta,z);
}

void componentesConexos (TipoGrafo *grafo, int verticeInicial , bool dfsPath , bool articulacao){
    // quando dfsPath == false, ele nao printa o caminho. mesmo codigo do busca profundidade, pois
    // nao dava pra salvar os arrays com cada componente.
    int cor[grafo->numVertices],tdesc[grafo->numVertices],tterm[grafo->numVertices],tempo = 0;
    int antecessor[grafo->numVertices];
    int caminho[grafo->numVertices];
    cor[verticeInicial] = 0;
    int resposta[grafo->numVertices];
    int u;
    for (u = 0; u<grafo->numVertices; u++) {
        cor[u] = 0;
    }
    grafo->componentesConexos =1;
    tdesc[verticeInicial] = tterm[verticeInicial] = 0;
    antecessor[verticeInicial] = -1;
    tempo = 0;
    int i = 0;
    int z = 0;
    for (u = verticeInicial; u < grafo->numVertices; u++){
        // quando ele faz o loop, procura por partes nao conexas, adiciona novo componentes pras partes achadas.
        if(cor[u] == 0){
            int novoComponente[grafo->numVertices];
            caminho[i] = u;
            i++;
            visitaBP(u, grafo, &tempo, cor, tdesc, tterm, antecessor, caminho , &i,dfsPath);
            if(articulacao == false){
                printf("C%i: ",grafo->componentesConexos);
                insertionSort(caminho,i);
                imprimeCaminho(caminho, i);
            } if(grafo->componentesConexos >= 0){
                grafo->componentesConexos++;
                adicionaComponente(caminho, i , novoComponente , resposta , &z);
                i = 0;
            }
        }
    }
}
void buscaEmLargura(TipoGrafo * grafo , int raiz, bool bfsPath){
    int cor[grafo->numVertices], distancia[grafo->numVertices];
    int antecessor[grafo->numVertices];
    int caminho[grafo->numVertices];
    FILADEPRIORIDADE fila = *criarFila(grafo->numVertices);
    cor[raiz] = 0;
    for (int i = 0; i < grafo->numVertices; i++) {
        cor[i] = 0;
        antecessor[i] = -1;
        distancia[i]=0;
    }
    for (int i = 0; i < grafo->numVertices; i++) {
        if(cor[i] == 0)
            visitaLargura(i,grafo, cor, antecessor, distancia ,&fila,caminho,bfsPath);
    }
}
/*
 BuscaEmLargura
 escolha uma raiz s de G
 marque s
 insira s em F
 enquanto F não está vazia faça
 seja v o primeiro vértice de F
 para cada w ∈ listaDeAdjacência de v faça
 se w não está marcado então
 visite aresta entre v e w
 marque w
 insira w em F
 senao se w ∈ F entao
 visite aresta entre v e w
 fim se
 fim para
 retira v de F
 fim enquanto
 */

void visitaLargura(int vertice, TipoGrafo * grafo, int * cor, int * antecessor, int * distancia, PFILA fila , int * caminho , bool bfsPath){
    cor[vertice] = 1;
    distancia[vertice] = 0;
    inserirElemento(fila, vertice, 1);
    TipoApontador anterior;
    int resposta[grafo->numVertices];
    int u = 0;
    int z = 0;
    while (tamanho(fila) != 0) {
        for (int i = vertice; i < grafo->numVertices; i++) {
            if(cor[i] == 0){ //nao esta marcado
                TipoApontador aresta= existeERetornaAresta(vertice, i, grafo, &anterior);
                if(aresta != NULL){ //existeAresta
                    cor[i] = 1;
                    antecessor[i] = vertice;
                    int novoComponente[grafo->numVertices];
                    caminho[u] = i;
                    u++;
                    if(bfsPath == true)
                        imprimeCaminho(caminho, u);
                    visitaLargura(i, grafo, cor, antecessor, distancia, fila, caminho,bfsPath);
                    if(grafo->componentesConexos >= 0){
                        if(bfsPath == false){
                            grafo->componentesConexos++;
                        }
                        adicionaComponente(caminho, u , novoComponente , resposta , &z);
                        u = 0;
                    }
                }
            }
        } removerElemento(fila);
        if(bfsPath == false)
            imprimeCaminho(resposta,z);
    }
}
void insertionSort(int vetorDesordenado[], int tamanhoVetor )
{
    int i, j, valorAtual;
    
    for( j=1; j < tamanhoVetor; j++ )
    {
        valorAtual = vetorDesordenado[j];
        i = j-1;
        
        while(i >= 0 && vetorDesordenado[i] > valorAtual)
        {
            vetorDesordenado[i+1] = vetorDesordenado[i];
            i--;
        }
        
        vetorDesordenado[i+1] = valorAtual;
    }
}

void  adicionaComponente(int* caminho , int numeroVertices , int * componente , int *resposta , int * iterador){
    int i;
    for(i = 0; i < numeroVertices;i++){
        componente[i] = caminho[i];
        resposta[*iterador] = caminho[i];
        caminho[i] = 0;
        (*iterador)++;
    }
}

bool Articulacao ( TipoGrafo * grafo,int v){
    int i,ordem;
    if(v<0 || v>grafo->numVertices){
        printf("\n\tVertice inexistente\n");
        return false;
    }
    ordem = grafo->numVertices;
    TipoGrafo grafo_aux;
    inicializaGrafo(&grafo_aux, ordem);
    
    grafo_aux.componentesConexos = 0;
    for(i = 0; i < grafo->numVertices; i++){
        TipoApontador p = grafo->listaAdj[i];
        while (p != NULL) {
            insereAresta(i, p->vdest, p->peso, &grafo_aux);
            p = p->prox;
        }
    }
    removeVertice(&grafo_aux, v);
    if(v == 1)
        componentesConexos(grafo, 1, false, true);
    if(v == 1){ //primeiro vertice
        componentesConexos(&grafo_aux, 2, false, true);
        grafo_aux.componentesConexos--;
    }else{
        if(v==5)
            componentesConexos(&grafo_aux, 1, false, true);
        else
            componentesConexos(&grafo_aux, 1, false, true);
    }
    if((grafo->componentesConexos) < grafo_aux.componentesConexos)
        return true;
    else return false;
}
void imprimeArticulacao(TipoGrafo* grafo, int verticeInicial){
    int articulado[grafo->numVertices];
    printf("Articulation Vertices: \n");
    for (int aux = verticeInicial; aux < grafo->numVertices; aux++) {
        if(Articulacao(grafo, aux) == true){
            articulado[aux] = aux;
            printf("%i " , articulado[aux]);
        }
    } printf("\n");
}
void removeVertice (TipoGrafo *grafo , int v){
    TipoApontador apagar;
    apagar = grafo->listaAdj[v];
    while (apagar!=NULL) {
        apagar = grafo->listaAdj[v];
        if(apagar!=NULL)
            grafo->listaAdj[v] = apagar->prox;
        free(apagar);
    }
}
void visitaBP(int v, TipoGrafo *grafo, int *tempo,  int *cor,  int *tdesc,  int *tterm,  int * antecessor , int * caminho, int* i, bool dfsPath){
    if(cor[v] == 0)
        cor[v] =1;
    tdesc[v] = ++(*tempo);
    int u;
    TipoApontador atual;
    atual = grafo->listaAdj[v];
    while (atual != NULL) {
        u = atual->vdest;
        atual = proxListaAdj(v, grafo, atual);
        antecessor[u]=v;
        if(cor[u] == 0){
            caminho[*i] = u;
            (*i)++;
            if(dfsPath == true)
                imprimeCaminho(caminho, *i);
            visitaBP(u, grafo, tempo, cor, tdesc, tterm, antecessor, caminho, i, dfsPath);
        } if(cor[u] == 1 || cor[u] == 2){
            u = antecessor[u];
            cor[u] = 2;
            tterm[u] = ++(*tempo);
        }
    }
}

void imprimeCaminho(int*caminho , int u){
    int i;
    for (i = 0; i< u; i++) {
        if(caminho[i] != 0 || caminho[i+1] != 0)
            printf("%d ", caminho[i]);
    } printf("\n");
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
TipoApontador existeERetornaAresta(int v1, int v2, TipoGrafo *grafo, TipoApontador * anterior){
    if(grafo->listaAdj[v1] == NULL) return NULL;
    *anterior = NULL;
    TipoApontador buscado;
    buscado = grafo->listaAdj[v1];
    while (buscado!=NULL) {
        *anterior = buscado;
        if(buscado->vdest == v2) return buscado;
        buscado = buscado->prox;
    }
    return NULL;
}


/********************************************************************************/
//FILA
PFILA criarFila(int max){
    PFILA res = (PFILA) malloc(sizeof(FILADEPRIORIDADE));
    res->maxRegistros = max;
    res->arranjo = (PONT*) malloc(sizeof(PONT)*max);
    int i;
    for (i=0;i<max;i++) res->arranjo[i] = NULL;
    res->fila = NULL;
    return res;
}

bool exibirLog(PFILA f){
    printf("Log [elementos: %i]\n", tamanho(f));
    PONT atual = f->fila;
    while (atual){
        printf("%p[%i;%f;%p]%p ", atual->ant, atual->id, atual->prioridade, atual, atual->prox);
        atual = atual->prox;
    }
    printf("\n\n");
    return 0;
}

int tamanho(PFILA f){
    PONT end = f->fila;
    int tam = 0;
    while (end != NULL){
        tam++;
        end = end->prox;
    }
    return tam;
}

PONT buscaSeq(PFILA f, int id){
    PONT pos = f->fila;
    while (pos != NULL){
        if (pos->id == id) return pos;
        pos = pos->prox;
    }
    return NULL;
}

PONT buscaSeqExc(float prioridade, PFILA f, PONT *ant){ //dado um ponteiro, com uma prioridade,  retorna seu anterior;
    *ant = NULL;
    PONT pos = f->fila;
    while ((pos != NULL) && (pos->prioridade >= prioridade)){
        *ant = pos;
        pos = pos->prox;
    }
    if ((pos != NULL) && (pos->prioridade == prioridade)) return pos;
    return NULL;
} /* buscaSeqExc */

bool inserirElemento(PFILA f, int id, float prioridade){
    if (id < 0 || id >= f->maxRegistros) return false;//id invalido
    PONT ant, i;
    i = buscaSeqExc(prioridade, f, &ant);
    if(i!= NULL) return false;
    i = (PONT) malloc(sizeof(REGISTRO));
    i -> id = id;
    i->prioridade = prioridade;
    if(ant == NULL){ //novo elemento sera o primeiro
        if(f->fila != NULL){
            f->fila->ant = i;
        }
        i->prox = f->fila;
        f->fila = i;
    }
    else {
        i->prox = ant->prox;
        ant->prox = i;
        i->ant = ant;
        if(i->prox != NULL)
            i->prox->ant = i;
    }
    f->arranjo[id] = i;
    return true;
}

bool aumentarPrioridade(PFILA f, int id, float novaPrioridade){
    if(id < 0 || id > f->maxRegistros) return false;
    if(buscaSeq(f, id) == NULL) return false;
    if(buscaSeq(f, id)->prioridade >= novaPrioridade) return false;
    PONT ant, i;
    i = buscaSeqExc(novaPrioridade, f, &ant);
    if(f->arranjo[id]->ant == NULL && ant == NULL){ // ja sou o primeiro, e continuo em primeiro
        f->arranjo[id]->prioridade = novaPrioridade;
        return true;
    }
    if(ant == NULL){ // sou primeiro elemento
        if(f->arranjo[id]->ant != NULL)
            f->arranjo[id]->ant->prox = f->arranjo[id]->prox;
        f->arranjo[id]->ant = ant;
        if(f->arranjo[id]->prox != NULL) // caso nao seja o ultimo
            f->arranjo[id]->prox->ant = f->arranjo[id]->ant;
        f->arranjo[id]->prox = f->fila;
        f->arranjo[id]->prox->ant = f->arranjo[id];
        f->fila = f->arranjo[id];
    }
    else {
        if(f->arranjo[id]->prox != NULL)// nao sou o ultimo
            f->arranjo[id]->prox->ant = f->arranjo[id]->ant;
        if (f->arranjo[id]->ant != NULL)
            f->arranjo[id]->ant->prox = f->arranjo[id]->prox;
        f->arranjo[id]->prox = ant->prox; // meu novo proximo realocado
        ant->prox = f->arranjo[id]; // minha nova posicao
        f->arranjo[id]->ant = ant;
        if(f->arranjo[id]->prox != NULL) //caso eu nao seja o ultimo.
            f->arranjo[id]->prox->ant = f->arranjo[id];
    }
    f->arranjo[id]->prioridade = novaPrioridade;
    
    return true;
}
bool reduzirPrioridade(PFILA f, int id, float novaPrioridade){
    if(id < 0 || id > f->maxRegistros) return false;
    if(buscaSeq(f, id) == NULL) return false;
    if(buscaSeq(f, id)->prioridade <= novaPrioridade) return false;
    PONT ant , i;
    i = buscaSeqExc(novaPrioridade, f, &ant);
    if(ant == NULL) // vou estar no primeiro da fila
    { if(f->arranjo[id]->prox != NULL)
        f->arranjo[id]->prox->ant = f->arranjo[id]->ant;
        f->arranjo[id]-> prox = f->fila;
        f->arranjo[id]->prox->ant = f->arranjo[id];
        f->fila = f->arranjo[id];
    }
    else { // demais posicoes
        if(f->arranjo[id]->prox != NULL)// nao sou o ultimo
            f->arranjo[id]->prox->ant = f->arranjo[id]->ant;
        if(f->arranjo[id]->ant == NULL) // sou o primeiro, com a prioridade antiga
            f->fila = f->arranjo[id]->prox; //fila tem que apontar para meu proximo
        if(f->arranjo[id]->prox->ant != NULL)
            f->arranjo[id]->ant->prox = f->arranjo[id]->prox;
        f->arranjo[id]->prox = ant->prox; // meu novo proximo realocado
        ant->prox = f->arranjo[id]; // minha nova posicao
        f->arranjo[id]->ant = ant;
        if(f->arranjo[id]->prox != NULL) //caso eu nao seja o ultimo.
            f->arranjo[id]->prox->ant = f->arranjo[id];
        
    }
    f->arranjo[id]->prioridade = novaPrioridade;
    return true;
}

PONT removerElemento(PFILA f){
    if(f->fila == NULL) return NULL;
    PONT apagar = f->fila;
    *apagar = *(f-> fila);
    f->arranjo[f->fila->id] = NULL;
    f->fila = f->fila->prox;
    if(f->fila != NULL)
        f->fila->ant = NULL;
    return apagar;
}

bool consultarPrioridade(PFILA f, int id, float* resposta){
    if(id< 0 || id > f-> maxRegistros) return false;
    if(buscaSeq(f, id) == NULL) return false;
    resposta = &f->arranjo[id]->prioridade;
    return true;
}


