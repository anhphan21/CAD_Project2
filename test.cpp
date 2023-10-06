#include <iostream>
#include <string>
#include <list>
#include "./include/Graph.h"
using namespace std;

int main() {
    Graph test;
    test.setModuleName("test");
    vector <string> in1 {"a", "b", "c"};
    string out1 = "y";
    test.add_Gate(in1, out1, -1);
    test.print_Graph();
    return 0;
}