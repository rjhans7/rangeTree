#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

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
        this->m_pSon[0] = nullptr;
        this->m_pSon[1] = nullptr;
        this->m_pSon[2] = nullptr;
        this->next = nullptr;
        this->prev = nullptr;
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
bool compare(pair <int, int> a, pair <int, int> b) {
    return ((a.second < b.second) && (a.first < b.first)) || (a.second < b.second);
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

Nodo* create_y_range_tree(pair <int, int> v[], int l, int h, bool first_time = true) {
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
    if (first_time) sort(v+l, v+h+1, comparey);
    Nodo* child_left = create_y_range_tree(v,l, m, false);
    Nodo* child_right = create_y_range_tree(v,m+1, h, false);
	
	Nodo* child_most_right = get_most_right(child_left);
	Nodo* child_most_left = get_most_left(child_right);
	child_most_right->next = child_most_left;
	child_most_left->prev = child_most_right;
    
    Nodo* parent = new Nodo((child_most_right->value + child_most_left->value)/2, {0,0}, false);
    parent->m_pSon[0] = child_left;
    parent->m_pSon[2] = child_right;
    
    return parent;
}

Nodo* create_range_tree(pair <int, int> v[], int l, int h) {
    if (l==h) {
        Nodo* padre_x = new Nodo(v[l].first , {0,0}, false);
        Nodo* child_x = new Nodo(v[l].first , {v[l].first,v[l].second});
        padre_x->m_pSon[0] = child_x;
        padre_x->m_pSon[1] = create_y_range_tree(v, l, h);
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
     
        //Linking ytree with xtree
        sort(v+l, v+h+1, comparey);
        padre_x->m_pSon[1] = create_y_range_tree(v, l, h);
        return padre_x;
    }
    int m = (l + h) / 2;
	
    //y tree
    Nodo * parent_y = create_y_range_tree(v, l, h);
    sort(v+l,v+h+1, comparex);
    Nodo* child_left = create_range_tree(v,l, m);
    Nodo* child_right = create_range_tree(v,m+1, h);
	
	Nodo* child_most_right = get_most_right(child_left);
	Nodo* child_most_left = get_most_left(child_right);
	child_most_right->next = child_most_left;
	child_most_left->prev = child_most_right;

	   
    Nodo* parent = new Nodo((child_most_right->value + child_most_left->value) / 2, {0, 0}, false);
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

void printTree(Nodo *cur, int level) 
{
  if (cur) {
    printTree(cur->m_pSon[0],level+1);
    for (int k = 0; k < level; k++) cout << "\t";
    if (cur->isLeaf) cout << cur->point.first << " " << cur->point.second <<endl;
    else cout << cur->value << endl;

    printTree(cur->m_pSon[2], level+1);

  }
}

bool inRange(pair<int, int> point, int beg_x, int end_x, int beg_y, int end_y) {
    bool inX = (beg_x <= point.first) && (point.first <= end_x);
    bool inY = (beg_y <= point.second) && (point.second <= end_y);
    return inX && inY;
}

vector <pair <int, int>> range_search_1d (int beg, int end, Nodo* cur) {
    vector <pair <int, int>> result;
    
    if (!cur) return result;

    while (!cur->isLeaf) {
        if (beg <= cur->value) cur = cur->m_pSon[0];
        else {
            if (cur->m_pSon[2]) cur = cur->m_pSon[2];
            else cur = cur->m_pSon[0];
        }
    }

    if (cur && cur->value < beg) cur = cur->next;
    while (cur && cur->value <= end) {
        if (beg <= cur->value && cur->value <= end)
            result.push_back(cur->point);
        cur = cur->next;
    }
    return result;
}




vector <pair <int, int>> range_search_2d (int beg_x, int end_x, int beg_y, int end_y, Nodo* cur) {

    vector <pair <int, int>> result;
    if (!cur) return result;

    while (!cur->isLeaf) {
        if (end_x < cur->value) cur = cur->m_pSon[0];
        else if (cur->value < beg_x) {
            if (cur->m_pSon[2]) cur = cur->m_pSon[2];
            else cur = cur->m_pSon[0];
        }
        else break;
    }

    if (cur->isLeaf) {
        if (inRange(cur->point, beg_x, end_x, beg_y, end_y))
            result.push_back(cur->point);
    } else {
        Nodo* temp = cur;
        cur = cur->m_pSon[0];
        while (!cur->isLeaf) {
            if (beg_x <= cur->value) {
                vector<pair<int, int>> re;
                Nodo * cur_y;
                if (cur->m_pSon[2]) cur_y = cur->m_pSon[2];
                else cur_y = cur->m_pSon[0];
                if (cur_y->m_pSon[1])
                    re = range_search_1d(beg_y, end_y, cur_y->m_pSon[1]);
                else {
                    if (inRange(cur_y->point, beg_x, end_x, beg_y, end_y))
                        result.push_back(cur_y->point);
                }

                for (auto r: re) result.push_back(r);
                cur = cur->m_pSon[0];
            } else {
                if (cur->m_pSon[2]) cur = cur->m_pSon[2];
                else cur = cur->m_pSon[0];
            }
        }

        if (inRange(cur->point, beg_x, end_x, beg_y, end_y))
            result.push_back(cur->point);
        
        if (temp->m_pSon[2]) cur = temp->m_pSon[2];
        else cur = temp->m_pSon[0];

        while (!cur->isLeaf) {
            if (cur->value <= end_x) {
                vector<pair<int, int>> re;
                if (cur->m_pSon[0]->m_pSon[1])
                    re = range_search_1d(beg_y, end_y, cur->m_pSon[0]->m_pSon[1]);
                else {
                    if (inRange(cur->m_pSon[0]->point, beg_x, end_x, beg_y, end_y))
                        result.push_back(cur->m_pSon[0]->point);
                }

                for (auto r: re) result.push_back(r);
                
                if (cur->m_pSon[2]) cur = cur->m_pSon[2];
                else cur = cur->m_pSon[0];

            } else cur = cur->m_pSon[0];
        }

        if (inRange(cur->point, beg_x, end_x, beg_y, end_y))
            result.push_back(cur->point);
    }
    return result;
}




vector <pair <int, int>> search_by_range(Nodo* &root, pair<int, int> begin, pair<int, int> end) {
    //return search_by_range_x(root, begin.first, begin.second, end.first, end.second);
    return range_search_2d(begin.first, end.first, begin.second, end.second, root);
}