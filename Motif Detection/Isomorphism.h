#ifndef _ISOMORPHISM_
#define _ISOMORPHISM_

#include "Graph.h"

#define MAXN 1000 
#define WORKSPACE_SIZE MAXN*160

#include "../nauty/nauty.h"

class Isomorphism {
  private:
    static setword workspace[WORKSPACE_SIZE];
    static int n, m, lab[MAXN], ptn[MAXN], orbits[MAXN];
    static set *gv;
    static graph g[MAXN*MAXM];

  public:
    static void canonicalStrNauty(Graph myg, char *s, int size, int _lab[], int _ptn[]);
};

#endif