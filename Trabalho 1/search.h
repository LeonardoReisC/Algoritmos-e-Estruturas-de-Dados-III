#include <iostream>
#include <list>

using namespace std;

/** @brief Estrutura que armazena (u,v) e o peso de sua aresta. */
struct node {
    int u;
    int v;
    int weight = 0;
};

/*******************************************************************************************************************
 * @brief Contém o vértice v.                                                                                      *     
 * @details                                                                                                        *     
 * O peso das subÁrvores de u(raiz principal) equivale ao peso da aresta de v com o seu pai(o que está acima)      *         
 *               +-----------+   +-----------+   +-----------+                +-------------+                      *     
 *               | u = v = 1 |   | u = v = 4 |   | u = v = 7 |                | u = v = 532 |                      *             
 *  forest---->  | ------------> | ------------> | ------------>     ...      |             |                      *         
 *               | peso = 0  |   | peso = 0  |   | peso = 0  |                | peso = 0    |                      *         
 *               +-----------+   +-----------+   +-----------+                +-------------+                      *     
 *                     O               |               |                            |                              *             
 *                     |               |               |                            |                              *         
 *                     |               V               V                            V                              *              
 *                     |             vazio           vazio                        vazio                            *     
 *                     V                                                                                           *         
 *               +-----------+            +-----------+                                                            *                 
 *               | u = v = 2 |            | u = v = 3 |                                                            *     
 * (1,2)         | ------------------->   | ----------|  (2,3)                                                     *         
 * peso(1,2 = 15)| peso = 15 |            | peso = 28 |  peso(2,3) = 28                                            *                   
 *               +-----------+            +-----------+                                                            *             
 *                     |                        |                                                                  *             
 *                     |                        |                                                                  *     
 *                     |                        |                                                                  *                 
 *                     V                        V                                                                  *             
 *               +-----------+            +-----------+                                                            *         
 *               | u = v = 5 |            | u = v = 6 |                                                            *         
 * (1,5)         | ----------|            | ----------| (3,6)                                                      *     
 * peso(1,5) = 89| peso = 89 |            | peso = 97 | peso(3,6) = 97                                             *         
 *               +-----------+            +-----------+                                                            *         
 *                     |                        |                                                                  *         
 *                     |                        |                                                                  *         
 *                     V                        V                                                                  *                 
 *                   vazio                    vazio                                                                *         
 *                                                                                                                 * 
 * Os vértices 2,3,5,6 foram conjunto únicos de "forest".                                                          *             
 * Foram apagados dela, por que se conectaram aos vértice {1,2,1,3}, respectivamente                               *             
 * Agora todos eles foram um só conjunto cuja raiz é o vértice 1                                                   *                 
 ******************************************************************************************************************/    
struct tree {
    node edge;
    list<tree>::iterator parent;
    list<tree>* subTree;;
};

/*****************************************************************************************
 * @brief Estrutura simples armazena o cojunto u em que v pertence e seu endereço nela.  *   
 * @details                                                                              *   
 * Na estrutura do comentário da struct "tree", a struct "local" para o vertice 3 seria. *                                    
 ****************************************************************************************/
struct local {
    list<tree>::iterator root;
    list<tree>::iterator address;
};

/** @brief Retorna na saída o nome do arquivo, os 3 vértices do subgrafo e a soma dos pesos de suas arestas. */
void printSubGraph(string fileName, list<list<tree>::iterator>* subGraph) {
    fileName.erase(fileName.size() - 4);
    cout << fileName << endl;
    cout << (*subGraph->begin())->edge.v << " " << (*++subGraph->begin())->edge.v << " " << (*--subGraph->end())->edge.v << endl;
    cout << (*++subGraph->begin())->edge.weight + (*--subGraph->end())->edge.weight;
}

/** @brief Função recursiva quer pecorre a Árvore Geradora Mínima para encontrar o maior suibgrafo de 3 vértices. */
list<list<tree>::iterator>* findThreeVertices(list<tree>::iterator root) {
    list<list<tree>::iterator>* subGraph = new list<list<tree>::iterator>;
    list<list<tree>::iterator>::iterator index;
    list<tree>::iterator it = root->subTree->begin();
    subGraph->resize(3);
    *subGraph->begin() = root;
    *++subGraph->begin() = it; 
    int high = (*++subGraph->begin())->edge.weight;

    for (; it != root->subTree->end(); it++) { 
        list<tree>::iterator it_2;
        if(!it->subTree->empty()) {
            it_2 = it->subTree->begin();
            list<list<tree>::iterator>* v;
            int sum = 0;
            
            for (; it_2 != it->subTree->end(); it_2++) {
                if (high <= it->edge.weight + it_2->edge.weight) {
                    *subGraph->begin() = root;
                    *++subGraph->begin() = it;
                    *--subGraph->end() = it_2;
                    high = (*++subGraph->begin())->edge.weight + it_2->edge.weight;
                }
            }

            if (it->subTree->size() == 1 && it->subTree->begin()->subTree->empty()) {
                return subGraph;
            }

            v = findThreeVertices(it);
            sum = (*++v->begin())->edge.weight + (*--v->end())->edge.weight;
            if (high < sum) {
                *subGraph->begin() = *v->begin();
                *++subGraph->begin() = *++v->begin();
                *--subGraph->end()  = *--v->end();
                high = sum;
            }
            delete v;
        }

        it_2 = ++it;
        it--;
        for (; it_2 != root->subTree->end(); it_2++) {
            if (high < it->edge.weight + it_2->edge.weight) {
                *subGraph->begin() = root;
                *++subGraph->begin() = it;
                *--subGraph->end()  = it_2;
                high = (*++subGraph->begin())->edge.weight + it_2->edge.weight;
            }
        }
    }
    return subGraph; 
}

/** @return lista contendo o maior subgrafo de 3 vértices. */
list<list<tree>::iterator>* searchSubGraph(list<tree>::iterator agm) {
    return findThreeVertices(agm);
}