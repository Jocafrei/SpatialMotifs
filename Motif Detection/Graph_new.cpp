#include "Graph_new.h"
#include "Isomorphism.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <string.h>
#include <fstream>
#include <map>

using namespace std;

#define NQUADS 4

map<string, int> motifs;
int total = 0;

Graph::Graph(int _numNodes, int _dir) {
    this->dir = _dir;
    this->numNodes = _numNodes;
    this->adjm = (int**)malloc(_numNodes * sizeof(int*));
    for(int i = 0; i < _numNodes; i++) {
        this->adjm[i] = new int[_numNodes];
        for (int j = 0; j < _numNodes; j++)
            this->adjm[i][j] = 0;
    }
}

void Graph::addEdge(int _i, int _j, int _val) {
    if(_i == _j)
        return;
    if(this->adjm[_i][_j] == 0) {
        this->numEdges++;
        this->adjm[_i][_j] = _val;
        if(!this->dir) {
            adjm[_j][_i] = _val;
        }
    }
}

int** Graph::getAdjM() {
    return this->adjm;
}

int Graph::getNumNodes() {
    return this->numNodes;
}

vector<int> Graph::getNeigh(int _i) {
    vector<int> neigh;
    for(int i = 0; i < numNodes; i++){
        if(adjm[_i][i] == 1)
            neigh.push_back(i);
    }
    return neigh;
}

void Graph::toString() {
    for (int i = 0; i < this->numNodes; i++) {
        for (int j = 0; j < this->numNodes; j++)
            cout << this->adjm[i][j] << " ";
        cout << "\n";
    }
}

void Graph::enumerate(vector<pair<double,double>> _coords, int _k, char* name, int quads) {
    for(int i = 0; i < this -> numNodes; i++) {
        vector<int> vextension;
        vector<int> neigh = this->getNeigh(i);
        for(size_t j = 0; j < neigh.size(); j++) {
            if(neigh[(int)j] > i)
                vextension.push_back(neigh[j]);
        }
        vector<int> subgraph;
        subgraph.push_back(i);
        extendSubgraph(_coords, subgraph, vextension, i, _k, quads);
    }
    printToFile(motifs, _k, name, quads);
}

