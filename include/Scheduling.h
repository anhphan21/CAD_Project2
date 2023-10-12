#ifndef SCHEDULING_H
#define SCHEDULING_H

#include "./Graph.h"

class Scheduling : public Graph {
private:
    int AND_CONST   = 0;
    int OR_CONST    = 0;
    int NOT_CONST   = 0;

    vector<string> schedule;
    vector<int> list_inputs;
    vector<int> list_outputs;
public:
    Scheduling();
    ~Scheduling();
    void set_AND_CONST(int);
    void set_OR_CONST(int);
    void set_INV_CONST(int);
    int get_AND_CONST();
    int get_OR_CONST();
    int GET_NOT_CONST();

    void update_wire_slack(int);
    void update_gate_arr_time(int);
    void update_gate_req_time(int);
    bool check_predecessor_scheduled(vector<int>, int);
    void alap();
    void asap();
    void list_scheduling();
    void print_Schedule();
};

#endif