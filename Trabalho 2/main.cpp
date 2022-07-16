#include "graph.h"

/**
 * RESUMO
 * Para cada vertice no grafo, computar dijkstra e armazenar em uma lista e colocar essa lista em um vetor
 *      - o resultado será similar à uma lista de adjacencia contendo dijkstra para todos os vertices u
 * Selecionar o maior caminho dentre todos da estrutura contruida acima e disponibilizar a saida solicitada
 * */

int main() {
    list<node>** graph;
    int size;
    ifstream file;
    file.open("n100.txt"); // input para arquivo depois
    if (file.is_open()) {
        file >> size;
        file.close();
    } else {
        cout << "Unable to open file! Try again.." << endl;
        return 1;
    }
    
    buildGraph(size);
    graph = buildGraph(size);

    list<node>** minPath = new list<node>*[size];
    for (int i = 0; i < size; i++) {
        minPath[i] = dijkstraAlgorithm(graph, to_string(i), size);
    }
    
    

    return 0;
}