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