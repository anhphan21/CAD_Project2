#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Graph{
    enum op_code {
        Not = -1,
        And = 0,
        Or  = 1
    };

    struct Wire {
        int             req_time, arr_time, slack = 0;
        vector<int>     nxt_gate;
    };

    struct  Gate {
        string          output_name;
        int             operation;                //-1: Not, 0: AND, 1: OR
        int             delay = 1;
        Wire            out;
    };

    struct Input {
        string          in_name;
        int             req_time, arr_time, slack = 0;
        vector<int>     nxt_gate;
    };
    
    private:
        string module_name;
        vector<Gate> list_gate;
        vector<Input> list_input;
        int no_gate;
        int no_wire;
    public:
        Graph();
        ~Graph();
        void setModuleName(string);
        int find_gate_idx(string);

        void add_Gate(vector<string>, string, int);

        void printGraph();
};

#endif