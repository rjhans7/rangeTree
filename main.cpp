#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef pair<int, int> pii;

struct Nodo {
     int x;
     Nodo * m_pSon[3];
     Nodo * next;
     Nodo * prev;
     bool isLeaf;
     Nodo(int n)
     {
        x = n;
        m_pSon[0]=0;
        m_pSon[1]=0; 
        m_pSon[2]=0; 
        next = prev = 0;
        isLeaf = true;
     }
};

//Nodo * indice = nullptr;

bool comparex(pii a, pii b) {
  return (a.first < b.first);
}

bool comparey(pii a, pii b) {
  return (a.second < b.second);
}

Nodo * create_range_tree(vector <pii> v, int l, int h, bool axis=true) {
   if(l==h)
   {
     Nodo * padre;
     if (axis) {
      padre = new Nodo(v[l].first);
      padre->isLeaf = false;
      Nodo* child = new Nodo(v[l].first);
      
      padre->m_pSon[0] = child;

     } else {
      padre = new Nodo(v[l].second);
      padre->isLeaf = false;
      Nodo* child = new Nodo(v[l].second);
      padre->m_pSon[0] = child;
     }
      
     return padre;

   } 
   if( l+1==h) {
     Nodo * padre;
     if (axis) {
         padre = new Nodo((v[l].first + v[h].first)/2);
         padre->isLeaf = false;
         Nodo* child_left =  new Nodo(v[l].first);

         padre->m_pSon[0] = child_left;
         Nodo* child_right =  new Nodo(v[h].first);

         padre->m_pSon[2] = child_right;
         child_left->next = child_right;
         child_right->prev = child_left;

     } else {
         padre = new Nodo((v[l].second + v[h].second)/2);
         padre->isLeaf = false;
         Nodo* child_left = new Nodo(v[l].second);

         padre->m_pSon[0] = child_left;
         Nodo* child_right = new Nodo(v[h].second);

         padre->m_pSon[2] = child_right;
         child_left->next = child_right;
         child_right->prev = child_left;

     }

     return padre;
   }
   int m = (l + h)/2;
   sort(v.begin()+l, v.begin()+h, comparex);
   Nodo * plx = create_range_tree(v,l, m, axis);
   Nodo * prx = create_range_tree(v,m+1, h, axis);
   Nodo * left_most_right_lx = plx;
   while (!left_most_right_lx->isLeaf){
      if(left_most_right_lx->m_pSon[2]){
         left_most_right_lx = left_most_right_lx->m_pSon[2];
      }else{
         left_most_right_lx = left_most_right_lx->m_pSon[0];
      }
   }
   Nodo * right_most_left_lx = prx;
   while (!right_most_left_lx->isLeaf){
      if(right_most_left_lx->m_pSon[0]){
         right_most_left_lx = right_most_left_lx->m_pSon[0];
      }else{
         right_most_left_lx = left_most_right_lx->m_pSon[2];
      }
   }
   left_most_right_lx->next = right_most_left_lx;
   right_most_left_lx->prev = left_most_right_lx;

   
   sort(v.begin()+l, v.begin()+h, comparey);
   Nodo * ply = create_range_tree(v,l, m, false);
   plx->m_pSon[1] = ply;
   Nodo * pry = create_range_tree(v,m+1, h, false);
   prx->m_pSon[1] = pry;

   left_most_right_lx = ply;
   while (!left_most_right_lx->isLeaf){
      if(left_most_right_lx->m_pSon[2]){
         left_most_right_lx = left_most_right_lx->m_pSon[2];
      }else{
         left_most_right_lx = left_most_right_lx->m_pSon[0];
      }
   }
   right_most_left_lx = pry;
   while (!right_most_left_lx->isLeaf){
      if(right_most_left_lx->m_pSon[0]){
         right_most_left_lx = right_most_left_lx->m_pSon[0];
      }else{
         right_most_left_lx = left_most_right_lx->m_pSon[2];
      }
   }
   left_most_right_lx->next = right_most_left_lx;
   right_most_left_lx->prev = left_most_right_lx;

   
   Nodo * padrex = new Nodo((plx->x + prx->x)/2);
   padrex->isLeaf = false;
   padrex->m_pSon[0] = plx;

   Nodo * padrey = new Nodo((ply->x + pry->x)/2);
   padrey->m_pSon[0] = ply;
   padrey->m_pSon[2] = pry;

   padrey->isLeaf = false;
   padrex->m_pSon[1] = padrey;
   padrex->m_pSon[2] = prx;
   
   return padrex;
}

/*void enlazarHojas(Nodo * r) {
  if (!r) return;
  enlazarHojas(r->m_pSon[0]);
  if(!r->m_pSon[0] && !r->m_pSon[2]) {
    if (!indice) {
      indice = r;
    } else {
      indice->next = r;
      r->prev = indice;
      indice = r;
    }
  }
  enlazarHojas(r->m_pSon[2]);
}

void linkTree(Nodo * r){

  while(r->m_pSon[0]){ ///mas a la izquierda
     r = r->m_pSon[0];
  }
  enlazarHojas(r);
  Nodo* temp = r->m_parent; //subo al primer padre que tenga hijo a la derecha
  while(!temp->m_pSon[2]) {
     temp = temp->m_parent;
  }
  Nodo* yo = r;
  while(temp->m_pSon[2] == yo) {
     temp = temp->m_parent;
     yo = temp->m_pSon[2];
  }

  temp = temp->m_pSon[2];
  while(temp->m_pSon[0]){  //derecha mas a la izquiera
     temp = temp->m_pSon[0];
  }

  r->next = temp;
  temp->prev = r;
  if (!temp->m_parent) return;
  linkTree(temp);
}*/

void print(Nodo * r) {
   if(!r) return;
   print(r->m_pSon[0]);
   if(!r->m_pSon[0] && !r->m_pSon[2])
      cout<<r->x<<" ";
   print(r->m_pSon[2]);
}

int main() {
  vector <pii> v = {{36, 1}, {37, 12}, {42, 16}, {51, 13}, {62, 15}, {72, 17}};
  Nodo * root = create_range_tree(v,0,5);
  //linkTree(root);
  cout << "Eje x: \n";
  print(root);
  cout << "\nEje y: \n";
  print(root->m_pSon[1]);

  return 0;
}

