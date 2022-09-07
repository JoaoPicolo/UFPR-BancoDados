#include "graph.hpp"

transaction_t createTransaction(
    int arrival_time, int identifier,
    char operation, char attribute
) {
    transaction_t tx;
    tx.time = arrival_time;
    tx.id = identifier;
    tx.operation.type = operation;
    tx.operation.attr = attribute;

    return tx;
}


nodo_t* findTx(int id, vector<nodo_t> &adj) {
    for (auto &tx: adj) {
        if (tx.id == id) {
            return &tx;
        }
    }

    return NULL;
}


nodo_t* createTx(transaction_t tx) {
    nodo_t* node = (nodo_t*)malloc(sizeof(nodo_t));
    node->id = tx.id;
    node->commit = false;
    node->visited = false;
    node->adjTx = {};

    return node;
}


bool transactionsClosed(vector<nodo_t> adj) {
    for (auto tx: adj) {
        if (tx.commit == false) {
            return false;
        }
    }

    return true;
}


bool updateAdj(vector<nodo_t> &adj, transaction_t tx) {
    nodo_t* node = findTx(tx.id, adj);
    if (node == NULL) {
        node = createTx(tx);
        adj.push_back(*node);
        node = findTx(tx.id, adj);
    }

    if (tx.operation.type == 'C' || tx.operation.type == 'c') {
        node->commit = true;
        return transactionsClosed(adj);
    }
    
    for (auto &item: adj) {
        //cout << "Compara " << tx.id << " com " << item.id << endl;
        if (item.id == tx.id) {
            //cout << "Adiciona operacao " << tx.operation.type << endl;
            item.operations.push_back(tx.operation);
        }
        else {
            //cout << "Verifica operacao" << endl;
            vector<operation_t> operations = item.operations;
            for (auto op: operations) {
                if (op.attr == tx.operation.attr) {
                    //cout << "Operacao eh " << op.type << " e " << tx.operation.type << endl;
                    if (
                        ((op.type == 'R' || op.type == 'r') && (tx.operation.type == 'W' || tx.operation.type == 'w')) ||
                        ((op.type == 'W' || op.type == 'w') && (tx.operation.type == 'R' || tx.operation.type == 'r')) ||
                        ((op.type == 'W' || op.type == 'w') && (tx.operation.type == 'W' || tx.operation.type == 'w'))
                    ) {
                        //cout << "Vai adicionar " << item.id << " em " << node->id << endl;
                        node->adjTx.insert(item.id);
                    }
                }
            }
        }
    }

    return false;
}


int index(vector<nodo_t> adj, int id) {

    int size = adj.size();
    for (int i = 0; i < size; i++) {
        if (adj[i].id == id)
            return i;
    }

    return 0;
    
}

bool isCyclicUtil(vector<nodo_t> adj, vector<bool> &visited, int current) {
    if (visited[current])
        return true;
    
    visited[current] = true;
    bool flag = false;

    set<int>::iterator it;

    for(it = adj[current].adjTx.begin(); it != adj[current].adjTx.end(); it++) {
        int currentIndex = index(adj, (*it));
        /* cout << "Trx: " << (*it) << " index: " << currentIndex << endl; */
        flag = isCyclicUtil(adj, visited, currentIndex);

        if (flag) {
            return true;
        }
    }

    return false;
}

bool hasCycle(vector<nodo_t> adj) {
    vector<bool> visited(adj.size(), false);
    set<int>::iterator it;

    bool flag = false;
    
    int size = adj.size();
    for (int i = 0; i < size; i++) {
        visited[i] = true;
    
        for(it = adj[i].adjTx.begin(); it != adj[i].adjTx.end(); it++) {
            int currentIndex = index(adj, (*it));
            /* cout << "Trx: " << (*it) << " index: " << currentIndex << endl; */
            flag = isCyclicUtil(adj, visited, currentIndex);

            if (flag) {
                return true;
            }
        }
        visited[i] = false;
    }

    return false;
}

int findTxVisao(int id, vector<nodo_visao_t> &adj) {
    int size = adj.size();
    for (int i = 0; i < size; i++) {
        if (adj[i].id == id) {
            return i;
        }
    }

    return -1;
}

void updateVisao(vector<nodo_visao_t> &arr, transaction_t tx) {
    int nodeIdx = findTxVisao(tx.id, arr);
    if (nodeIdx == -1) {
        nodo_visao_t node;
        node.id = tx.id;
        arr.push_back(node);
        nodeIdx = arr.size() - 1;
    }

    nodo_visao_t *aux = &arr[nodeIdx];
    aux->transactions.push_back(tx);
}

bool notWriteTx (vector<transaction_t> tx, int init) {

    int size = tx.size();

    for (int i = init; i < size; i++) {
        //cout << "posição: " << i << "tipo: " << tx[i].operation.type << endl;
        if (tx[i].operation.type == 'W' || tx[i].operation.type == 'w') {
            return true;
        }
    }

    return false;
}


bool validate(vector<transaction_t> arr1, vector<transaction_t> arr2) {
    int size1 = arr1.size();
    int size2 = arr2.size();

    for (int i = 0; i < size1; i++) {
        transaction_t tx1 = arr1[i];

        for (int j = 0; j < size2; j++) {
            transaction_t tx2 = arr2[j];

            if (tx1.operation.attr == tx2.operation.attr) {
                if ((tx1.operation.type == 'W' || tx1.operation.type == 'w') && (tx2.operation.type == 'R' || tx2.operation.type == 'r')) {
                    if (tx1.time > tx2.time) {
                        return false;
                    }
                }

                if ((tx1.operation.type == 'W' || tx1.operation.type == 'w') && (tx2.operation.type == 'W' || tx2.operation.type == 'w')) {
                    if (tx1.time < tx2.time && notWriteTx(arr1, i+1)) {
                        return true;
                    }
                    else return false;
                }
            }
        }
    }

    return true;
}

bool heapPermutation(vector<nodo_visao_t> &a, int size) {
    // if size becomes 1 then prints the obtained
    // permutation
    int isValid = true;
    int arraySize = a.size();

    if (size == 1) {
        arraySize = a.size();
        for (int i = 0; i < arraySize - 1; i++) {
            //cout << "Comparing " << a[i].id << " with " << a[i+1].id << ": ";
            isValid = validate(a[i].transactions, a[i+1].transactions);
            //cout << isValid << endl;
            if (isValid) {
                return true;
            }
        }

        return false;
    }
 
    for (int i = 0; i < size; i++) {
        isValid = heapPermutation(a, size - 1);
        if (isValid) {
            return true;
        }

        //cout << "not valid" << endl;
 
        // if size is odd, swap 0th i.e (first) and
        // (size-1)th i.e (last) element
        if (size % 2 == 1)
            swap(a[0], a[size - 1]);
 
        // If size is even, swap ith and
        // (size-1)th i.e (last) element
        else
            swap(a[i], a[size - 1]);
        
    }

    return false;

}


bool visaoEq(vector<nodo_visao_t> arr) {
    int size = arr.size();

    return heapPermutation(arr, size);
}
