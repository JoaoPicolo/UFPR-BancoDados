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

//void appendAttr(list<attribute_t> attrList, int idTx, char attr) {
//    list<attribute_t>::iterator it;
//    attribute_t newAttr;
//
//    if(attrList.size() == 0) {
//        newAttr.idTx = idTx;
//        newAttr.attribute = attr;
//        attrList.push_back(newAttr);
//        return;
//    }
//
//    for(it = attrList.begin(); it != attrList.end(); (*it).attribute != attr)
//        it++;
//    
//    if (it == attrList.end()) {
//        newAttr.idTx = idTx;
//        newAttr.attribute = attr;
//        attrList.push_back(newAttr);
//    }
//    
//}


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

        /* if (!visited[i]) {
            if (isCyclic(i, visited, -1)) {
                return true;
            }
        } */
    }

    return false;
}
