#include "utils.hpp"
#include "vision.hpp"
#include "conflict.hpp"

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    int arrival_time, identifier;
    char operation, attribute;


    helpers_t helper;
    vector<node_vision_t> vision;
    vector<node_conflict_t> adjacencies;

    int counter = 1;
    bool endedTransactions = false;
    while (scanf("%d %d %c %c", &arrival_time, &identifier, &operation, &attribute) != EOF) {
        transaction_t transaction = createTransaction(arrival_time, identifier, operation, attribute);

        updateConflict(adjacencies, transaction);
        updateVision(helper, vision, transaction);

        if ((operation == 'C') || (operation == 'c')) {
            endedTransactions = transactionsClosed(adjacencies);
        }

        if (endedTransactions) {
            cout << counter << " ";

            printTransactionsList(vision);

            if (!hasCycle(adjacencies)) {
                cout << " SS ";
            }
            else {
                cout << " NS ";
            }

            // cout << "Last writer:" << endl;
            // for (auto l: helper.lastWriter) {
            //     cout << l.first << ": " << l.second << endl;
            // }

            // cout << "Writters:" << endl;
            // for (auto l: helper.writters) {
            //     cout << l.first << ": ";
            //     set<int> writters = l.second;

            //     set<int>::iterator it;
            //     for(it = writters.begin(); it != writters.end(); it++) {
            //         cout << (*it) << " ";
            //     }
            //     cout << endl;
            // }

            cout << "Transactions" << endl;
            for (auto v: vision) {
                cout << v.id << endl;
                vector<transaction_vision_t> trans = v.transactions;
                for (auto t: trans) {
                    cout << t.time << " " << t.isCleanRead << endl;
                }
            }

            if (isVisionEquivalent(helper, vision))  {
                cout << "SV";
            }
            else {
                cout << "NV";
            }
            
            cout << endl;

            counter++;
            endedTransactions = false;

            vision.clear();
            adjacencies.clear();

            helper.writters.clear();
            helper.lastWriter.clear();
            helper.writtenAttributes.clear();
        }
    }
}