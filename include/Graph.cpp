#include "./Graph.h"

Graph::Graph() {
    list_input.resize(0);
    list_gate.resize(0);
    no_wire = 0;
    no_gate = 0;
}

Graph::~Graph() {
    list_gate.empty();
    list_input.empty();
}

void Graph::setModuleName(string name) {
    module_name = name;
}

string Graph::getModuleName(){
    return module_name;
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
            new_input.in_name = in_name[i];
            new_input.nxt_gate.push_back(no_gate);
            list_input.push_back(new_input);
        } else
            list_gate[idx].out.nxt_gate.push_back(no_gate);
    }
    //3rd: Add gate to the list and increase the size of gate_list
    list_gate.push_back(new_gate);
    no_gate++;
}

vector<string> Graph::find_Successor(string output_name) {
    vector<string> successor;
    //1st: Check index of the gate with coressponding name
    int idx = find_gate_idx(output_name);
    //2nd: Check if exists gate ?
    if (idx != -1) { 
    //3rd: If exist gate then push_back gate.ouput_name to the successor list
        for (int i = 0; i < list_gate[idx].out.nxt_gate.size(); i++) 
            successor.push_back(list_gate[i].output_name);
    }
    return successor;
}

vector<string> Graph::find_Predecessor(string output_name) {
    vector<string> precessor;
    //1st: Check index of the gate with coressponding name
    int idx = find_gate_idx(output_name);
    //2nd: Check if exists gate ?
    if (idx != -1) {
    //3rd: If exist gate then travel along the gate list to check the nxt_gate list of the gate then push_back to precessor
        for (int i = 0; i < no_gate; i++) {
            if (i == idx)
                continue;
            else {
                for (int j = 0; j < list_gate[i].out.nxt_gate.size(); j++) {
                    int check_idx = list_gate[i].out.nxt_gate[j];
                    if (check_idx == idx)
                        precessor.push_back(list_gate[check_idx].output_name);
                }
            }
        }   
    }
    return precessor;
}

void Graph::print_Graph() {
    cout << "--------------------" << endl;
    cout << "Number of Gate: " << no_gate << endl;
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

//---------- Changelog ----------
//