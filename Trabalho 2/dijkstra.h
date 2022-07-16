#include <iostream>
#include <tuple>
#include <list>
#include "math.h"

using namespace std;

/**
 * Cria uma minHeap de tamanho V que contem todos os vertices de G - "ok"
 * Todas as distância da minHeap devem ser inicializadas com INFINITY - "ok"
 * Seleciona a "source root" e altera a distância para 0 - "ok"
 * Enquanto minHeap não estiver vazia, faça - "ok"
 *      extrai o vertice de menor distância(de inicio sera a "source root") - "ok"
 *      para cada v adjacente de u, - "ok"
 *          verifica se v esta na minHeap - "ok"
 *              se d(v) > w(u,v) + d(u) - "ok"
 *                  atualiza d(v) - "ok"
 * 
 * */

struct node{
    tuple<string,string,int> info;
};

list<node>* findAdjacents(list<node>** graph, string vertex) { return graph[stoi(vertex)]; } 

bool isInMinHeap(list<node>* minHeap, string vertex, list<node>::iterator* v) {
    for (list<node>::iterator it = minHeap->begin(); it != minHeap->end(); it++) {
        if (get<1>(it->info) == vertex) {
            *v = it;
            return true;
        }
    }
    return false;
}

list<node>* createMinHeap(int size, string sourceRoot) {
    list<node>* tmp = new list<node>;
    for (int i = 0; i < size; i++) {
        node* u = new node;
        u->info = make_tuple(to_string(i), to_string(i), (int)INFINITY);
        if (get<0>(u->info) == sourceRoot) u->info = make_tuple(sourceRoot,sourceRoot,0);
        tmp->push_back(*u);
        delete u;
    }
    return tmp;
}

node* extractMinHeap(list<node>* minHeap) {
    node* tmp = NULL;
    int min = (int)INFINITY;
    list<node>::iterator it1 = minHeap->begin();
    for (list<node>::iterator it2 = it1; it2 != minHeap->end(); it2++) {
        if (get<2>(it2->info) < min) {
            min = get<2>(it2->info);
            tmp = new node;
            *tmp = *it2;
            it1 = it2;
        }
    }
    minHeap->erase(it1);
    return tmp;
}

list<node>* dijkstraAlgorithm(list<node>** graph, string sourceRoot, int size) {
    list<node>* minHeap = createMinHeap(size, sourceRoot);
    list<node>* dijkstra = new list<node>;
    while (!minHeap->empty()) {
        list<node>* adjacents = new list<node>;
        node* vertex = extractMinHeap(minHeap);
        if (vertex == NULL) return dijkstra;
        dijkstra->push_back(*vertex);
        adjacents = findAdjacents(graph, get<1>(vertex->info));

        for (list<node>::iterator it = adjacents->begin(), v; it != adjacents->end(); it++) {
            if (minHeap->empty()) return dijkstra;
            if (isInMinHeap(minHeap, get<1>(it->info), &v)) {
                int dV = get<2>(v->info), wU_V = get<2>(it->info), dU = get<2>(vertex->info);           
                if (dV > wU_V + dU) {
                    get<2>(v->info) = wU_V + dU;
                    get<0>(v->info) = get<1>(vertex->info);
                }
            }
        }
    }
    return dijkstra;
}