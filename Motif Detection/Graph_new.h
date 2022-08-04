#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <fstream>
#include <map>

using namespace std;

class Graph {
private:
    int dir = 0;
    int** adjm;
    int numNodes = 0;
    int numEdges = 0;

public:
    
    Graph(int _numNodes, int _dir);

    void addEdge(int _i, int _j, int _val);

    int getNumNodes();

    int** getAdjM();

    vector<int> getNeigh(int _i);

    void toString();

    void enumerate(vector<pair<double,double>> coords, int _k, char* name, int quads);

    void extendSubgraph(vector<pair<double,double>> coords, vector<int> _subgraph, vector<int> _vextension, int _i, int _k, int quads);

    void printToFile(map<string, int> motifs, int _k, char* name, int quads);
};

#endif