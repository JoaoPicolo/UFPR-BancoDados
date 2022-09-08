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

void updateHelpers(helpers_t &helper, transaction_vision_t &transaction) {
    if (transaction.operation.type == 'R' || transaction.operation.type == 'r') {
        set<char>::iterator it;
        bool hasWritten = false;
        
        for(it = helper.writtenAttributes.begin(); it != helper.writtenAttributes.end(); it++) {
            if (transaction.operation.attr == (*it)) {
                hasWritten = true;
                break;
            }
        }

        if (!hasWritten) {
            transaction.isCleanRead = true;
        }
    }
    else if (transaction.operation.type == 'W' || transaction.operation.type == 'w') {
        helper.writtenAttributes.insert(transaction.operation.attr);
        helper.lastWriter[transaction.operation.attr] = transaction.id;
        helper.writters[transaction.operation.attr].insert(transaction.id);
    }
}

void updateVision(helpers_t &helper, vector<node_vision_t> &vision, transaction_t transaction) {
    int nodeIndex = findTransactionIndex(vision, transaction.id);

    if (nodeIndex == -1) {
        node_vision_t aux;
        aux.id = transaction.id;
        vision.push_back(aux);
        nodeIndex = vision.size() - 1;
    }

    node_vision_t *node = &vision[nodeIndex];

    transaction_vision_t newTransaction;
    newTransaction.id = transaction.id;
    newTransaction.time = transaction.time;
    newTransaction.isCleanRead = false;
    newTransaction.operation = transaction.operation;
    updateHelpers(helper, newTransaction);
    node->transactions.push_back(newTransaction);
}

bool isLastWrite(int startIndex, set<int>writters, vector<node_vision_t> vision) {
    int size = vision.size();
    set<int>::iterator it;

    for (int i = startIndex; i < size; i++) {
        node_vision_t current = vision[i];
        for(it = writters.begin(); it != writters.end(); it++) {
            if (current.id == (*it)) {
                return false;
            }
        }
        
    }

    return true;
}

bool validateWrite(helpers_t helper, vector<node_vision_t> vision) {
    int size = vision.size();

    for (int i = 0; i < size - 1; i++) {
        vector<transaction_vision_t> transactions = vision[i].transactions;
        int transactionsSize = transactions.size();

        for (int j = 0; j < transactionsSize; j++) {
            transaction_vision_t transaction = transactions[j];

            if (transaction.operation.type == 'W' || transaction.operation.type == 'w') {
                char attribute = transaction.operation.attr;
                if (helper.lastWriter[attribute] == transaction.id) {
                    bool isLast = isLastWrite(j+1, helper.writters[attribute], vision);
                    if (!isLast) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool validateCleanRead(int currentIndex, vector<node_vision_t> vision) {
    node_vision_t current = vision[currentIndex];
    vector<transaction_vision_t> transactions = current.transactions;

    int size = transactions.size();
    for (int i = 0; i < size; i++) {
        transaction_vision_t transaction = transactions[i];

        if (transaction.operation.type == 'R' || transaction.operation.type == 'r') {
            if (transaction.isCleanRead) {
                //cout << "Will validate " << transaction.time << " from " << transaction.id << endl;

                for (int j = currentIndex-1; j >= 0; j--) {
                    node_vision_t previous = vision[j];
                    vector<transaction_vision_t> previousTransactions = previous.transactions;

                    int previousSize = previousTransactions.size();
                    for (int k = 0; k < previousSize; k++) {
                        transaction_vision_t previousTransaction = previousTransactions[i];
                        if (previousTransaction.operation.type == 'W' || previousTransaction.operation.type == 'w') {
                            if (previousTransaction.operation.attr == transaction.operation.attr) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}


bool validateRead(vector<transaction_vision_t> fstTransactions, vector<transaction_vision_t> scdTransactions) {
    int fstSize = fstTransactions.size();
    int scdSize = scdTransactions.size();

    for (int i = 0; i < fstSize; i++) {
        transaction_vision_t fstTransaction = fstTransactions[i];

        cout << fstTransaction.id << " " << fstTransaction.time << " " << fstTransaction.operation.type << endl;

        for (int j = 0; j < scdSize; j++) {
            transaction_vision_t scdTransaction = scdTransactions[j];
            cout << "---> " << scdTransaction.id << " " << scdTransaction.time << " " << scdTransaction.operation.type << endl;

            if (fstTransaction.operation.attr == scdTransaction.operation.attr) {
                if (
                    (fstTransaction.operation.type == 'R' || fstTransaction.operation.type == 'r') &&
                    (scdTransaction.operation.type == 'W' || scdTransaction.operation.type == 'w')
                ) {
                    if (fstTransaction.time < scdTransaction.time) {
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

bool heapPermutation(helpers_t helper, vector<node_vision_t> &vision, int size) {
    if (size == 1) {

        for (auto v: vision) {
            cout << v.id << " ";
        }
        cout << endl;

        bool validWrite = validateWrite(helper, vision);
        if (validWrite) {
            cout << "Valid write" << endl;
            int realSize = vision.size();
            bool validClean = true;

            for (int i = 1; i < realSize; i++) {
                validClean = validClean && validateCleanRead(i, vision);
            }

            if (validClean) {
                bool validRead = true;
                cout << "Valid clean" << endl;
                for (int i = 0; i < realSize - 1; i++) {
                    bool isValid = validRead &&  validateRead(vision[i].transactions, vision[i+1].transactions);
                    if (isValid) {
                        return true;
                    }
                }
            }
        }

        return false;
    }
 
    for (int i = 0; i < size; i++) {
        bool isValid = heapPermutation(helper, vision, size-1);
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

bool isVisionEquivalent(helpers_t helper, vector<node_vision_t> vision) {
    int size = vision.size();
    return heapPermutation(helper, vision, size);
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

