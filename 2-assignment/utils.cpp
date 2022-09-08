#include "utils.hpp"

/** @brief Função responsável pela criação de uma nova transação.
* @details Essa função recebe os parametros lidos da entrada padrao
* e os armazena na estrutura designada.
*
* @param arrival_time Tempo de entrada de uma operacao
* @param identifier Identificador relativo a transacao a qual a operacao pertence
* @param operation Tipo da operacao: leitura (R), escrita (W) ou commit (C)
* @param atribbute Atributo sob o qual a operacao esta sendo realizada
* @return bool
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
