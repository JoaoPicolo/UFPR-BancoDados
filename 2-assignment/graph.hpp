#include <string>
#include <iostream>
#include <set>
#include <list>

using namespace std;

struct transaction_t {
    int arrival_time, identifier;
    char operation, attribute;
};

struct attribute_t {
    int idTx;
    char attribute;
};

struct nodo_t {
    int id;
    char op;
    char attr;
    int commited;
    std::list<int> adjList;
};

transaction_t createTransaction(
    int arrival_time, int identifier,
    char operation, char attribute
);

void appendAttr(
    list<attribute_t> attrList,
    int idTx,
    char attr
);
