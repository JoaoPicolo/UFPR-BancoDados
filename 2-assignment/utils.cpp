#include "utils.hpp"


/** @brief Cria de uma nova transação.
* @details Essa função recebe os parâmetros lidos da entrada padrão
* e os armazena na estrutura designada.
*
* @param arrival_time Tempo de entrada de uma operação.
* @param identifier Identificador relativo a transação à qual a operação pertence.
* @param operation Tipo da operação: leitura (R), escrita (W) ou commit (C).
* @param atribbute Atributo sob o qual a operação está sendo realizada.
*
* @return transaction_t
*/
transaction_t createTransaction(
    int arrival_time, int identifier,
    char operation, char attribute
) {
    transaction_t tx;
    tx.time = arrival_time;
    tx.id = identifier;
    tx.operation.type = operation;
    tx.operation.attr = attribute;

    return tx;
}
