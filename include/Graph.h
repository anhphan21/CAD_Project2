#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
using namespace std;

class Graph{
    struct Wire;
    
    struct  Gate {
        string      Gate_ID;
        bool        OP_code;                //0: AND, 1: OR
        int         delay;
        vector<Wire*> out;
    };

    struct Wire {
        string  Wire_ID;
        bool    inv;                        //0: not invert, 1: inversion
        int     req_time, arr_time, slack;
        Gate    *next;
    };
    
    private:
        string module_name;
        vector<Wire> list_wire;
        vector<Gate> list_gate;
        int no_gate;
        int no_wire;
    public:
        Graph();
        ~Graph();
        void add_Gate();
        void add_Wire();
        void get_Predecessors(string);
        void get_Successors(string);
        void printGraph();
};

#endif