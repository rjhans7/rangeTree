#include "rangeTree.h"


int main() {
  vector <pii> v = {{36, 1}, {37, 12}, {42, 16}, {51, 13}, {62, 15}, {72, 17}};
  Nodo * root = create_range_tree(v,0,5);
  cout << "Fin";
  return 0;
}