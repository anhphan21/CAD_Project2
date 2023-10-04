#include <iostream>
#include <string>
#include <list>
using namespace std;

class Graph{
    struct Wire;
    
    struct  Gate {
        string      Gate_ID;
        bool        OP_code;                //0: AND, 1: OR
        int         delay;
        list<Wire*> out;
    };

    struct Wire {
        string  Wire_ID;
        bool    inv;                        //0: not invert, 1: inversion
        int     req_time, arr_time, slack;
        Gate    *next;
    };
    
    private:
        list<Wire> list_wire;
        list<Gate> list_gate;
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

int main() {
    Graph test;
    return 0;
}