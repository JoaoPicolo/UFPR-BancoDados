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

    if (tx.operation.type == 'C') {
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
                        (op.type == 'R' && tx.operation.type == 'W') ||
                        (op.type == 'W' && tx.operation.type == 'R') ||
                        (op.type == 'W' && tx.operation.type == 'W')
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

nodo_visao_t* findTxVisao(int id, vector<nodo_visao_t> &adj) {
    if (adj.size() == 0) {
        return NULL;
    }

    for (auto &tx: adj) {
        if (tx.id == id) {
            return &tx;
        }
    }

    return NULL;
}

nodo_visao_t* createTxVisao(transaction_t tx) {
    nodo_visao_t* node = (nodo_visao_t*)malloc(sizeof(nodo_visao_t));
    node->id = tx.id;

    return node;
}

void updateVisao(vector<nodo_visao_t> &arr, transaction_t tx) {
    nodo_visao_t* node = findTxVisao(tx.id, arr);
    if (node == NULL) {
        node = createTxVisao(tx);
        arr.push_back(*node);
        node = findTxVisao(tx.id, arr);
    }

    node->transactions.push_back(tx);
}


bool validate(vector<transaction_t> arr1, vector<transaction_t> arr2) {
    int size1 = arr1.size();
    int size2 = arr2.size();

    for (int i = 0; i < size1; i++) {
        transaction_t tx1 = arr1[i];

        for (int j = 0; j < size2; j++) {
            transaction_t tx2 = arr1[j];

            if (tx1.operation.attr == tx2.operation.attr) {
                if (tx1.operation.type == 'W' && tx2.operation.type == 'R') {
                    if (tx1.time > tx2.time) {
                        return false;
                    }
                }

                if (tx1.operation.type == 'W' && tx2.operation.type == 'W') {
                    if (tx1.time > tx2.time) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}


bool permuteArrays(vector<nodo_visao_t> arr, int first) {
    nodo_visao_t elem = arr[0];
    arr[0] = arr[first];
    arr[first] = elem;

    int size = arr.size();
    for (int i = 0; i < size - 1; i++) { // Defines a first array
        vector<transaction_t> fstTxs = arr[i].transactions;
        for (int j = i+1; j < size; j++) { // Defines a second array to compare
            vector<transaction_t> scdTxs = arr[j].transactions;
            bool isValid = validate(fstTxs, scdTxs);
            
            if (!isValid) {
                return false;
            }
        }
    }

    return true;
}

void heapPermutation(vector<nodo_visao_t> &a, int size) {
    // if size becomes 1 then prints the obtained
    // permutation
    if (size == 1) {
        for(auto k: a) {
            cout << k.id << " ";
        }
        cout << endl;
        return;
    }
 
    for (int i = 0; i < size; i++) {
        heapPermutation(a, size - 1);
 
        // if size is odd, swap 0th i.e (first) and
        // (size-1)th i.e (last) element
        if (size % 2 == 1)
            swap(a[0], a[size - 1]);
 
        // If size is even, swap ith and
        // (size-1)th i.e (last) element
        else
            swap(a[i], a[size - 1]);
    }
}


bool visaoEq(vector<nodo_visao_t> arr) {
    int size = arr.size();

    heapPermutation(arr, size);

    return true;
}
