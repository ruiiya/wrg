#include "wrg.h"

#include <bits/stdc++.h>

using namespace std;
using namespace wrg;
using namespace wrg::tm;

void create() {
    int n = 100000, k;
    vector<int> a;
    wrdrg r;
    r.get(k, 1, n);
    r.get(a, NORMAL, n, 1, 2*n);
    wout << make_tuple(n, k) << '\n' << a;
}

void solve() {
    int n, k;
    cin >> n >> k;
    vector<int> a(n);
    for(int i = 0;i < n;i ++) cin >> a[i];
    cout << a[k-1];
}

int main(int argc, char** argv) {
    test_maker tmkr("GET", 20);
    tmkr.themis_build_input(create);
    tmkr.themis_solve_input(solve);
    return 0;
}