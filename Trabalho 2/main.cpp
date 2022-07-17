#include <iostream>
#include "graph.h"



/**
 * RESUMO
 * Para cada vertice no grafo, computar dijkstra e armazenar em uma lista e colocar essa lista em um vetor
 *      - o resultado será similar à uma lista de adjacencia contendo dijkstra para todos os vertices u
 * Selecionar o maior caminho dentre todos da estrutura contruida acima e disponibilizar a saida solicitada
 * */

void printStats(string fileName, list<int>* path, int cost) {
    fileName.erase(fileName.size()-4);
    cout << fileName << endl;
    for (list<int>::iterator it = path->begin(); it != path->end(); it++) {
        cout << *it << " ";
    }
    cout << endl << cost;
}

int main() {
    list<node>** graph;
    list<node>::iterator diameter;
    int graphSize;
    ifstream file;
    string fileName;
    cout << "+---------------------------------------+" << endl;
    cout << "|\tInforme o arquivo de entrada\t| " << endl;
    cout << "+---------------------------------------+" << endl;
    cin >> fileName;

    file.open(fileName);
    if (file.is_open()) {
        file >> graphSize;
        file.close();
    } else {
        cout << "Unable to open file! Try again.." << endl;
        return EXIT_FAILURE;
    }
    
    graph = buildGraph(fileName, graphSize);

    list<node>* dijkstra = dijkstraAlgorithm(graph, to_string(0), graphSize);
    diameter = *findDiameter(dijkstra);

    for (int i = 0; i < graphSize-1; i++) {
        list<node>* tmp = dijkstraAlgorithm(graph, to_string(i+1), graphSize);
        list<node>::iterator it = *findDiameter(tmp);
        
        if (isHigher(get<2>(*it), get<2>(*diameter))) {
            diameter = it;
            dijkstra = tmp;
        }
    }

    list<int>* path = getPath(dijkstra, diameter);
    printStats(fileName, path, get<2>(*diameter));
    return EXIT_SUCCESS;
}