#include "./Graph.h"

void Graph::setModuleName(string name) {
    module_name = name;
}

int Graph::find_gate_idx(string output_name) {
    for (int i = 0; i < no_gate; i++) {
        if (list_gate[i].output_name == output_name)
            return i;
    }
    cout << "Cannot find the gate corresponding to wire " << output_name << "!" << endl;
    return -1;
}

void Graph::add_Gate(vector<string> in_name, string out_name, int op) {
    //1st: Create Gate with the init parameter
    Gate new_gate;
    new_gate.output_name = out_name;
    new_gate.operation = op; 
    //2nd: Add output wire to the existed gate or create new input
    for (int i = 0; i < in_name.size(); i++) {
        int idx = find_gate_idx(in_name[i]);
        if (idx == -1) {
            Input new_input;
            new_input.nxt_gate.push_back(no_gate);
            list_input.push_back(new_input);
        } else
            list_gate[idx].out.nxt_gate.push_back(no_gate);
    }
    //3rd: Add gate to the list and increase the size of gate_list
    list_gate.push_back(new_gate);
    no_gate++;
}

void Graph::printGraph() {
    cout << "--------------------" << endl;
    cout << "Input: ";
    for (int i = 0; i < list_input.size(); i++)
        cout << list_input[i].in_name << " ";
    cout << endl;
    cout << "Gate: ";
    for (int i = 0; i < no_gate; i++) {
        cout <<  list_gate[i].output_name << " ";
    }
    cout << endl;
    cout << "--------------------" << endl;
}