void Graph::extendSubgraph(vector<pair<double,double>> _coords, vector<int> _subgraph, vector<int> _vextension, int _i, int _k, int quads) {
    map<int, vector<int>> colours;
    vector<int> colourlist;
    srand(time(NULL));
    if((int)_subgraph.size() == _k) {
        Graph aux(_k, this->dir);
        for (size_t i = 0; i < _subgraph.size(); i++) {
            for (size_t j = 0; j < _subgraph.size(); j++) {
                int val = this->adjm[_subgraph[i]][_subgraph[j]];
                if(val)
                    aux.addEdge(i, j, val);
            }
        }
        double min_x = _coords[_subgraph[0]].first;
        double max_x = _coords[_subgraph[0]].first;
        double min_y = _coords[_subgraph[0]].second;
        double max_y = _coords[_subgraph[0]].second;
        for(int i = 1; i < _k; i++) {
            min_x = min(min_x, _coords[_subgraph[i]].first);
            max_x = max(max_x, _coords[_subgraph[i]].first);
            min_y = min(min_y, _coords[_subgraph[i]].second);
            max_y = max(max_y, _coords[_subgraph[i]].second);
        }
        int quad = 1;
        for(int i = 0; i < _k; i++) {
            if(quads == 4) {
                if(_coords[_subgraph[i]].second < (max_y+min_y)/(double)2)
                    quad = (_coords[_subgraph[i]].first < (max_x+min_x)/(double)2) ? 3 : 4;
                else if(_coords[_subgraph[i]].first < (max_x+min_x)/(double)2)
                    quad = 2;
            }
            else if(quads == 9) {
                if(_coords[_subgraph[i]].second <= (min_y*2/3) + (max_y/3)) {
                    if(_coords[_subgraph[i]].first <= (min_x*2/3) + (max_x/3))
                        quad = 7;
                    else 
                        quad = (_coords[_subgraph[i]].first <= (min_x/3) + (max_x*2/3)) ? 8 : 9;
                }
                else if(_coords[_subgraph[i]].second <= (min_y/3) + (max_y*2/3)) {
                    if(_coords[_subgraph[i]].first <= (min_x*2/3) + (max_x/3))
                        quad = 4;
                    else
                        quad = (_coords[_subgraph[i]].first <= (min_x/3) + (max_x*2/3)) ? 5 : 6;
                }
                else if(_coords[_subgraph[i]].first >= (min_x*2/3) + (max_x/3)) {
                    quad = (_coords[_subgraph[i]].first <= (min_x/3) + (max_x*2/3)) ? 2 : 3;
                }
            }
            else {
                throw std::invalid_argument( "number of quadrants not supported" );
            }
            if(!colours.count(quad)) {
                    colours[quad] = vector<int>();
                    colours[quad].push_back(i);
                }
            else
                colours[quad].push_back(i);
            colourlist.push_back(quad);
        }
        map<int, vector<int>>::iterator it;
        int cur = 0;
        int lab[_k] = {};
        int ptn[_k] = {};
        for(auto const &it : colours) {
            for(size_t innerit = 0; innerit < it.second.size(); innerit++) {
                lab[cur] = it.second[innerit];
                if(innerit == it.second.size()-1)
                    ptn[cur] = 0;
                else
                    ptn[cur] = 1;
                cur++;
            }
        }
        //cout << endl;
        char s[1000] = "";
        string cols = "";
        Isomorphism::canonicalStrNauty(aux, s, _k, lab, ptn);
        //cout << "lab: " ;
        for(int node = 0; node < _k; node++) {
            //cout << lab[node] << " ";
            cols += to_string(colourlist[lab[node]]);
        }
        //cout << endl;
        //cout << cols << endl;

        string lbl = string(cols) + string(s);
        map<string, int>::iterator motit;
        motit = motifs.find(lbl);
        total++;
        if(motit == motifs.end()) {
            motifs[lbl] = 1;
        }
        else
            motifs[lbl]++;
        return;
    }
    while(_vextension.size() != 0) {
        int r = rand() % _vextension.size();
        int w = _vextension[r];
        _vextension.erase(_vextension.begin() + r);
        vector<int> updvex;
        updvex = _vextension;
        vector<int> neigh = this->getNeigh(w);
        int flag = 0;
        for(size_t i = 0; i < neigh.size(); i++) {
            if(find(_subgraph.begin(), _subgraph.end(), neigh[i]) != _subgraph.end() || neigh[i] <= _i)
                continue;
            flag = 1;
            for(size_t j = 0; j < _subgraph.size(); j++) {
                vector<int> neigh2 = this->getNeigh(_subgraph[j]);
                for(size_t m = 0; m < neigh2.size(); m++) {
                    if(neigh2[m] == neigh[i]) {
                        flag = 0;
                        break;
                    }
                }
            }

            if(flag == 1)
                updvex.push_back(neigh[i]);
        }
        vector<int> newsubgraph = _subgraph;
        newsubgraph.push_back(w);
        extendSubgraph(_coords, newsubgraph, updvex, _i, _k, quads);
    }
}

