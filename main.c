#include <stdio.h>
#include "ep1.c"
int main(int argc, const char * argv[]) {
    TipoGrafo grafo;
    char nomeArquivo[] = "/Users/lucasbordinhoncapalbo/Documents/Ep1 AED2/en.lproj/grafo.txt";
    if( leGrafo(nomeArquivo, &grafo) == 0)
        printf("arquivo nao encontrado");
    printf("DFS: \n");
    buscaProfundidade(&grafo, 1, false);
    printf("\nDFS Paths: \n");
    buscaProfundidade(&grafo, 1, true);
    int numeroConexos = grafo.componentesConexos;
    printf("Connected components: %i \n", numeroConexos);
    componentesConexos(&grafo,1,false, false);
    printf("\n");
    imprimeArticulacao(&grafo, 1);
    // imprimeGrafo(&grafo);
    return 0;
}
