#ifndef __CONFLICT__
#define __CONFLICT__

#include "utils.hpp"

/** @brief Estrutura na qual se guarda um nodo do teste de conflito.
* @details Esta estutura armazena o id da transação realizada, o estado dela
* (commitada ou ativa), se ela já foi visitada (utilizado para a recursão),
* as operações que a transação com o id corresponde já realizou, além dos vértices adjacentes
* à transação correspondente no grafo formado.
*/
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