void Graph::printToFile(map<string, int> motifs, int _k, char* name, int quads) {
    ofstream out;
    out.open("../motifVisualization/new_templatetest.json");
    out << "{\n\t\"name\": \"" + string(name) + "\",\n\t\"dir\": " << this->dir << ",\n\t\"subgraphs\": [";
    int flag = 0;
    map<int, vector<int>> pos;
    if (quads == 4) {
        pos = {
            {1, {4,4}},
            {2, {1,4}},
            {3, {1,1}},
            {4, {4,1}}
        };
    }
    else if(quads == 9) {
        pos = {
            {1, {3,19}},
            {2, {11,19}},
            {3, {19,19}},
            {4, {3,11}},
            {5, {11,11}},
            {6, {19,11}},
            {7, {3,3}},
            {8, {11,3}},
            {9, {19,3}}
        };
    }
    for(const auto& elem : motifs) {
        int used[quads];
        fill_n(used, quads, 0);
        if(flag == 1)
            out << ",";
        flag = 1;
        out << "\n\t\t{\n\t\t\t\"label\": " << elem.first << ",";
        out << "\n\t\t\t\"nodes\": [";
        for(int i = 0; i < _k; i++) {
            if(i != 0)
                out << ",";
            out << "\n\t\t\t\t{";
            out << "\n\t\t\t\t\t";
            out << "\"id\": " << i;
            out << ",\n\t\t\t\t\t";
            out << "\"color\": " << elem.first[i];
            out << ",\n\t\t\t\t\t";
            out << "\"pos\": [";
            if(used[(int)elem.first[i]-(int)'0' - 1] == 1) {
                if (quads == 4) {
                    if((int)elem.first[i]-(int)'0' == 1)
                        out << pos[1][0]-1 << "," << pos[1][1];
                    if((int)elem.first[i]-(int)'0' == 2)
                        out << pos[2][0]+1 << "," << pos[2][1];
                    if((int)elem.first[i]-(int)'0' == 3)
                        out << pos[3][0]+1 << "," << pos[3][1];
                    if((int)elem.first[i]-(int)'0' == 4) {
                        out << pos[4][0]-1 << "," << pos[4][1];
                    }
                }
                if (quads == 9) {
                    if((int)elem.first[i]-(int)'0' == 1)
                        out << pos[1][0]+4 << "," << pos[1][1];
                    if((int)elem.first[i]-(int)'0' == 2)
                        out << pos[2][0] << "," << pos[2][1]-4;
                    if((int)elem.first[i]-(int)'0' == 3)
                        out << pos[3][0]-4 << "," << pos[3][1];
                    if((int)elem.first[i]-(int)'0' == 4) 
                        out << pos[4][0]+4 << "," << pos[4][1];
                    if((int)elem.first[i]-(int)'0' == 5) 
                        out << pos[5][0] << "," << pos[5][1]-4;
                    if((int)elem.first[i]-(int)'0' == 6) 
                        out << pos[6][0]-4 << "," << pos[6][1];
                    if((int)elem.first[i]-(int)'0' == 7) 
                        out << pos[7][0]-4 << "," << pos[7][1];
                    if((int)elem.first[i]-(int)'0' == 8) 
                        out << pos[8][0] << "," << pos[8][1]-4;
                    if((int)elem.first[i]-(int)'0' == 9) 
                        out << pos[9][0]+4 << "," << pos[9][1];
                }
            }
            else {
                out << pos[(int)elem.first[i]-(int)'0'][0] << "," << pos[(int)elem.first[i]-(int)'0'][1];
            }
            out << "]";
            used[(int)elem.first[i]-(int)'0' - 1] = 1;
            out << "\n\t\t\t\t}";
        }
        out << "\n\t\t\t],\n\t\t\t\"edges\": [";
        int nedge = 0;
        for(int j = _k; j < _k*_k+_k; j++) {
            if((int)elem.first[j]-(int)'0' == 1) {
                if(nedge == 1)
                    out << ",";
                out << "\n\t\t\t\t[" << (j/_k)-1 << "," << j%_k << "]";
                nedge = 1;
            }
        }
        out << "\n\t\t\t],\n\t\t\t\"occurrences\": " << elem.second << ",";
        out << "\n\t\t\t\"percentage\": " << (double)elem.second/(double)total << "\n\t\t}";
    }
    out << "\n\t]\n}";
    out.close();
}