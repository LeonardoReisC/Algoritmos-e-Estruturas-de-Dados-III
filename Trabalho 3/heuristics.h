#include <iostream>
#include <tuple>
#include <list>
#include <iterator>
#include <cstdlib>
#include <time.h>

using namespace std;
typedef int dist;
typedef int vertex;

typedef tuple<vertex,dist> edge;
typedef list<list<edge>*> aList;

//retorn lista de vertex
list<edge> findAdjacents(int vertex, aList graph) {
    aList::iterator it = graph.begin();
    for (int i = 0; i < (vertex-1); i++, it++);
    return **it;
}

bool formedCycle(list<vertex>* solution, list<vertex>::iterator it, vertex v) {
    if (it == prev(solution->end(), 1)) return false;

    return (*it == v) || formedCycle(solution, ++it, v);
   
}

bool formedCycleInterface(list<vertex>* solution, vertex v) {
    return formedCycle(solution, solution->begin(), v);
}

edge getEdge(list<edge> adjacents, vertex v) {
    list<edge>::iterator it = adjacents.begin();
    for (; get<0>(*it) != v; it++);
    return *it;
}

bool compare(const tuple<int,int,int>& first, const tuple<int,int,int>& second) {
    return get<2>(first) < get<2>(second);
}

list<tuple<int,int,int>> getRelatedEdges(list<vertex>* v, aList graph) {
    list<tuple<int,int,int>>* edges = new list<tuple<int,int,int>>;
    for (int i = 0; i < 4; i++) {
        list<edge> adjacents = findAdjacents(i+1,graph);
        list<vertex>::iterator it = v->begin();
        advance(it, i);
        int u = *it;
        for (it++; it != prev(v->end(), 1); it++) {
            edge e = getEdge(adjacents, *it);
            edges->push_back(make_tuple(u,get<0>(e),get<1>(e)));
        }
    }
    return *edges;
}

int getIndex(list<vertex>* vector, vertex v) {
    list<vertex>::iterator it = vector->begin();
    int i = 0;
    for (; *it != v; i++, it++);
    return i;
}

//tenta a possibilidade de ser uma arovores binaria
//retorna o custo do subgrafo
int calculateCost(list<vertex>* solution, aList graph) {
    list<tuple<int,int,int>> edges = getRelatedEdges(solution, graph);
    int info[5] = {0, 0, 0, 0, 0}, sum = 0;
    edges.sort(compare);

    for (int i = 0; i < 4; i++, edges.pop_back()) {
        tuple<int,int,int> e = edges.back();
        int index = getIndex(solution, get<0>(e));
        if (info[index] == 2) {
            i--;
            continue;
        }
        sum += get<2>(e);
        info[index]++;
    }

    return sum;
}

edge getHigherDistance(list<edge> adjacents) {
    list<edge>::iterator it = adjacents.begin();
    edge hD = make_tuple(0,0);
    int higherDistance = 0;
    for (; it != adjacents.end(); it++) {
        if (get<1>(*it) > get<1>(hD)) hD = *it;
    }

    return hD;
}

//pega a maior aresta do vertice e assim por diante
//se formar ciclo, escolhe uma aleatoria
list<vertex>* heuristic(aList graph) {
    vertex u = (rand() % graph.size()) +1;
    list<vertex>* solution = new list<int>;
    solution->push_back(u);
    solution->push_back(0);
    list<vertex>:: iterator it = solution->begin();
    int cost = 0;
    for (int i = 0; i < 4; i++) {
        list<edge> adjacents = findAdjacents(*it, graph);
        edge e = getHigherDistance(adjacents);
        it++;
        *it = get<0>(e);
        while (formedCycleInterface(solution, *it)) {
            *it = (rand() % graph.size()) +1;
            e = getEdge(adjacents,*it);
        }

        cost += get<1>(e);
        solution->push_back(0);
    }
    solution->back() = cost;

    return solution;
}

//escolhe, aleatoriamente as arestas
list<vertex>* randomize(aList graph) {
    vertex u = (rand() % graph.size()) +1;
    list<vertex>* solution = new list<int>;
    solution->push_back(u);
    solution->push_back(0);
    list<vertex>:: iterator it = solution->begin();
    it++;
    for (int i = 1; i < 5; i++, it++) {
        *it = (rand() % graph.size()) +1;
        if (formedCycleInterface(solution, *it)) {
            it--;
            i--;
            continue;
        }
        solution->push_back(0);
    }

    solution->back() = calculateCost(solution, graph);
    
    return solution;
}

list<vertex>* invert(list<vertex>* solution, aList graph) {
    list<vertex>* s1 = new list<vertex>;
    list<vertex>::iterator it = solution->begin();
    for (int i = 0; i < 5; i++, it++) s1->push_back(*it);  
    s1->reverse();
    s1->push_back(calculateCost(s1,graph));
    return s1;
}

bool compareSolution(int cost1, int cost2) { return cost1 > cost2; }

void localSearch(list<vertex>** s, aList graph, int seconds) {
    time_t start = time(NULL);
    while (difftime(time(NULL),start) < seconds) {
        list<vertex>* s1 = invert(*s,graph);
        if (compareSolution(s1->back(), (*s)->back())) *s = s1;
        else {
            s1 = randomize(graph);
            if (compareSolution(s1->back(), (*s)->back())) *s = s1;
            else {
                s1 = randomize(graph);
                if (compareSolution(s1->back(), (*s)->back())) *s = s1;
            }
        }
    }
}