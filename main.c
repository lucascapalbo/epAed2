#include <stdio.h>
#include "ep1.c"
int main(int argc, const char * argv[]) {
    TipoGrafo grafo;
    char nomeArquivo[] = "/Users/lucasbordinhoncapalbo/Documents/Ep1 AED2/en.lproj/grafo.txt";
   if( leGrafo(nomeArquivo, &grafo) == 0)
       printf("arquivo nao encontrado");
    printf("DFS: ");
    buscaProfundidade(&grafo, 1);
    // imprimeGrafo(&grafo);
    return 0;
}
