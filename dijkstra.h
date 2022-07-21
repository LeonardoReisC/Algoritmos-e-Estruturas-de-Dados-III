#include <tuple>
#include <list>
#include "math.h"

using namespace std;

typedef tuple<string,string,int> node;

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
 * @return verdairo, se existir "vertex"                        *    
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

/** 
 * @brief Varre uma lista genérica para encontrar um elemento que tem relação com "factor",
 * tal que essa relação é definida por "func"
 * 
 * @param func critério de seleção dos elementos da lista com "factor"
 * @param state define o elemento que será seleciona da tupla bem como impacta na execução 
 * do código
 *      Se true: compara com o 3º elemento da tupla e retorna seu valor no momento em que
 *               o elemento for encontra na lista, interrompendo da execução da função
 *      Caso contrário, compara com o 2º elemento da tupla e não retorna seu valor. Portanto
 *                      a lista deverá ser percorrida por completo
 * 
 * @return iterator que aponta para o local encontrado na lista
*/
list<node>::iterator* searchList(list<node>* List, int factor, bool func(int,int), bool state) {
    list<node>::iterator* tmp = NULL;

    for (list<node>::iterator it = List->begin(); it != List->end(); it++) {
        int compare;
        if (state) compare = get<2>(*it);
        else compare = stoi(get<1>(*it)); 

        if (func(compare, factor)) {
            factor = get<2>(*it);
            tmp = new list<node>::iterator;
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
    if (it == NULL) return NULL;
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
        if (vertex == NULL) return dijkstra;
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