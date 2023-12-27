#include <bits/stdc++.h>
#include "wrg.h"

using namespace std;
using namespace wrg;

int main(int argc, char** argv) {
    int v = 0;
    random<vector<vector<int>>> r;
    size_t n, m;
    gout << wrdrg::tget(n,1,10) << " " << wrdrg::tget(m,1,10) << '\n';
    vector<vector<int>> a;
    gout << wrdrg::tget(a, NORMAL,n,NORMAL,m,1,100) << '\n';
    return 0;
}