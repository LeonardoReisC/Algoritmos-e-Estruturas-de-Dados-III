#include "graph.h"
#include "heuristics.h"

void printSolution(list<vertex>* solution, string fileName) {
    int cost = solution->back();
    solution->pop_back();
    solution->sort();

    cout << fileName << endl;
    for (list<int>::iterator it = solution->begin(); it != solution->end(); it++) cout << *it << " ";
    cout << endl;
    cout << cost << endl;
}

int graphSize(string fileName) {
    ifstream file;
    file.open(fileName);
    int i = 0;
    string aux;
    for (;!file.eof(); i++) getline(file,aux);
    return i;
}

int main(int argc, char *argv[]) {
    srand(1); 
    ifstream file;
    int fileSize;
    int time = stoi(argv[1]); //passa o tempo e o arquivo em linha de comando
    string fileName = argv[2];

    file.open(fileName);
    if (file.is_open()) {
        fileSize = graphSize(fileName);
        file.close();
    } else { 
        cout << "Unable to open file!" << endl;
        return 1;
    }

    aList graph = buildGraph(fileSize, fileName);
    list<vertex>* solution = new list<vertex>;
    solution = heuristic(graph);
    localSearch(&solution,graph, time);

    printSolution(solution, fileName);
    return 0;
}
