#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef pair<int, int> pii;

struct Nodo {
    int x;
    Nodo* m_pSon[3]{};
    Nodo* next;
    Nodo* prev;
    bool isLeaf;
    
    explicit Nodo(int n, bool is_leaf = true) {
        x = n;
        m_pSon[0] = nullptr;
        m_pSon[1] = nullptr;
        m_pSon[2] = nullptr;
        next = prev = nullptr;
        isLeaf = is_leaf;
    }   
};

bool comparex(pii a, pii b) {
    return (a.first < b.first);
}

bool comparey(pii a, pii b) {
    return (a.second < b.second);
}

Nodo* get_most_right(Nodo* parent) {
  	while (!parent->isLeaf) {
		if(parent->m_pSon[2]) {
		    parent = parent->m_pSon[2];
		} else {
            parent = parent->m_pSon[0];
		}
    }
  	return parent;
}

Nodo* get_most_left(Nodo* parent) {
	while (!parent->isLeaf) {
		if(parent->m_pSon[0]) {
		    parent = parent->m_pSon[0];
		} else {
		    parent = parent->m_pSon[2];
		}
    }
  	return parent;
}

Nodo* create_range_tree(vector <pii> v, int l, int h, bool axis=true) {
    if (l==h) {
        Nodo* padre;
        int value = axis? v[l].first: v[l].second;
        padre = new Nodo(value, false);
        Nodo* child = new Nodo(value);
        padre->m_pSon[0] = child;
        return padre;
    }
    if (l+1==h) {
        Nodo* padre;
        int value_left = axis? v[l].first: v[l].second;
        int value_right = axis? v[h].first: v[h].second;
        padre = new Nodo((value_left + value_right)/2, false);
        Nodo* child_left =  new Nodo(value_left);
        padre->m_pSon[0] = child_left;
        Nodo* child_right =  new Nodo(value_right);
        padre->m_pSon[2] = child_right;
        child_left->next = child_right;
        child_right->prev = child_left;
     
        return padre;
    }
    int m = (l + h)/2;
	vector<pii> temp = v;
	if (axis) sort(v.begin()+l, v.begin()+h, comparex); //x axis
    Nodo* child_left_x = create_range_tree(v,l, m, axis);
    Nodo* child_right_x = create_range_tree(v,m+1, h, axis);
	
	Nodo* child_most_right = get_most_right(child_left_x);
	Nodo* child_most_left = get_most_left(child_right_x);
	child_most_right->next = child_most_left;
	child_most_left->prev = child_most_right;

	if (axis) {
        Nodo* child_left_y = create_range_tree(v,l, m, false);
		child_left_x->m_pSon[1] = child_left_y;
        Nodo* child_right_y = create_range_tree(v,m+1, h, false);
		child_right_x->m_pSon[1] = child_right_y;
	}
    
    Nodo* parent_x = new Nodo((child_left_x->x + child_right_x->x)/2, false);
    parent_x->m_pSon[0] = child_left_x;
	
	if (axis) {
		v = temp; //restore the input 
		sort(v.begin()+l, v.begin()+h, comparey); //y axis
		Nodo * parent_y = create_range_tree(v, l, h, false);
		parent_x->m_pSon[1] = parent_y;
	}
    parent_x->m_pSon[2] = child_right_x;
    
    return parent_x;
}