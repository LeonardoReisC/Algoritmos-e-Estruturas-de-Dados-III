#include <fstream>
#include "dijkstra.h"

/** @brief Aloca, dinamicamente, um vetor de ponteiros para "list<node>". */
list<node>** createAdjacencyList(int size) {
    list<node>** tmp = new list<node>*[size]; 
    for (int i = 0; i < size; i++) tmp[i] = new list<node>;
    return tmp;
}

/** @brief Adiciona uma aresta à lista de adjacências. */
void addEdge(string u, string v, int weight, list<node>** graph) {
    node* tmp = new node;
    *tmp = make_tuple(u,v,weight);
    graph[stoi(u)]->push_back(*tmp);
    delete tmp;
}

/************************************************************************************
 * @brief Constrói um grafo por representação em lista de adjacência da forma:      *    
 *                                                                                  *        
 *   +----+                                                                         *
 *   |    | -----> (0, 2, w(0,2)) --> (0, 5, w(0,5)) --> (0, 12, w(0,12)) --> ...   *    
 *   +----+                                                                         *                    
 *   |    | -----> (1, 7, w(1,7)) --> (1, 67, w(1,67))                              *                            
 *   +----+                                                                         *    
 *   |    | -----> (2, 8, w(2,8)) --> (2, 78, w(2,78)) --> (2, 63, w(2,63))         *                    
 *   +----+                                                                         *        
 *   |    | -----> (3, 85, w(3,85))                                                 *                        
 *   +----+                                                                         *                
 *      .                                                                           *        
 *      .                                                                           *            
 *      .                                                                           *                
 *                                                                                  *    
 ***********************************************************************************/
list<node>** buildGraph(string fileName, int size) {
    list<node>** graph = createAdjacencyList(size);
    ifstream file; 
    file.open(fileName);
    file >> size;
    do {
        string u,v;
        int weight;
        file >> u >> v >> weight;
        addEdge(u,v,weight,graph);
    } while (!file.eof());
    return graph;
}

/** @brief Encontra o diâmetro(maior caminho mínimo) de uma CPT(Cheapest-Paths Tree). */
list<node>::iterator* findDiameter(list<node>* minPaths) { 
    return searchList(minPaths, get<2>(*(minPaths->begin())), isHigher, true); 
}