#ifndef __VISION__
#define __VISION__

#include "utils.hpp"

/** @brief Estrutura na qual se guarda informações auxiliares ao teste de visão.
* @details Esta estutura armazena quais os atributos foram escritos entre todas as transções,
* para cada atributo escrito guarda-se também o identificador da última transação a escrever
* neste atributo e, por fim, para cada atributo escrito guarda-se quais as transações que
* escreveram nele.
*/
struct helpers_t {
    set<char> writtenAttributes;
    map<char, int> lastWriter;
    map<char, set<int>> writters;
};


/** @brief Estrutura na qual se guarda uma transação para o teste de visão.
* @details Esta estutura armazena o id da transação realizada, caso ela seja de leitura
* armazena se ela foi lida sem nenhuma escrita anterior e armazena por fim a operação realizada.
*/
struct transaction_vision_t {
    int id, time;
    bool isCleanRead;
    operation_t operation;
};

/** @brief Estrutura na qual se guarda um nodo do teste de visão.
* @details Esta estutura armazena o id da transação realizada, e todas as transações
* realizadas com o mesmo id.
*/
struct node_vision_t {
    int id;
    vector<transaction_vision_t> transactions;
};

void updateVision(helpers_t &helper, vector<node_vision_t> &vision, transaction_t transaction);

bool isVisionEquivalent(helpers_t helper, vector<node_vision_t> vision);

void printTransactionsList(vector<node_vision_t> &vision);

#endif
