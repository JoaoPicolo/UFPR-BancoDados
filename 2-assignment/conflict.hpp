#ifndef __CONFLICT__
#define __CONFLICT__

#include "utils.hpp"

struct node_conflict_t {
    int id;
    bool commit;
    bool visited;
    vector<operation_t> operations;
    set<int> neighbours;
};

void updateConflict(vector<node_conflict_t> &adjacencies, transaction_t transaction);

bool transactionsClosed(vector<node_conflict_t> adjacencies);

bool hasCycle(vector<node_conflict_t> adjacencies);

#endif
