#include <iostream>
#include "graph.h"  

/*−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−-------------------------------------------------------------------+
|                                                UNIFAL - Universidade Federal de Alfenas                                       | 
|                                                  BACHARELADO EM CIÊNCIA DA COMPUTAÇÃO                                         | 
| Trabalho...........: Diâmetro Máximo de um Grafo                                                                              |    
| Disciplina.........: Algoritmos e Estruturas de Dados III                                                                     |    
| Professor..........: Iago Augusto de Carvalho                                                                                 |        
| Alunos.............: Daniel da Costa Lima     - 2021.1.08.026                                                                 |    
|                      Leonardo Reis Coimbra    - 2021.1.08.013                                                                 |       
|                      Renan Magalhães Lage     - 2021.1.08.020                                                                 |       
| Data...............: 21/07/2022                                                                                               |    
+------------------------------------------------------------------------------------------------------------------------------*/

void printStats(string fileName, list<int>* path, int cost) {
    int pos = fileName.find_last_of("/");
    if (pos != -1) fileName.erase(fileName.begin(), fileName.begin()+(pos+1)); //checa se o arquivo está dentro de um diretório

    pos = fileName.find_last_of("."); //checa se o arquivo apresenta extensão ".txt"
    if (pos != -1) fileName.erase(pos,4);
    
    cout << fileName << endl;
    for (list<int>::iterator it = path->begin(); it != path->end(); it++) {
        cout << *it << " ";
    }
    cout << endl << cost << endl;
}

int main() {
    list<node>** graph;
    list<node>::iterator diameter;
    int graphSize;
    ifstream file;
    string fileName;
    cout << "+---------------------------------------+" << endl;
    cout << "|\tInforme o arquivo de entrada\t| " << endl;
    cout << "+---------------------------------------+" << endl;
    cin >> fileName;
    cout << endl;

    file.open(fileName);
    if (file.is_open()) {
        file >> graphSize;
        file.close();
    } else {
        cout << "Unable to open file! Try again.." << endl;
        return EXIT_FAILURE;
    }
    
    graph = buildGraph(fileName, graphSize);

    //calcula Dijkstra para o 1º vértice(0) e armazena em "diameter" o diâmetro da CPT(Cheapest-Paths Tree) de raiz 0.
    list<node>* dijkstra = dijkstraAlgorithm(graph, to_string(0), graphSize);
    diameter = *findDiameter(dijkstra);

    //loop que calcula o diâmetro máximo de G(graph).
    //compara o diametro da CPT de raiz (i+1) com o maior diamêtro, até então encontrado.
    for (int i = 0; i < graphSize-1; i++) {
        list<node>* tmp = dijkstraAlgorithm(graph, to_string(i+1), graphSize);
        list<node>::iterator it = *findDiameter(tmp);
        
        if (isHigher(get<2>(*it), get<2>(*diameter))) {
            diameter = it;
            dijkstra = tmp;
        }
    }

    list<int>* path = getPath(dijkstra, diameter); 
    printStats(fileName, path, get<2>(*diameter));

    return EXIT_SUCCESS;
}