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

struct attribut_t {
    char attr;
    int id;
};

struct nodo_t {
    int id;
    bool commit;
    bool visited;
    vector<operation_t> operations;
    set<int> adjTx;
};

struct nodo_visao_t {
    int id;
    vector<transaction_t> transactions;
};

transaction_t createTransaction(
    int arrival_time, int identifier,
    char operation, char attribute
);


void updateAttr(vector<attribut_t> &attr, transaction_t tx);

bool updateAdj(vector<nodo_t> &adj, transaction_t tx);

bool hasCycle(vector<nodo_t> adj);

void updateVisao(vector<nodo_visao_t> &arr, transaction_t tx);

bool visaoEq(vector<nodo_visao_t> arr, vector<attribut_t> attributes);

