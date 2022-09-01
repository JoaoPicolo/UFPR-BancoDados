#include "graph.hpp"
#include "processing.hpp"

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(0);

    string line;
    for (string line; getline(cin, line);) {
        lineToNode(line);
    }
}