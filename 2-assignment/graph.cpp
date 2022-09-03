#include "graph.hpp"

transaction_t createTransaction(
    int arrival_time, int identifier,
    char operation, char attribute
) {
    transaction_t tx;
    tx.arrival_time = arrival_time;
    tx.identifier = identifier;
    tx.operation = operation;
    tx.attribute = attribute;

    return tx;
}

void appendAttr(list<attribute_t> attrList, int idTx, char attr) {
    list<attribute_t>::iterator it;
    attribute_t newAttr;

    if(attrList.size() == 0) {
        newAttr.idTx = idTx;
        newAttr.attribute = attr;
        attrList.push_back(newAttr);
        return;
    }

    for(it = attrList.begin(); it != attrList.end(); (*it).attribute != attr)
        it++;
    
    if (it == attrList.end()) {
        newAttr.idTx = idTx;
        newAttr.attribute = attr;
        attrList.push_back(newAttr);
    }
    
}

void updateAdj (list<nodo_t> adj, int idTx, char operation, char attribute) {

    if (operation == 'C') {

    }

    nodo_t newTrx;
    newTrx.id = idTx;
    newTrx.op = operation;
    newTrx.attr = attribute;
    
    
    if (adj.size() == 0) {
        adj.push_back(newTrx);
        return;
    }

    list<nodo_t>::iterator it;

    for (it = adj.begin(); it != adj.end(); it++) {

        if (idTx != (*it).id && attribute == (*it).attr) {
            if (operation == 'R' && (*it).op == 'W' || operation == 'W' && (*it).op == 'R' || operation == 'W' && (*it).op == 'W') {
                // adiciona na lista de adjacencia do nodo newTrx
                // atualiza o atributo do nodo it para 'none'
                return;
            }

        }
    
        
    }




}