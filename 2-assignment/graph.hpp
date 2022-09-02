#include <string>
#include <iostream>

using namespace std;

struct transaction_t {
    int arrival_time, identifier;
    char operation, attribute;
};

transaction_t createTransaction(
    int arrival_time, int identifier,
    char operation, char attribute
);
