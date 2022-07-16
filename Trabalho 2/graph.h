#include <fstream>
#include "dijkstra.h"

list<node>** createAdjacencyList(int size) {
    list<node>** tmp = new list<node>*[size]; 
    for (int i = 0; i < size; i++) tmp[i] = new list<node>;
    return tmp;
}

void addEdge(string u, string v, int weight, list<node>** graph) {
    node* tmp = new node;
    tmp->info = make_tuple(u,v,weight);
    graph[stoi(u)]->push_back(*tmp);
    delete tmp;
}

list<node>** buildGraph(int size) {
    list<node>** graph = createAdjacencyList(size);
    ifstream file; 
    file.open("n100.txt");
    file >> size;
    do {
        string u,v;
        int weight;
        file >> u >> v >> weight;
        addEdge(u,v,weight,graph);
    } while (!file.eof());
    return graph;
}