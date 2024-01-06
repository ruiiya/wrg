#include "wrg.h"

#include <bits/stdc++.h>

using namespace std;
using namespace wrg;

int main(int argc, char** argv) {
    wrdrg r;
    vector<int> a;
    vector<vector<int>> b;
    wout << print_size();
    wout << r.get(a, NORMAL, 10, 1, 100);
    wout << '\n';
    wout << r.get(b, NORMAL, 10, NORMAL, 10, 1, 100);
    wout << '\n';
    wout << make_tuple(1,2,3,4,"asdasjd",'a','3');
    wout << '\n';
    wout << make_pair(1, 2);
    return 0;
}