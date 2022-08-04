#include "Isomorphism.h"

setword Isomorphism::workspace[WORKSPACE_SIZE];
int Isomorphism::n;
int Isomorphism::m;
set *Isomorphism::gv;
graph Isomorphism::g[MAXN*MAXM];
int Isomorphism::orbits[MAXN];
graph mm[MAXN*MAXM];
DEFAULTOPTIONS(options);

statsblk(stats);

void Isomorphism::canonicalStrNauty(Graph myg, char *s, int size, int _lab[], int _ptn[]) {
    options.getcanon = TRUE;
    options.writeautoms = FALSE;
    options.defaultptn = FALSE;

    int **adjM = myg.getAdjM();

    //cout << "matriz gerada: " << endl;

    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            //cout << adjM[i][j] << " ";
        }
        //cout << endl;
    }

    n = size;
    m = SETWORDSNEEDED(n);

    nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);

    EMPTYGRAPH(g,m,n);

    for (int i = 0; i < n; i++) {
        gv = GRAPHROW(g, i, m);
        EMPTYSET(gv, m);
        for (int j = 0; j < n; j++) {
            if (adjM[i][j])
                ADDELEMENT(gv, j);
        }
    }
    
    nauty(g, _lab, _ptn, NULL, orbits, &options, &stats, workspace, WORKSPACE_SIZE, m, n, mm);
    
    int aux = 0;
    for (int i = 0; i < n; i++) {
        gv = GRAPHROW(mm, i, m); 
        for (int j = 0; j < n; j++)
            s[aux++] = ISELEMENT(gv, j)?'1':'0';
    }
    s[aux] = 0;
}