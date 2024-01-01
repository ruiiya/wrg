#include "printer.h"

#include <iostream>

using namespace std;
using namespace wrg;

int main(int argc, char** argv) {
    base_graph a;
    a.add(1,2);
    a.add(2,3,3);
    a.add(1,3);
    a.add(4, 5);
    a.add(1, 5, 3);
    cout << a.get_edges();
    return 0;
}