#include <iostream>
#include <ctime>
#include <cstdlib>
#include "RangeTree.h"
#include <fstream>
#define SAMPLE_SIZE 100000

typedef pair<int, int> pii;

using namespace std;

vector<pii> sample_points (size_t sample_size) {
    vector<pii> result;
    for (size_t i = 0; i < sample_size; i++) {
        int x = rand() % 1000;
        int y = rand() % 1000;
        result.emplace_back(x, y);
    }
    
    sort(result.begin(), result.end(), comparex);
    return result;    
}

vector<pair<pii, pii>> sample_queries (size_t sample_size) {
    vector<pair<pii, pii>> result;
    for (size_t i = 0; i < sample_size; i++) {
        int x_min = rand() % 1000;
        int y_min = rand() % 1000;
        int x_max = rand() % 1000;
        int y_max = rand() % 1000;
        if (x_min > x_max) swap(x_min, x_max);
        result.push_back({{x_min, y_min}, {x_max, y_max}});
    }
    return result;    
}

vector<pii> brute_force_query(vector<pii> &sample, pii beg, pii end){
    vector <pii> result;
    for (auto s: sample) {
        if (s.first >= beg.first && s.first <= end.first && 
            s.second >= beg.second && s.second <= end.second)
            result.push_back(s);
    }
    return result;
}

void validate (vector<pii> &sample, const vector<pii>& resultado, pii beg, pii end, ofstream &outfile, bool verbose) {
    vector<pii> bf_result = brute_force_query(sample, beg, end);
    sort(bf_result.begin(), bf_result.end(), comparex);
    if (resultado == bf_result) outfile << "Valid Result: TRUE\n";
    else {
        outfile << "Valid Result: FALSE\n";
        if (verbose) {
            outfile << "It should be:\n";
            for (auto it : bf_result) 
                outfile << '(' << it.first << ',' << it.second << ')' << endl;
        }
 
    }
}

void test (Nodo* &tree, vector<pii> &sample, vector<pair<pii, pii>> &queries, ofstream &outfile, bool verbose = false) {
    clock_t tStart, tEnd;
    double timeTaken = 0;
    for (auto query: queries) {
        tStart = clock();
        vector <pii> resultado = search_by_range(tree, query.first, query.second);
        tEnd = clock();
        timeTaken += double(tEnd - tStart)/CLOCKS_PER_SEC;        
        outfile << "Query: (" << query.first.first << ',' << query.first.second << "), (" << query.second.first << ',' << query.second.second << ')'  << endl;
        if (verbose) {
            outfile << "Results:" << endl;
            sort(resultado.begin(), resultado.end(), comparex);
            for (auto it : resultado) {
                outfile << '(' << it.first << ',' << it.second << ')' << endl;
            }
        }

        
        validate(sample, resultado, query.first, query.second, outfile, verbose);  
        outfile << endl;
    }
    cout << "Query size: " << queries.size() << " Time Taken: " << timeTaken << endl;
   
}

void test_all (Nodo* &tree, vector<pii> &sample, const vector<int>& queries_sizes) {
    for (auto qs: queries_sizes) {
        string name = "results/result_query_size_" + to_string(qs) + ".txt";
        ofstream outfile(name, ios::trunc);
        vector<pair<pii, pii>> queries = sample_queries(qs);
        test(tree, sample, queries, outfile);
    }
}


int main() {
    srand(time(nullptr));
    clock_t tStart, tEnd;
    
    //Filling the tree
    vector<pii> muestra = sample_points(SAMPLE_SIZE);
    pii v[SAMPLE_SIZE];
    for (int i = 0; i < SAMPLE_SIZE; i++) v[i] = muestra[i];
    tStart = clock();
    Nodo* tree = create_range_tree(v, 0, SAMPLE_SIZE - 1);
    tEnd = clock();
    cout << "Build Range Tree of size: " << muestra.size() << " Time Taken: " << double(tEnd - tStart)/CLOCKS_PER_SEC << endl;
    
    // Tests
    vector<int> query_sizes = {10, 100, 1000, 10000};
    test_all(tree, muestra, query_sizes);

    
    return 0;
}