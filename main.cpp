#include <iostream>

#include "rangeTree.h"

using namespace std;

int main() {
    vector <pair <int, int>> v = {{36, 1}, {37, 12}, {42, 16}, {51, 13}, {62, 15}, {72, 17}};
    Nodo* root = create_range_tree(v,0,(int)v.size()-1);
    vector <pair <int, int>> resultado = search_by_range_x(root,37,5,52,20);
    for (auto it : resultado) {
        cout << '(' << it.first << ',' << it.second << ')' << endl;
    }
    return 0;
}