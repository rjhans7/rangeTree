#include <iostream>

#include "RangeTree.h"
#define N 6
using namespace std;

int main() {
    pair <int, int> v[N] = {{36, 1}, {37, 12}, {42, 16}, {51, 13}, {62, 15}, {72, 17}};
    sort(v, v + N, comparex);
    Nodo* root = create_range_tree(v,0,N-1);
    vector <pair <int, int>> resultado = search_by_range(root,{37,5},{52,20});
    for (auto it : resultado) {
        cout << '(' << it.first << ',' << it.second << ')' << endl;
    }
    return 0;
}