#include <tuple>
#include <list>
#include "math.h"

using namespace std;

typedef tuple<string,string,int> node;

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


bool isLower(int x, int y) {
    if (x < y) return true;
    else return false;
}

bool isHigher(int x, int y) {
    if (x >= y) return true;
    else return false;
}

bool equalsTo(int x, int y) {
    if (x == y) return true;
    else return false;
}

/** @brief Retorna um lista do grafo contendo todos so adjacentes de "vertex" */
list<node>* findAdjacents(list<node>** graph, string vertex) { return graph[stoi(vertex)]; } 

/**************************************************************** 
 * @brief Verifica se um vértice "vertex" está na Minimum Heap  *    
 * @return verdadeiro, se existir "vertex"                        *    
****************************************************************/
bool isInMinHeap(list<node>* minHeap, string vertex, list<node>::iterator* v) {
    for (list<node>::iterator it = minHeap->begin(); it != minHeap->end(); it++) {
        if (get<1>(*it) == vertex) {
            *v = it;
            return true;
        }
    }
    return false;
}

/** @brief Aloca, dinamicamente, uma lista contendo todos os vertices de G e inicializa seus pesos com infinito*/
list<node>* createMinHeap(int size, string sourceRoot) {
    list<node>* tmp = new list<node>;

    for (int i = 0; i < size; i++) {
        node* u = new node;
        *u = make_tuple(to_string(i), to_string(i), (int)INFINITY);
        if (get<0>(*u) == sourceRoot) *u = make_tuple(sourceRoot,sourceRoot,0);

        tmp->push_back(*u);
        delete u;
    }
    return tmp;
}

/** @brief Varre a */
list<node>::iterator* searchList(list<node>* List, int factor, bool func(int,int), bool state) {
    list<node>::iterator* tmp = new list<node>::iterator; //# list<node>::iterator* tmp = NULL;

    for (list<node>::iterator it = List->begin(); it != List->end(); it++) {
        int compare;
        if (state) compare = get<2>(*it);
        else compare = stoi(get<1>(*it)); 

        if (func(compare, factor)) {
            factor = get<2>(*it);
            *tmp = it;
            if (!state) return tmp;
        }
    }
    
    return tmp;
}

/** @brief Retira da Minimum Heap o vértice de menor peso. */
node* extractMinHeap(list<node>* minHeap) {
    node* tmp = new node;
    list<node>::iterator* it = searchList(minHeap, (int)INFINITY, isLower, true);
    //# if (it == NULL) return NULL;
    *tmp = **it;
    minHeap->erase(*it);
    return tmp;
}

/** Computa o Algoritmo de Dijkstra levando em consideração a raiz(sourceRoot) da CPT. */
list<node>* dijkstraAlgorithm(list<node>** graph, string sourceRoot, int size) {
    list<node>* minHeap = createMinHeap(size, sourceRoot);
    list<node>* dijkstra = new list<node>;
    while (!minHeap->empty()) {
        list<node>* adjacents = new list<node>;
        node* vertex = extractMinHeap(minHeap);
        //# if (vertex == NULL) return dijkstra;
        dijkstra->push_back(*vertex);
        adjacents = findAdjacents(graph, get<1>(*vertex));

        for (list<node>::iterator it = adjacents->begin(), v; it != adjacents->end(); it++) {
            if (minHeap->empty()) return dijkstra;
            if (isInMinHeap(minHeap, get<1>(*it), &v)) {
                int dV = get<2>(*v), wU_V = get<2>(*it), dU = get<2>(*vertex);           
                if (dV > wU_V + dU) {
                    get<2>(*v) = wU_V + dU;
                    get<0>(*v) = get<1>(*vertex);
                }
            }
        }
    }
    return dijkstra;
}

list<int>* getPath(list<node>* dijkstra, list<node>::iterator it) {
    list<int>* path = new list<int>;
    while (it != dijkstra->begin()) {
        path->push_back(stoi(get<1>(*it)));
        it = *searchList(dijkstra, stoi(get<0>(*it)), equalsTo, false);
    }
    path->push_back(stoi(get<1>(*it)));
    path->reverse();
    return path;        
}