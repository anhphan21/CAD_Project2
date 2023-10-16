#ifndef SCHEDULING_H
#define SCHEDULING_H

#include "./Graph.h"
#include <algorithm>
#include <stack>

class Scheduling : public Graph {
protected:
    bool    debug   = 1;
    int AND_CONST   = 0;
    int OR_CONST    = 0;
    int NOT_CONST   = 0;

    void check_resource_available(vector<int>&, int);
    vector<vector<int>> schedule;

public:
    Scheduling();
    ~Scheduling();

    void set_AND_CONST(int AND_CONST) { this->AND_CONST = AND_CONST; }
    void set_OR_CONST(int OR_CONST) { this->OR_CONST = OR_CONST; }
    void set_NOT_CONST(int NOT_CONST) { this->NOT_CONST = NOT_CONST; }

    int get_AND_CONST() { return AND_CONST; }
    int get_OR_CONST() { return OR_CONST; }
    int get_NOT_CONST() { return NOT_CONST; }

    void reset_schedule();
    vector<vector<int>> get_schedule() { return schedule; }

    vector<int> get_gate_only();
    void remove_scheduled_gate(vector<int>&);

    void alap_scheduling(int);
    void asap_scheduling();
    void list_scheduling(int, int, int);
    void ilp_scheduling(int, int, int);
    void print_Schedule();
};

#endif