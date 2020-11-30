#include <iostream>
#include <ctime>
#include <cstdlib>
#include "rangeTree.h"
#define SAMPLE_SIZE 1000

typedef pair<int, int> pii;

using namespace std;

vector<pii> sample_points (size_t sample_size) {
    vector<pii> result;
    for (size_t i = 0; i < sample_size; i++) {
        int x = rand() % 1000  ;
        int y = rand() % 1000  ;
        result.push_back({x, y});
    }
    return result;    
}

vector<pair<pii, pii>> sample_queries (size_t sample_size) {
    vector<pair<pii, pii>> result;
    for (size_t i = 0; i < sample_size; i++) {
        int x_min = rand() % 1000  ;
        int y_min = rand() % 1000  ;
        int x_max = rand() % 1000  ;
        int y_max = rand() % 1000  ;
        if (x_min > x_max) swap(x_min, x_max);
        result.push_back({{x_min, y_min}, {x_max, y_max}});
    }
    return result;    
}

void test (Nodo* &tree, vector<pair<pii, pii>> &queries, bool show_results = false) {   
    for (auto query: queries) {
        vector <pii> resultado = search_by_range(tree, query.first, query.second);
        if (show_results) {
            cout << "Query: (" << query.first.first << ',' << query.first.second << "), (" << query.second.first << ',' << query.second.second << ')'  << endl;
            cout << "Results:" << endl;
            for (auto it : resultado) {
                cout << '(' << it.first << ',' << it.second << ')' << endl;
            }
        }    
    }
}


int main() {
    srand((unsigned int)time(NULL));
    clock_t tStart, tEnd;
    double timeTaken;
    int query_size = 0;
    
    //Filling the tree
    vector<pii> muestra = sample_points(SAMPLE_SIZE);
    Nodo* tree = create_range_tree(muestra, 0, SAMPLE_SIZE - 1);

    // Tests

    query_size = 10;
    vector<pair<pii, pii>> queries_10 = sample_queries(query_size);
    tStart = clock();
    test(tree, queries_10, true);
    tEnd = clock();
    timeTaken = double(tEnd - tStart)/CLOCKS_PER_SEC; 
    cout << "Query size: " << query_size << " Time Taken: " << timeTaken << endl;

    query_size = 100;
    vector<pair<pii, pii>> queries_100 = sample_queries(query_size);
    tStart = clock();
    test(tree, queries_100);
    tEnd = clock();
    timeTaken = double(tEnd - tStart)/CLOCKS_PER_SEC; 
    cout << "Query size: " << query_size << " Time Taken: " << timeTaken << endl;

    query_size = 1000;
    vector<pair<pii, pii>> queries_1k = sample_queries(query_size);
    tStart = clock();
    test(tree, queries_1k);
    tEnd = clock();
    timeTaken = double(tEnd - tStart)/CLOCKS_PER_SEC; 
    cout << "Query size: " << query_size << " Time Taken: " << timeTaken << endl;

    query_size = 10000;
    vector<pair<pii, pii>> queries_10k = sample_queries(query_size);
    tStart = clock();
    test(tree, queries_10k);
    tEnd = clock();
    timeTaken = double(tEnd - tStart)/CLOCKS_PER_SEC; 
    cout << "Query size: " << query_size << " Time Taken: " << timeTaken << endl;

    
    return 0;
}