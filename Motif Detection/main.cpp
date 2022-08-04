#include "Graph_new.h"
#include <iostream>
#include <stdio.h>
#include <utility>
#include <map>
using namespace std;

int main(int argc, char *argv[]) {
    int n_nodes, n_edges, dir = 0;

    scanf("%d", &dir);

    scanf("%d", &n_nodes);
    vector<pair<double,double>> coords;
    
    double x, y = 0;
    for(int i = 0; i < n_nodes; i++) {
        scanf("%lf%lf", &x, &y);
        pair<double,double> p(x, y);
        coords.push_back(p);
    }
    
    scanf("%d", &n_edges);

    int u, v, w = 0;
    Graph g(n_nodes, dir);
    
    for(int i = 0; i < n_edges; i++) {
        scanf("%d%d%d", &u, &v, &w);
        g.addEdge(u-1, v-1, w);
    }

    //g.toString();
    g.enumerate(coords, 3, argv[1], 9);
}