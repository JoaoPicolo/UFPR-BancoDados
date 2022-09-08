#include "vision.hpp"


/** @brief Retorna o index de uma transação no vetor de transações.
* @details A partir de um id de uma transação, esta função é responsável por retornar
* o index da transação correspondente no vetor de transações já criado.
*
* @param vision Vetor com todas as transações ativas.
* @param id Identificador da transação a ser encontrada.
*
* @return int
*/
int findTransactionIndex(vector<node_vision_t> &vision, int id) {
    int size = vision.size();
    for (int i = 0; i < size; i++) {
        if (vision[i].id == id) {
            return i;
        }
    }

    return -1;
}


/** @brief Atualiza as estruturas auxiliares ao teste de visão.
* @details Ao receber uma nova transação, verifica se a operação é do tipo leitura
* e, caso seja, verifica se é uma leitura limpa (sem escrita anterior sob o mesmo atributo).
* Caso seja do tipo escrita atualiza as variáveis auxiliares descritas no arquivo header.
*
* @param helper Variável com as estruturas auxiliares ao teste de visão.
* @param transaction Nova transação lida.
*
* @return void
*/
void updateHelpers(helpers_t &helper, transaction_vision_t &transaction) {
    if (transaction.operation.type == 'R' || transaction.operation.type == 'r') {
        set<char>::iterator it;
        bool hasWritten = false;
        
        for(it = helper.writtenAttributes.begin(); it != helper.writtenAttributes.end(); it++) {
            if (transaction.operation.attr == (*it)) {
                hasWritten = true;
                break;
            }
        }

        if (!hasWritten) {
            transaction.isCleanRead = true;
        }
    }
    else if (transaction.operation.type == 'W' || transaction.operation.type == 'w') {
        helper.writtenAttributes.insert(transaction.operation.attr);
        helper.lastWriter[transaction.operation.attr] = transaction.id;
        helper.writters[transaction.operation.attr].insert(transaction.id);
    }
}


/** @brief Atualiza o vetor de transações lidas.
* @details Ao receber uma nova transação, um nodo correspondente é criado no vetor
* de transações caso não exista e atualizado caso exista. Ainda é responsável pela
* atualização das estruturas auxiliares ao teste de visão uma vez que a construção
* dessas estruturas é feita conforme lê-se a entrada.
*
* @param helper Variável com as estruturas auxiliares ao teste de visão.
* @param vision Vetor com todas as transações ativas.
* @param transaction Nova transação lida.
*
* @return void
*/
void updateVision(helpers_t &helper, vector<node_vision_t> &vision, transaction_t transaction) {
    int nodeIndex = findTransactionIndex(vision, transaction.id);

    if (nodeIndex == -1) {
        node_vision_t aux;
        aux.id = transaction.id;
        vision.push_back(aux);
        nodeIndex = vision.size() - 1;
    }

    node_vision_t *node = &vision[nodeIndex];

    transaction_vision_t newTransaction;
    newTransaction.id = transaction.id;
    newTransaction.time = transaction.time;
    newTransaction.isCleanRead = false;
    newTransaction.operation = transaction.operation;
    updateHelpers(helper, newTransaction);
    node->transactions.push_back(newTransaction);
}


/** @brief Indica se a escrita indicada é a última.
* @details Verifica se uma transação subsequente à transação que invoca esta função
* possui uma escita sob o mesmo atributo. Caso possua returna falso, indicando que
* a transação responsável pela invocação não é a última a escrever no atributo correspondente.
*
* @param startIndex Index sob o qual deverá iniciar a busca por transações subsequentes.
* @param id Identificador da transação responsável por invocar esta chamada.
* @param writter Conjunto de transações que escreveram sob o tributo encontrado na transação invocadora.
* @param vision Vetor perrmutado com todas as transações ativas.
*
* @return bool
*/
bool isLastWrite(int startIndex, int id, set<int>writters, vector<node_vision_t> vision) {
    int size = vision.size();
    set<int>::iterator it;

    for (int i = startIndex; i < size; i++) {
        node_vision_t current = vision[i];
        for(it = writters.begin(); it != writters.end(); it++) {
            if ((current.id == (*it)) && (id != (*it))) {
                return false;
            }
        }
        
    }

    return true;
}


