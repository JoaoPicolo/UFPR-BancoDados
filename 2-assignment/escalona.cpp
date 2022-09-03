#include "graph.hpp"

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int arrival_time, identifier;
    char operation, attribute;

    //std::set<int> txId;
    // Lista de atributos e a respectiva transação que o mantém
    //std::list<attribute_t> attr;

    // Lista de adjacência para construção do grafo
    std::list<nodo_t> adj;

    while (scanf("%d %d %c %c", &arrival_time, &identifier, &operation, &attribute) != EOF) {
        transaction_t tx = createTransaction(arrival_time, identifier, operation, attribute);
        cout << tx.arrival_time << " " << tx.identifier << " " << tx.operation << " " << tx.attribute << endl;

        //appendAttr(attr, identifier, attribute);
    }
}