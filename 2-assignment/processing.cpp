#include "processing.hpp"

node_t lineToNode(string line) {
    node_t node;

    char *ptr;
    ptr = strtok(line, " ");
    while (ptr != NULL) {
        cout << ptr << endl; // print the string token
        ptr = strtok(NULL, " ");
    }
    cout << "added " << line << "\n";

    return node;
}