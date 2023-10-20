#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <stack>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

class Graph{
protected:
    struct Wire {
        string          wire_name;
        int             req_time, arr_time, slack = 0;
        vector<int>     nxt_gate;                   //vector store idx of next gate
    };

    struct  Gate {
        vector<int>     pre_gate;                   //vector stores idx of previous gate
        int             operation;                  //-1: Input, 0:AND, 1:OR, 2:Not
        int             delay = 1;                  //gate delay (Default = 1)
        Wire            out;
        int             step = 0;                   //0: unscheduled, > 0 schedule at step ...
    };
    // bool debug                  =   0;
    string module_name;
    vector<Gate> list_gate;
    vector<int> sorted_gate_list;
    int no_node;
    int no_input;

    void topology_sort_util(int, bool[], stack<int>&);

public:
        Graph();
        ~Graph();
        void setModuleName(string);
        string getModuleName();

        string get_wire_name(int);
        int get_wire_idx(string);

        vector<int> get_gate_inputs(int);
        
        void add_Input(string);
        void add_Output(string);

        void add_Gate(vector<string>, string, int);
        void add_Inv(string, string);               //Get index of input in list of inputs
        
        vector<int> get_successor_gate(int);
        vector<int> get_predecessor_gate(int);
        vector<int> get_predecessor_gate_n_input(int);
        vector<int> get_circuit_outputs();

        void print_Gate(int);
        void print_Graph();

        void read_blif_file(string);        //done
        void topology_sort();               //done
};

#endif