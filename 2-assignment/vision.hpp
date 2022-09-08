#ifndef __VISION__
#define __VISION__

#include "utils.hpp"

struct attribute_t {
    int id;
    char attr;
};

struct node_vision_t {
    int id;
    vector<transaction_t> transactions;
};

void updateVision(vector<node_vision_t> &vision, transaction_t transaction);

void updateAttributes(vector<attribute_t> &attributes, transaction_t transaction);

bool isVisionEquivalent(vector<node_vision_t> vision, vector<attribute_t> attributes);

void printTransactionsList(vector<node_vision_t> &vision);

#endif
