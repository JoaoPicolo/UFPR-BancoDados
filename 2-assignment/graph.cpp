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


nodo_t findTx(int id, vector<nodo_t> &adj) {
    nodo_t node;
    cout << "Criou " << &node << endl;
    node.id = -1;

    for (auto tx: adj) {
        if (tx.id == id) {
            cout << "Achou " << &tx << endl;
            return tx;
        }
    }

    return node;
}


nodo_t createTx(transaction_t tx) {
    nodo_t node;
    node.id = tx.id;
    node.commit = false;

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


void updateAdj(vector<nodo_t> &adj, transaction_t tx) {
    nodo_t node = findTx(tx.id, adj);
    if (node.id == -1) {
        node = createTx(tx);
        adj.push_back(node);
    }
    cout << "Endereco usado eh " << &node << endl;

    if (tx.operation.type == 'C') {
        node.commit = true;
        if (transactionsClosed(adj)) {
            cout << "Transacoes acabaram" << endl;
        }
        else {
            cout << "Transacoes nao acabaram" << endl;
        }

        return;
    }
    
    for (auto &item: adj) {
        cout << "Compara " << tx.id << " com " << item.id << endl;
        if (item.id == tx.id) {
            cout << "Adiciona operacao " << tx.operation.type << endl;
            item.operations.push_back(tx.operation);
        }
        else {
            cout << "Verifica operacao" << endl;
            vector<operation_t> operations = item.operations;
            for (auto op: operations) {
                if (op.attr == tx.operation.attr) {
                    cout << "Operacao eh " << op.type << " e " << tx.operation.type << endl;
                    if (
                        (op.type == 'R' && tx.operation.type == 'W') ||
                        (op.type == 'W' && tx.operation.type == 'R') ||
                        (op.type == 'W' && tx.operation.type == 'W')
                    ) {
                        cout << "Vai adicionar " << item.id << " em " << node.id << endl;
                        node.adjTx.push_back(item.id);
                    }
                }
            }
        }
    }
}