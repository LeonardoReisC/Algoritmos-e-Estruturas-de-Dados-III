#include <fstream>
#include <tuple>
#include <list>
#include <math.h>

using namespace std;
typedef int dist;
typedef int vertex;

typedef tuple<vertex,dist> edge;
typedef list<list<edge>*> aList;

int euclideanDistance(int x1, int x2, int y1, int y2) { return sqrt(pow(x1-x2,2)+pow(y1-y2,2)); }

void addEdges(list<edge>* l, tuple<int,int> coordinates, string fileName) {
    list<edge>::iterator it = l->begin();
    ifstream file;
    file.open(fileName);

    for (int i = 0; it != l->end(); i++, it++) {
        int auxX, auxY;
        file >> auxX >> auxX >> auxY;
        *it = make_tuple((i+1), euclideanDistance(get<0>(coordinates), auxX, get<1>(coordinates), auxY));
    }
}

aList adjacencyList(int numberOfVertices, string fileName) {
    ifstream file;
    aList* tmp = new aList(numberOfVertices); 
    aList::iterator it = tmp->begin(); 
    for (int i = 0; it != tmp->end(); i++, it++) {
        *it = new list<edge>(numberOfVertices); 
        int x, y;
        file.open(fileName);
        for (int j = 0; j < i+1; j++) file >> x >> x >> y;
        file.close();
        tuple<int,int> coordinates = make_tuple(x,y);
        addEdges(*it, coordinates, fileName);
    }
    return *tmp;
}

aList buildGraph(int numberOfVertices, string fileName) { return adjacencyList(numberOfVertices, fileName); }