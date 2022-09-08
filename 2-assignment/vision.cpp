#include "vision.hpp"

int findTransactionIndex(vector<node_vision_t> &vision, int id) {
    int size = vision.size();
    for (int i = 0; i < size; i++) {
        if (vision[i].id == id) {
            return i;
        }
    }

    return -1;
}

void updateVision(vector<node_vision_t> &vision, transaction_t transaction) {
    int nodeIndex = findTransactionIndex(vision, transaction.id);

    if (nodeIndex == -1) {
        node_vision_t aux;
        aux.id = transaction.id;
        vision.push_back(aux);
        nodeIndex = vision.size() - 1;
    }

    node_vision_t *node = &vision[nodeIndex];
    node->transactions.push_back(transaction);
}

void updateAttributes(vector<attribute_t> &attributes, transaction_t transaction) {
    if (transaction.operation.type != 'W' && transaction.operation.type != 'w') {
        return;
    }

    attribute_t aux;
    if (attributes.size() == 0) {
        aux.id = transaction.id;
        aux.attr = transaction.operation.attr;
        attributes.push_back(aux);
        return;
    }

    bool find = false;
    for (auto &attribute: attributes) {
        if (attribute.attr == transaction.operation.attr) {
            attribute.id = transaction.id;
            find = true;
            return;
        }
    }

    if (!find) {
        aux.id = transaction.id;
        aux.attr = transaction.operation.attr;
        attributes.push_back(aux);
    }
}

bool notWriteTx(vector<attribute_t> attr, int txId, char attrTx) {
    for (auto attribute: attr) {
        if (attribute.attr == attrTx) {
            return (attribute.id == txId);
        }
    }

    return false;
}


bool validate(vector<attribute_t> attributes, vector<transaction_t> fstTransactions, vector<transaction_t> scdTransactions) {
    int fstSize = fstTransactions.size();
    int scdSize = scdTransactions.size();

    for (int i = 0; i < fstSize; i++) {
        transaction_t fstTransaction = fstTransactions[i];

        for (int j = 0; j < scdSize; j++) {
            transaction_t scdTransaction = scdTransactions[j];

            if (fstTransaction.operation.attr == scdTransaction.operation.attr) {
                if (
                    (fstTransaction.operation.type == 'W' || fstTransaction.operation.type == 'w') &&
                    (scdTransaction.operation.type == 'R' || scdTransaction.operation.type == 'r')
                ) {
                    if (fstTransaction.time < scdTransaction.time) {
                        return true;
                    }
                    else {
                        return false;
                    }
                }

                if (
                    (fstTransaction.operation.type == 'W' || fstTransaction.operation.type == 'w') &&
                    (scdTransaction.operation.type == 'W' || scdTransaction.operation.type == 'w')
                ) {
                    if (
                        (fstTransaction.time < scdTransaction.time) &&
                        notWriteTx(attributes, scdTransaction.id, fstTransaction.operation.attr)
                    ) {
                        return true;
                    }
                    else {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool heapPermutation(vector<attribute_t> attributes, vector<node_vision_t> &vision, int size) {
    if (size == 1) {
        int realSize = vision.size();
        for (int i = 0; i < realSize - 1; i++) {
            bool isValid = validate(attributes, vision[i].transactions, vision[i+1].transactions);
            if (isValid) {
                return true;
            }
        }

        return false;
    }
 
    for (int i = 0; i < size; i++) {
        bool isValid = heapPermutation(attributes, vision, size-1);
        if (isValid) {
            return true;
        }

        if (size % 2 == 1) {
            swap(vision[0], vision[size - 1]);
        }
        else {
            swap(vision[i], vision[size - 1]);
        }
    }

    return false;

}


bool isVisionEquivalent(vector<node_vision_t> vision, vector<attribute_t> attributes) {
    int size = vision.size();
    return heapPermutation(attributes, vision, size);
}

void printTransactionsList(vector<node_vision_t> &vision) {
    vector<int> identifiers;
    int size = vision.size();

    for (int i = 0; i < size; i++) {
        identifiers.push_back(vision[i].id);
    }


    sort(identifiers.begin(), identifiers.end());
    for (int i = 0; i < size; i++) {
        cout << identifiers[i];

        if (i < size - 1) {
            cout << ",";
        }
    }
}

