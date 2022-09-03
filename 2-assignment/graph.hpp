#include <string>
#include <iostream>
#include <set>
#include <list>
#include <vector>

using namespace std;

struct operation_t {
    char type, attr;
};

struct transaction_t {
    int time, id;
    operation_t operation;
};

struct nodo_t {
    int id;
    bool commit;
    bool visited;
    vector<operation_t> operations;
    set<int> adjTx;
};

//struct attribute_t {
//    int idTx;
//    char attribute;
//};

transaction_t createTransaction(
    int arrival_time, int identifier,
    char operation, char attribute
);

//void appendAttr(
//    list<attribute_t> attrList,
//    int idTx,
//    char attr
//);

bool updateAdj(vector<nodo_t> &adj, transaction_t tx);

bool hasCycle(vector<nodo_t> adj);
