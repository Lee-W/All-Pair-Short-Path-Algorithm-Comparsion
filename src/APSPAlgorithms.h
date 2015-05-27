#ifndef APSP_ALGORITHMS_H
#define APSP_ALGORITHMS_H

#include <iostream>
#include <ctime>
#include <vector>
#include "Arc.h"
using namespace std;

class APSPAlgorithms {
public:
    clock_t countTime(void (*func)());
    void initial(int n, vector<Arc> arcs);
    void AlgebraicalFloydWarshall();
    void GraphicalFloydWarshall();

private:
    vector<vector<int> > dis;
    vector<vector<int> > pre;
    int nodeNum;
};

#endif