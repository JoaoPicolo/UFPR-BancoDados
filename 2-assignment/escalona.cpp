#include <bits/stdc++.h>
#include "graph.hpp"


int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int arrival_time, identifier;
    char operation, attribute;


    // Lista de adjacência para construção do grafo
    vector<nodo_t> adj;
    vector<nodo_visao_t> visao;
    vector<attribut_t> attributes;

    int count = 1;
    while (scanf("%d %d %c %c", &arrival_time, &identifier, &operation, &attribute) != EOF) {
        transaction_t tx = createTransaction(arrival_time, identifier, operation, attribute);
        updateAttr(attributes, tx);
        bool finished = updateAdj(adj, tx);
        updateVisao(visao, tx);

        if (finished) {
            cout << count << " ";

            vector<int> idsTx;
            int size = visao.size();
            for (int i = 0; i < size; i++) {
                idsTx.push_back(visao[i].id);
            }


            // for (auto a: adj) {
            //     cout << a.id << ": ";
            //     set<int> l = a.adjTx;
            //     for (auto it = l.begin(); it != l.end(); ++it)
            //         cout << ' ' << *it;
            //     cout << endl;
            // }


            sort(idsTx.begin(), idsTx.end());
            for (int i = 0; i < size; i++) {
                cout << idsTx[i];

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

            if (visaoEq(visao, attributes))  {
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