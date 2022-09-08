#ifndef __VISION__
#define __VISION__

#include "utils.hpp"

struct helpers_t {
    set<char> writtenAttributes;
    map<char, int> lastWriter;
    map<char, set<int>> writters;
};

struct transaction_vision_t {
    int id, time;
    bool isCleanRead;
    operation_t operation;
};

struct node_vision_t {
    int id;
    vector<transaction_vision_t> transactions;
};

void updateVision(helpers_t &helper, vector<node_vision_t> &vision, transaction_t transaction);

bool isVisionEquivalent(helpers_t helper, vector<node_vision_t> vision);

void printTransactionsList(vector<node_vision_t> &vision);

#endif
