#include "graph.hpp"

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int arrival_time, identifier;
    char operation, attribute;


    // Lista de adjacência para construção do grafo
    vector<nodo_t> adj;
    vector<nodo_visao_t> visao;
    int count = 1;
    while (scanf("%d %d %c %c", &arrival_time, &identifier, &operation, &attribute) != EOF) {
        transaction_t tx = createTransaction(arrival_time, identifier, operation, attribute);
        bool finished = updateAdj(adj, tx);
        updateVisao(visao, tx);

        if (finished) {
            // cout << count << " ";

            // int size = adj.size();
            // for (int i = 0; i < size; i++) {
            //     cout << adj[i].id;

            //     if (i < size - 1) {
            //         cout << ",";
            //     }
            // }

            // if (hasCycle(adj)) {
            //     cout << " NS ";
            // }
            // else {
            //     cout << " SS ";
            // }

            if (visaoEq(visao))  {
                cout << "SV";
            }
            else {
                cout << "NV";
            }
            
            cout << endl;

            count++;
            adj.clear();
            visao.clear();
        }
    }
}