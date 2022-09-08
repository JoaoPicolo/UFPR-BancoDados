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