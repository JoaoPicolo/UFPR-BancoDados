#ifndef __UTILS__
#define __UTILS__

#include <bits/stdc++.h>

using namespace std;

struct operation_t {
    char type, attr;
};

struct transaction_t {
    int time, id;
    operation_t operation;
};

transaction_t createTransaction(
    int arrival_time, int identifier,
    char operation, char attribute
);


#endif
