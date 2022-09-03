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
    vector<nodo_t> adj;
    int count = 1;
    while (scanf("%d %d %c %c", &arrival_time, &identifier, &operation, &attribute) != EOF) {
        //cout << "\nLeu novo" << endl;
        transaction_t tx = createTransaction(arrival_time, identifier, operation, attribute);
        bool finished = updateAdj(adj, tx);

        if (finished) {
            cout << count << " ";

            int size = adj.size();
            for (int i = 0; i < size; i++) {
                cout << adj[i].id;

                if (i < size - 1) {
                    cout << ",";
                }
            }

            if (hasCycle(adj)) {
                cout << " NS ";
            }
            else {
                cout << " SS ";
            }
            
            cout << endl;

            count++;
            adj.clear();
        }
    }

    //for (auto ad: adj) {
    //    cout << ad.id << ": ";
    //    vector<int> adjss = ad.adjTx;
    //    for (auto aa: adjss) {
    //        cout << aa << " ";
    //    }
    //    cout << endl;
    //}
}