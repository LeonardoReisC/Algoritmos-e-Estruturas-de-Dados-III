#include "graph.h"

/** @brief Calcula o número de vertices do arquivo. */
int fileSize(string fileName) {
    int size = 0;
    ifstream file;
    file.open(fileName);
    while (!file.eof()) {
        string aux;
        getline(file, aux);
        size++;       
    }
    file.close();
    return size;
}

int main() {
    list<int>* graph;
    list<tree>::iterator kruskal;
    list<list<tree>::iterator>* subGraph;
    int size;
    string fileName;
    ifstream file;
    cout << "+---------------------------------------+" << endl;
    cout << "|\tInforme o arquivo de entrada\t| " << endl;
    cout << "+---------------------------------------+" << endl;
    cin >> fileName;
    cout << endl;

    file.open(fileName);
    if (!file.is_open()) {
        cout << "Unable to open file! Try again..." << endl;
        return EXIT_FAILURE;
    } else file.close();

    size = fileSize(fileName);
    graph = buildGraph(fileName, size);
    kruskal = kruskalAlgorithm(graph, size);
    subGraph = searchSubGraph(kruskal);
    printSubGraph(fileName, subGraph);
    
    return EXIT_SUCCESS;
}
