#include "conflict.hpp"

int findTransactionIndex(vector<node_conflict_t> &adjacencies, int id) {
    int size = adjacencies.size();

    for (int i = 0; i< size; i++) {
        if (adjacencies[i].id == id) {
            return i;
        }
    }

    return -1;
}

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

bool transactionsClosed(vector<node_conflict_t> adjacencies) {
    for (auto transaction: adjacencies) {
        if (transaction.commit == false) {
            return false;
        }
    }

    return true;
}

int visitedIndex(vector<node_conflict_t> adjacencies, int id) {
    int size = adjacencies.size();
    for (int i = 0; i < size; i++) {
        if (adjacencies[i].id == id)
            return i;
    }

    return 0;
    
}

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
