#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Nodo {
    int x;
    Nodo* m_pSon[3]{};
    Nodo* next;
    Nodo* prev;
    pair <int, int> point;
    bool isLeaf;
    
    explicit Nodo(int n, pair <int, int> point, bool is_leaf = true) {
        x = n;
        m_pSon[0] = nullptr;
        m_pSon[1] = nullptr;
        m_pSon[2] = nullptr;
        next = prev = nullptr;
        this->point = point;
        isLeaf = is_leaf;
    }   
};

bool comparex(pair <int, int> a, pair <int, int> b) {
    return (a.first < b.first);
}

bool comparey(pair <int, int> a, pair <int, int> b) {
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

Nodo* create_range_tree(vector <pair <int, int>> v, int l, int h, bool axis=true) {
    if (l==h) {
        Nodo* padre;
        int value = axis ? v[l].first : v[l].second;
        padre = new Nodo(value, {0,0}, false);
        Nodo* child = new Nodo(value, {v[l].first,v[l].second});
        padre->m_pSon[0] = child;
        return padre;
    }
    if (l+1==h) {
        Nodo* padre;
        int value_left = axis? v[l].first: v[l].second;
        int value_right = axis? v[h].first: v[h].second;
        padre = new Nodo((value_left + value_right)/2, {0,0}, false);
        Nodo* child_left =  new Nodo(value_left, {v[l].first,v[l].second});
        padre->m_pSon[0] = child_left;
        Nodo* child_right =  new Nodo(value_right, {v[h].first,v[h].second});
        padre->m_pSon[2] = child_right;
        child_left->next = child_right;
        child_right->prev = child_left;
     
        return padre;
    }
    int m = (l + h)/2;
	vector<pair <int, int>> temp = v;
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
    
    Nodo* parent_x = new Nodo((child_left_x->x + child_right_x->x)/2, {0,0}, false);
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

vector <pair <int, int>> search_by_range_y(Nodo* &root, int minX, int minY, int maxX, int maxY) {
    vector <pair <int, int>> resultado;
    auto temp = root;
    while (temp->m_pSon[0]) {
        if (minY < temp->x) {
            temp = temp->m_pSon[0];
        } else {
            temp = temp->m_pSon[2];
        }
    }
    while (temp) {
        if (temp->x <= maxY) {
            if (temp->x >= minY) {
                if (temp->point.first >= minX && temp->point.first <= maxX) {
                    resultado.emplace_back(temp->point);
                }
            }
        } else {
            break;
        }
        temp = temp->next;
    }
    return resultado;
}

vector <pair <int, int>> search_by_range_x(Nodo* &root, int minX, int minY, int maxX, int maxY) {
    auto temp = root;
    while (temp->m_pSon[0]) {
        if (minX < temp->x && maxX > temp->x) {
            return search_by_range_y(temp->m_pSon[1], minX, minY, maxX, maxY);
        }
        else if (minX < temp->x && maxX < temp->x) {
            temp = temp->m_pSon[0];
        } else {
            temp = temp->m_pSon[2];
        }
    }
    return vector <pair <int, int>>();
}
