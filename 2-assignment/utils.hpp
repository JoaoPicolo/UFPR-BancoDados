#ifndef __UTILS__
#define __UTILS__

#include <set>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

/** @brief Estrutura na qual se guarda uma operação.
* @details Esta estutura armazena o tipo da operação e
* o atributo sob a qual ela está sendo realizada.
*/
struct operation_t {
    char type, attr;
};

/** @brief Estrutura na qual se guarda uma transação.
* @details Esta estutura armazena a operação realizada,
* bem como o tempo de entrada da operação e o id da transação feita
*/
struct transaction_t {
    int time, id;
    operation_t operation;
};

transaction_t createTransaction(
    int arrival_time, int identifier,
    char operation, char attribute
);


#endif
