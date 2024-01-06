#include "wrg.h"

#include <bits/stdc++.h>

using namespace std;
using namespace wrg;

int main(int argc, char** argv) {
    vector<int> a;
    cout << detail::split(wrdrg::get(a, UNIQUE, 100, 1, 100), 10);
    return 0;
}