/** @brief Indica se a escrita sobre um atributo é válida.
* @details Para cada conjunto de transações (posição do vetor de ativas)
* percorre cada transação até encontrar uma operação de leitura. Utiliza-se as
* estruturas auxiliares para saber se a transação selecionada foi a última e escrever
* sob um atributo. Retorna true caso nenhuma transação seguinte escreva sob o mesmo atributo.
*
* @param helper Variável com as estruturas auxiliares ao teste de visão.
* @param vision Vetor permutado com todas as transações ativas.
*
* @return bool
*/
bool validateWrite(helpers_t helper, vector<node_vision_t> vision) {
    int size = vision.size();

    for (int i = 0; i < size - 1; i++) {
        vector<transaction_vision_t> transactions = vision[i].transactions;
        int transactionsSize = transactions.size();
        for (int j = 0; j < transactionsSize; j++) {
            transaction_vision_t transaction = transactions[j];

            if (transaction.operation.type == 'W' || transaction.operation.type == 'w') {
                char attribute = transaction.operation.attr;
                if (helper.lastWriter[attribute] == transaction.id) {
                    int index = findTransactionIndex(vision, transaction.id);
                    bool isLast = isLastWrite(index+1, transaction.id, helper.writters[attribute], vision);
                    if (!isLast) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}


/** @brief Indica se uma leitura limpa permanece limpa.
* @details Para cada conjunto de transações procura por uma leitura limpa, isto é,
* uma leitura feita sem a antecedência de uma escrita sob o mesmo atributo. Caso encontre
* percorre todas as transações antecessoras no vetor permutado com a finalidade de garantir
* que a leitura continua limpa e retorna verdadeiro neste caso.
*
* @param currentIndex Index da transação invocadora no vetor de transações ativas.
* @param vision Vetor permutado com todas as transações ativas.
*
* @return bool
*/
bool validateCleanRead(int currentIndex, vector<node_vision_t> vision) {
    node_vision_t current = vision[currentIndex];
    vector<transaction_vision_t> transactions = current.transactions;

    int size = transactions.size();
    for (int i = 0; i < size; i++) {
        transaction_vision_t transaction = transactions[i];

        if (transaction.operation.type == 'R' || transaction.operation.type == 'r') {
            if (transaction.isCleanRead) {
                for (int j = currentIndex-1; j >= 0; j--) {
                    node_vision_t previous = vision[j];
                    vector<transaction_vision_t> previousTransactions = previous.transactions;

                    int previousSize = previousTransactions.size();
                    for (int k = 0; k < previousSize; k++) {
                        transaction_vision_t previousTransaction = previousTransactions[k];
                        if (previousTransaction.operation.type == 'W' || previousTransaction.operation.type == 'w') {
                            if (previousTransaction.operation.attr == transaction.operation.attr) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}


/** @brief Indica se as leituras feitas são válidas.
* @details Para cada conjunto de transações procura por uma escrita. Caso encontre percorre
* todas as transações antecessoras no vetor permutado com a finalidade de garantir que qualquer
* leitura feita sob o mesmo atributo não devesia ser sucerro à escrita feita. Caso encontre
* uma leitura que se enquadra no descrito, retorna false.
*
* @param currentIndex Index da transação invocadora no vetor de transações ativas.
* @param vision Vetor permutado com todas as transações ativas.
*
* @return bool
*/
bool validateRead(int currentIndex, vector<node_vision_t> vision) {
    node_vision_t current = vision[currentIndex];
    vector<transaction_vision_t> transactions = current.transactions;

    int size = transactions.size();
    for (int i = 0; i < size; i++) {
        transaction_vision_t transaction = transactions[i];

        if (transaction.operation.type == 'W' || transaction.operation.type == 'w') {
            for (int j = currentIndex-1; j >= 0; j--) {
                node_vision_t previous = vision[j];
                vector<transaction_vision_t> previousTransactions = previous.transactions;

                int previousSize = previousTransactions.size();
                for (int k = 0; k < previousSize; k++) {
                    transaction_vision_t previousTransaction = previousTransactions[k];
                    if (previousTransaction.operation.type == 'R' || previousTransaction.operation.type == 'r') {
                        if (previousTransaction.operation.attr == transaction.operation.attr) {
                            if (previousTransaction.time > transaction.time) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}

/** @brief Responsável por gerar as permutações possíveis.
* @details Algoritmo de Heap responsável por gerar todas as permutações
* possíveis a partir do vetor de entrada. Ao obter uma nova permutação,
* ela é testada de acordo com as especifiações relativas ao teste por visão.
* Retorna verdadeiro caso uma sequência serial válida seja encontrada.
*
* @param helper Variável com as estruturas auxiliares ao teste de visão.
* @param vision Vetor com todas as transações ativa que sofrerá permutações.
* @param size Tamanho do vetor de transações ativas a ser permutado.
*
* @return bool
*/
bool heapPermutation(helpers_t helper, vector<node_vision_t> &vision, int size) {
    if (size == 1) {
        bool validWrite = validateWrite(helper, vision);
        if (validWrite) {
            int realSize = vision.size();
            bool validClean = true;

            for (int i = 1; i < realSize; i++) {
                validClean = validClean && validateCleanRead(i, vision);
            }

            if (validClean) {
                bool validRead = true;
                for (int i = 1; i < realSize; i++) {
                    validRead = validRead &&  validateRead(i, vision);
                }

                if (validRead) {
                    return true;
                }
            }
        }

        return false;
    }
 
    for (int i = 0; i < size; i++) {
        bool isValid = heapPermutation(helper, vision, size-1);
        if (isValid) {
            return true;
        }

        if (size % 2 == 1) {
            swap(vision[0], vision[size - 1]);
        }
        else {
            swap(vision[i], vision[size - 1]);
        }
    }

    return false;

}

/** @brief Responsável por indicar se as transações ativas são equivalentes.
* @details Uma vez que todas as transações estão em um vetor no qual cada posição
* contém todas as operações realizadas pela transação correspondente esta função
* utiliza o algoritmo de Heap para analisar todas as permutações possíveis.
* Caso uma sequência serial valida seja encontrada, retorna-se true.
*
* @param helper Variável com as estruturas auxiliares ao teste de visão.
* @param vision Vetor com todas as transações ativas.
*
* @return bool
*/
bool isVisionEquivalent(helpers_t helper, vector<node_vision_t> vision) {
    int size = vision.size();
    return heapPermutation(helper, vision, size);
}


/** @brief Imprime as transações ativas em ordem crescente
* @details Cria uma cópia com somente os identificadores presentes no vetor
* de transações ativas, os ordena de forma crescente e imprime no stdout.
*
* @param vision Vetor com todas as transações ativas.
*
* @return void
*/
void printTransactionsList(vector<node_vision_t> &vision) {
    vector<int> identifiers;
    int size = vision.size();

    for (int i = 0; i < size; i++) {
        identifiers.push_back(vision[i].id);
    }


    sort(identifiers.begin(), identifiers.end());
    for (int i = 0; i < size; i++) {
        cout << identifiers[i];

        if (i < size - 1) {
            cout << ",";
        }
    }
}

