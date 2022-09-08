#include "conflict.hpp"


/** @brief Retorna o index de uma transação no vetor de transações.
* @details A partir de um id de uma transação, esta função é responsável por retornar
* o index da transação correspondente no vetor de transações já criado.
*
* @param adjacencies Vetor com todas as transações ativas.
* @param id Identificador da transação a ser encontrada.
*
* @return int
*/
int findTransactionIndex(vector<node_conflict_t> &adjacencies, int id) {
    int size = adjacencies.size();

    for (int i = 0; i< size; i++) {
        if (adjacencies[i].id == id) {
            return i;
        }
    }

    return -1;
}


/** @brief Atualiza o vetor de transações lidas para.
* @details Ao receber uma nova transação, um nodo correspondente é criado no vetor
* de transações caso não exista e atualizado caso exista.
*
* @param adjacencies Vetor com todas as transações ativas.
* @param transaction Nova transação lida.
*
* @return void
*/
void updateConflict(vector<node_conflict_t> &adjacencies, transaction_t transaction) {
    int nodeIndex = findTransactionIndex(adjacencies, transaction.id);

    if (nodeIndex == -1) {
        node_conflict_t aux;
        aux.id = transaction.id;
        aux.commit = false;
        aux.visited = false;
        aux.neighbours = {};
    
        adjacencies.push_back(aux);
        nodeIndex = adjacencies.size() - 1;
    }

    node_conflict_t *node = &adjacencies[nodeIndex];

    if (transaction.operation.type == 'C' || transaction.operation.type == 'c') {
        node->commit = true;
        return;
    }
    
    for (auto &adjacency: adjacencies) {
        if (adjacency.id == transaction.id) {
            adjacency.operations.push_back(transaction.operation);
        }
        else {
            vector<operation_t> operations = adjacency.operations;
            for (auto operation: operations) {
                if (operation.attr == transaction.operation.attr) {
                    if (
                        (
                            (operation.type == 'R' || operation.type == 'r') &&
                            (transaction.operation.type == 'W' || transaction.operation.type == 'w')
                        ) ||
                        (
                            (operation.type == 'W' || operation.type == 'w') &&
                            (transaction.operation.type == 'R' || transaction.operation.type == 'r')
                        ) ||
                        (
                            (operation.type == 'W' || operation.type == 'w') &&
                            (transaction.operation.type == 'W' || transaction.operation.type == 'w')
                        )
                    ) {
                        node->neighbours.insert(adjacency.id);
                    }
                }
            }
        }
    }
}


/** @brief Função responsável por apontar se todas as transações ativas foram commitadas.
* @details Percorre o vetor de tranções ativas a caso todas tenham sido commitadas returna true.
*
* @param adjacencies Vetor com todas as transações ativas.
*
* @return bool
*/
bool transactionsClosed(vector<node_conflict_t> adjacencies) {
    for (auto transaction: adjacencies) {
        if (transaction.commit == false) {
            return false;
        }
    }

    return true;
}


/** @brief Função responsável por informar o index da transação
* @details Ao executarmos uma Depth First Search (DFS) é necessário
* armazenar em um vetor de booleanos se a posição corrente já foi visitada
* como forma de se detectar ciclos. Está função retorna o index de uma transação
* neste vetor de booleanos que possui indexes correspondentes ao vetor de transações ativas.
*
* @param adjacencies Vetor com todas as transações ativas.
* @param id Identificador da transação a ser encontrada.
*
* @return int
*/
int visitedIndex(vector<node_conflict_t> adjacencies, int id) {
    int size = adjacencies.size();
    for (int i = 0; i < size; i++) {
        if (adjacencies[i].id == id)
            return i;
    }

    return 0;
    
}


/** @brief Função auxiliar à Depth First Search (DFS)
* @details Responsável pela recursão da DFS, está função permite identificar
* quando um nodo é visitado mais de uma vez dentro da mesma iteração, indicando
* a existência de um ciclo. Retorna verdadeiro caso o ciclo seja identificado.
*
* @param adjacencies Vetor com todas as transações ativas.
* @param visited Vetor com as posições já visitadas.
* @param currentIndex Index da transação corrente analisada pela DFS.
*
* @return bool
*/
bool isCyclicUtil(vector<node_conflict_t> adjacencies, vector<bool> &visited, int currentIndex) {
    if (visited[currentIndex])
        return true;
    
    visited[currentIndex] = true;

    bool flag = false;
    set<int>::iterator it;
    for(it = adjacencies[currentIndex].neighbours.begin(); it != adjacencies[currentIndex].neighbours.end(); it++) {
        int index = visitedIndex(adjacencies, (*it));
        flag = isCyclicUtil(adjacencies, visited, index);

        if (flag) {
            return true;
        }
    }

    visited[currentIndex] = false;

    return false;
}


/** @brief Depth First Search (DFS) para encontrar ciclos no grafo
* @details Utiliza-se neste momento o vetor neighbours da estrutura node_conflict_t
* como forma de identificar as transações adjacentes à transação corrente no grafo
* montado como forma de detectar ciclos. Ao final retorna se o grafo possui um ciclo ou não.
*
* @param adjacencies Vetor com todas as transações ativas.
*
* @return bool
*/
bool hasCycle(vector<node_conflict_t> adjacencies) {
    bool flag = false;
    int size = adjacencies.size();
    vector<bool> visited(adjacencies.size(), false);

    set<int>::iterator it;
    for (int i = 0; i < size; i++) {
        visited[i] = true;
    
        for(it = adjacencies[i].neighbours.begin(); it != adjacencies[i].neighbours.end(); it++) {
            int index = visitedIndex(adjacencies, (*it));
            flag = isCyclicUtil(adjacencies, visited, index);

            if (flag) {
                return true;
            }
        }

        visited[i] = false;
    }

    return false;
}
