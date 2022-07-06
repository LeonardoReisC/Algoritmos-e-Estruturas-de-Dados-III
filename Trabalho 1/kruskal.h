#include "search.h"

/** @brief Função parâmetro para ordenar as arestas conforme o critério de ordenação ".weight". */
bool compare(const node& first, const node& second) {
    return first.weight < second.weight;
}

/** @brief Função que lê os vertices do grafo, aloca (u,v) e o peso de sua aresta em uma struct "node" e ordena essas structs. */
list<node>* sortEdges(list<int>* graph, int size) {
    list<node>* edges = new list<node>;
    for (int i = 0; i < size; i++) {
        list<int>::iterator it = graph[i].begin();
        for (int j = 0; it != graph[i].end(); it++, j++) {
            node* tmp = new node;
            tmp->u = i;
            tmp->v = j;
            tmp->weight = *it;
            edges->push_back(*tmp);
            delete tmp;
        }        
    }

    edges->sort(compare);
    return edges;
}

/************************************************************************************************ 
 * @brief Cria a floresta desenhada na descrição da struct "tree".                              * 
 * @details                                                                                     *         
 *               +-----------+   +-----------+   +-----------+                +-------------+   *  
 *               | u = v = 1 |   | u = v = 2 |   | u = v = 3 |                | u = v = 532 |   *                 
 *  forest---->  | ------------> | ------------> | ------------>     ...      |             |   *
 *               | peso = 0  |   | peso = 0  |   | peso = 0  |                | peso = 0    |   *                
 *               +-----------+   +-----------+   +-----------+                +-------------+   *                    
 *                     |               |               |                            |           *    
 *                     |               |               |                            |           *              
 *                     V               V               V                            V           *          
 *                   vazio           vazio           vazio                        vazio         *             
 ***********************************************************************************************/
list<tree>* buildForest(int size) {
    list<tree>* set = new list<tree>;
    for (int i = 0; i < size; i++) {
        list<tree>::iterator it;
        tree* tmp = new tree;
        tmp->edge.u = i;
        tmp->edge.v = i;   
        tmp->subTree = new list<tree>;
        set->push_back(*tmp);  
        it = --set->end();
        it->parent = it;
        delete tmp;
    }

    return set;
}

/******************************************************************* 
 * @brief Varre as subÁrvores de u para encontra o vértice v.      *                                                                  
 *                                                                 *  
 * @param setNumber vértice v.                                     *                              
 * @param vertex estrutura que armazenará o endereço do vértice v. *
 *                                                                 *
 * @return verdadeiro, se v estiver no conjunto u.                 *                                                                  
 ******************************************************************/
bool findVertex(list<tree>* set, list<tree>::iterator it, local* vertex, int setNumber) {
    for (; it != set->end(); it++) {
        if (it->edge.v == setNumber) {
            vertex->address = it;
            return true;
        }

        if (!it->subTree->empty()) {
            if (findVertex(it->subTree, it->subTree->begin(), vertex, setNumber)) return true;
        }
    }

    return false;
}

/***********************************************************************************
 * @brief Encontra o conjunto u em que v se encontra.                              * 
 * @details                                                                        *          
 * Varre cada conjunto da floresta e chama findVertex(v).                          *                  
 *                                                                                 *                              
 * @param vertex armazena o endereço de v e o endereço de u, caso v estiver em u.  *                                                                                       *      
 **********************************************************************************/
void findRoot(list<tree>* forest, local* vertex, int setNumber) {
    list<tree>::iterator it = forest->begin();
    for (; it != forest->end(); it++) {
        if (it->edge.v == setNumber) {
            vertex->root = it;
            vertex->address = it;
            return;
        }

        if (!it->subTree->empty()) {
            if (findVertex(it->subTree, it->subTree->begin(), vertex, setNumber)) {
                vertex->root = it;
            }
        }
    }
}

/*****************************************************************
 * @brief Recupera o vértice pai(parent) de v, isto é u.         *   
 * @details                                                      *               
 * Evita erros depois de realizar a operação connectTrees().     *
 ****************************************************************/
void recoverParent(list<tree>::iterator root) {
    for (list<tree>::iterator it = root->subTree->begin(); it != root->subTree->end(); it++) {
        it->parent = root;
        if (!it->subTree->empty()) {
            recoverParent(it);
        }
    }
}

/********************************************************************************
 * @brief Inverte o conjunto u para v.                                          *
 * @details                                                                     *      
 * v passa a ser a raiz do conjunto e u passa a pertercer ao novo conjunto v.   *    
 *******************************************************************************/
void invertRoot(local* set) {
    list<tree>::iterator it;
    tree* tmp = new tree;
    tmp->subTree = new list<tree>;
    int v = set->address->edge.v;

    //passa v como raiz
    *tmp = *set->root->parent;
    set->root->edge.weight = set->root->parent->edge.weight;
    tmp->edge.weight = 0;

    //apaga v do conjunto u e une u em v
    set->root->subTree->erase(set->root->parent);
    tmp->subTree->push_front(*set->root);
    *set->root = *tmp;

    set->root->subTree->begin()->parent = set->root;

    //recursão até que v se torne a raiz
    if (set->root->edge.v != v) {
        invertRoot(set);
    }
}

/************************************************************************************************
 * @brief Orienta o conjunto "set" para v.                                                      *     
 * @details                                                                                     *     
 * Localiza v em "set" e altera "parent" dos vértices que conectam "set" a v, incluindo "set".  *     
 * Necessária para a inversão da raiz.                                                          *             
 ***********************************************************************************************/
void reArrange(local* set) {
    list<tree>::iterator it = set->root->subTree->begin();
    for (; it != set->root->subTree->end(); it++) {
        if (it == set->address) {
            set->root->parent = it;
            return;
        }
        if (!it->subTree->empty()) {
            if (findVertex(it->subTree, it->subTree->begin(), set, set->address->edge.v)) {
                local* tmp = new local;
                tmp->root = it;
                tmp->address = set->address;
                set->root->parent = it;
                reArrange(tmp);
                return;
            }
        }
    }
}

/** @brief Conectam os conjuntos u e v. */
void connectTrees(local* A, local* B, int weight) {
    //chama "reArrange()"" e "invertRoot()"", se, e somente se, v pertencer à um conjunto B, tal que v não é a raiz"
    if (B->root != B->address) { 
        reArrange(B);
        invertRoot(B);
    }

    B->root->parent = A->address;
    B->root->edge.weight = weight;
    A->address->subTree->push_back(*B->root);

    recoverParent(A->root);
}

/**************************************************
 * @brief Implementação do algoritmo de kruskal.  *  
 *                                                *  
 * @return Árvore Geradora mínima.                *      
 *************************************************/
list<tree>::iterator kruskalAlgorithm(list<int>* graph, int size) {
    list<node>* edges = sortEdges(graph, size);
    list<tree>* forest = buildForest(size);

    for (list<node>::iterator it = edges->begin(); it != edges->end();) {
        if (it->u != it->v) { 
            local *vertex1 = new local;
            local *vertex2 = new local;
            findRoot(forest, vertex1, it->u);
            findRoot(forest, vertex2, it->v);
            
            if (vertex1->root != vertex2->root) {
                connectTrees(vertex1, vertex2, it->weight);
                forest->erase(vertex2->root);
            }
            delete vertex1;
            delete vertex2;
        }
        it++;
        //remove a aresta da lista ordenada
        edges->pop_front();
    }
    return forest->begin();
}