#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Graph{
    struct Wire {
        string          wire_name;
        int             req_time, arr_time, slack = 0;
        vector<int>     nxt_gate;
    };

    struct  Gate {
        int             operation;                //-1: Not, 0: AND, 1: OR
        int             delay = 1;
        Wire            out;
    };
    
    private:
        string module_name;
        vector<Gate> list_gate;
        vector<Wire> list_input;
        int no_gate;
    public:
        Graph();
        ~Graph();
        void setModuleName(string);
        string getModuleName();

        int get_gate_idx(string);
        string get_gate_name(int);
        int get_input_idx(string);
        vector<string> get_gate_inputs(string);
        
        void add_Input(string);
        void add_Output(string);

        void add_Gate(vector<string>, string, int);
        void add_Inv(string, string);               //Get index of input in list of inputs
        
        vector<int> get_successor_gate(string);
        vector<int> get_predecessor_gate(string);

        void print_Input(int);
        void print_Gate(int);
        void print_Graph();
};

#endif