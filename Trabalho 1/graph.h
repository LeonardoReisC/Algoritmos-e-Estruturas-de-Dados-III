#include <fstream>
#include <math.h>
#include "kruskal.h"

/** @brief Cria, dinamicamente, um vetor de listas e aloca uma lista de inteiros em cada slot. */
list<int>* adjacencyList(int size) {
    list<int>* tmp1 = new list<int>[size];
    list<int>* tmp2 = tmp1;

    for (int i = 0; i < size; i++, tmp2++) tmp2 = new list<int>;
    
    return tmp1;
}

/** @brief Fixa o vértice u(coordX, coordY). */
void readCoordinates(int limit, int* coordX, int* coordY, string fileName) {
    ifstream file;
    file.open(fileName);

    for (int i = 0; i < limit; i++) file >> *coordX >> *coordX >> *coordY;
    
    file.close();
}

/** @brief Calcula a distância euclidiana entre o vértice u(x1,y1) e v(x2,y2). */
int euclideanDistance(int x1, int x2, int y1, int y2) {
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

/*********************************************************************************
 * @brief Função geral que cria o grafo na representação em lista de adjacencia. *                                          
 * @return posicão inicial do vetor de listas                                    *   
 ********************************************************************************/
list<int>* buildGraph(string fileName, int size) {
    list<int>* graph = adjacencyList(size);
    ifstream file;
    for (int i = 0, coordX, coordY; i < size; i++) {
        readCoordinates(i+1, &coordX, &coordY, fileName);

        file.open(fileName);
        while (!file.eof()) {
            int auxX, auxY;
            file >> auxX >> auxX >> auxY; //le o vértice v(auxX, auxY) para calcula a distância com A(coordX, coordY) 
            graph[i].push_back(euclideanDistance(coordX, auxX, coordY, auxY));
        }
        file.close();
    }

    return graph;
}
