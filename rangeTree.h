#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Nodo {
    int value;
    Nodo* m_pSon[3]{};
    Nodo* next;
    Nodo* prev;
    pair <int, int> point;
    bool isLeaf;
    
    explicit Nodo(int value, pair <int, int> point, bool isLeaf = true) {
        this->value = value;
        m_pSon[0] = nullptr;
        m_pSon[1] = nullptr;
        m_pSon[2] = nullptr;
        next = prev = nullptr;
        this->point = point;
        this->isLeaf = isLeaf;
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

Nodo* create_y_range_tree(vector <pair <int, int>> v, int l, int h, bool first_time = true) {
    if (l==h) {
        Nodo* padre;
        padre = new Nodo(v[l].second, {0,0}, false);
        Nodo* child = new Nodo(v[l].second, {v[l].first,v[l].second});
        padre->m_pSon[0] = child;
        return padre;
    }
    else if (l+1==h) {
        Nodo* padre = new Nodo((v[l].second + v[h].second)/2, {0,0}, false);
        Nodo* child_left =  new Nodo(v[l].second, {v[l].first,v[l].second});
        padre->m_pSon[0] = child_left;
        Nodo* child_right =  new Nodo(v[h].second, {v[h].first,v[h].second});
        padre->m_pSon[2] = child_right;
        child_left->next = child_right;
        child_right->prev = child_left;
     
        return padre;
    }
    int m = (l + h)/2;
    if (first_time) sort(v.begin()+l, v.begin()+h+1, comparey); //y axis
    Nodo* child_left = create_y_range_tree(v,l, m, !first_time);
    Nodo* child_right = create_y_range_tree(v,m+1, h, !first_time);
	
	Nodo* child_most_right = get_most_right(child_left);
	Nodo* child_most_left = get_most_left(child_right);
	child_most_right->next = child_most_left;
	child_most_left->prev = child_most_right;
    
    Nodo* parent = new Nodo((child_left->value + child_right->value)/2, {0,0}, false);
    parent->m_pSon[0] = child_left;
    parent->m_pSon[2] = child_right;
    
    return parent;
}

Nodo* create_range_tree(vector <pair <int, int>> v, int l, int h) {
    if (l==h) {
        Nodo* padre_x = new Nodo(v[l].first , {0,0}, false);
        Nodo* padre_y = new Nodo(v[l].second , {0,0}, false);
        Nodo* child_x = new Nodo(v[l].first , {v[l].first,v[l].second});
        Nodo* child_y = new Nodo(v[l].second , {v[l].first,v[l].second});
        padre_x->m_pSon[0] = child_x;
        padre_y->m_pSon[0] = child_y;
        padre_x->m_pSon[1] = padre_y; //linking y tree
        return padre_x;
    }
    if (l+1==h) {
        Nodo* padre_x = new Nodo((v[l].first + v[h].first)/2, {0,0}, false);
        Nodo* child_left_x =  new Nodo(v[l].first, {v[l].first,v[l].second});
        Nodo* child_right_x =  new Nodo(v[h].first, {v[h].first,v[h].second});
        padre_x->m_pSon[0] = child_left_x;
        padre_x->m_pSon[2] = child_right_x;
        child_left_x->next = child_right_x;
        child_right_x->prev = child_left_x;

        // y tree
		sort(v.begin()+l, v.begin()+h+1, comparey); 
        Nodo* padre_y = new Nodo((v[l].second + v[h].second)/2, {0,0}, false);
        Nodo* child_left_y =  new Nodo(v[l].second, {v[l].first, v[l].second});
        Nodo* child_right_y =  new Nodo(v[h].second, {v[h].first, v[h].second});
        padre_y->m_pSon[0] = child_left_y;
        padre_y->m_pSon[2] = child_right_y;
        child_left_y->next = child_right_y;
        child_right_y->prev = child_left_y;
     
        //Linking ytree with xtree
        padre_x->m_pSon[1] = padre_y;
        return padre_x;
    }
    int m = (l + h) / 2;
	
    //y tree
    Nodo * parent_y = create_y_range_tree(v, l, h);
 
    Nodo* child_left = create_range_tree(v,l, m);
    Nodo* child_right = create_range_tree(v,m+1, h);
	
	Nodo* child_most_right = get_most_right(child_left);
	Nodo* child_most_left = get_most_left(child_right);
	child_most_right->next = child_most_left;
	child_most_left->prev = child_most_right;

	   
    Nodo* parent = new Nodo((child_left->value + child_right->value) / 2, {0, 0}, false);
    parent->m_pSon[0] = child_left;
    parent->m_pSon[2] = child_right;
    
    
    
	parent->m_pSon[1] = parent_y;
   
    return parent;
}

vector <pair <int, int>> search_by_range_y(Nodo* &root, int minX, int minY, int maxX, int maxY) {
    vector <pair <int, int>> resultado;
    auto temp = root;
    while (temp && temp->m_pSon[0]) {
        if (minY < temp->value) {
            temp = temp->m_pSon[0];
        } else {
            temp = temp->m_pSon[2];
        }
    }
    while (temp) {
        if (temp->value <= maxY) {
            if (temp->value >= minY) {
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

void print_leaves(Nodo* parent){
    cout << "Printing Leaves\n";
    while (!parent->isLeaf) {
        parent = parent->m_pSon[0];
    }
    while(parent->next){
        cout << parent->value << ", ";
        parent = parent->next;
    }
    cout << parent->value << ", ";
    cout << endl;

}

vector <pair <int, int>> search_by_range_x(Nodo* &root, int minX, int minY, int maxX, int maxY) {
    auto temp = root;
    while (temp && temp->m_pSon[0]) {
        if (minX < temp->value && maxX > temp->value) {
            return search_by_range_y(temp->m_pSon[1], minX, minY, maxX, maxY);
        }
        else if (minX < temp->value && maxX < temp->value) {
            temp = temp->m_pSon[0];
        } else {
            temp = temp->m_pSon[2];
        }
    }
    return vector <pair <int, int>>();
}

vector <pair <int, int>> search_by_range(Nodo* &root, pair<int, int> begin, pair<int, int> end) {
    return search_by_range_x(root, begin.first, begin.second, end.first, end.second);
}