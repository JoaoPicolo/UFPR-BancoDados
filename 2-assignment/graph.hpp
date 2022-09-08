#include <string>
#include <iostream>

#include <list>
#include <vector>


void copyVisao(vector<nodo_visao_t> a);

void updateAttr(vector<attribut_t> &attr, transaction_t tx);

bool updateAdj(vector<nodo_t> &adj, transaction_t tx);

bool hasCycle(vector<nodo_t> adj);

void updateVisao(vector<nodo_visao_t> &arr, transaction_t tx);

bool visaoEq(vector<nodo_visao_t> arr, vector<attribut_t> attributes);

