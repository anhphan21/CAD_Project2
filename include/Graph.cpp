#include "./Graph.h"

Graph::Graph() {
    list_input.resize(0);
    list_gate.resize(0);
    no_gate = 0;
}

Graph::~Graph() {
    list_gate.empty();
    list_input.empty();
}

void Graph::setModuleName(string name)  { module_name = name; }

string Graph::getModuleName()   { return module_name; }

int Graph::get_gate_idx(string output_name) {
    for (int i = 0; i < no_gate; i++) {
        if (list_gate[i].out.wire_name == output_name)
            return i;
    }
    // cout << "Cannot find the gate corresponding to wire " << output_name << "!" << endl;
    return -1;
}

int Graph::get_input_idx(string in_name) {
    for (int i = 0; i < list_input.size(); i++) {
        if (list_input[i].wire_name == in_name)
            return i;
    }
    return -1;
}

string Graph::get_gate_name(int idx) {
    return list_gate[idx].out.wire_name;
}

vector<string> Graph::get_gate_inputs(string gate_name) {
    vector<string> inputs;

    int gate_idx = get_gate_idx(gate_name);
    
    for (int i = 0; i < list_input.size(); i++) {
        for (int j = 0; j < list_input[i].nxt_gate.size(); j++) {
            if (list_input[i].nxt_gate[j] == gate_idx) {
                inputs.push_back(list_input[i].wire_name);
            }
        }
    }

    for (int i = 0; i < list_gate.size(); i++) {
        for (int j = 0; j < list_gate[i].out.nxt_gate.size(); j++) {
            if (list_gate[i].out.nxt_gate[j] == gate_idx)
                inputs.push_back(list_gate[i].out.wire_name);
        }
    }
    return inputs;
}

void Graph::add_Input(string in_name) {
    Wire new_input;
    new_input.wire_name = in_name;
    new_input.nxt_gate.resize(0);
    list_input.push_back(new_input);
}

void Graph::add_Output(string out_name) {
    Gate new_gate;
    new_gate.out.wire_name = out_name;
    new_gate.out.nxt_gate.resize(0);  
    list_gate.push_back(new_gate);
    no_gate++;
}

void Graph::add_Gate(vector<string> in_name, string out_name, int op) {
    //1st: Check if exist gate with same output or ifnot then create Gate with the init parameter
    int gate_idx = get_gate_idx(out_name);
    if (gate_idx == -1) {
        Gate new_gate;
        new_gate.out.wire_name = out_name;
        new_gate.operation = op; 
        new_gate.out.nxt_gate.resize(0);
        list_gate.push_back(new_gate);
        gate_idx = no_gate;
        no_gate++;
    }
    else {
        list_gate[gate_idx].operation = op;     //for case gate is output 
    }

    //2nd: Add connection to the new gate to previous inputs/gates
    for (int i = 0; i < in_name.size(); i++) {
        int idx = get_gate_idx(in_name[i]);
        if (idx == -1) {
            idx = get_input_idx(in_name[i]);
            list_input[idx].nxt_gate.push_back(gate_idx);
        } else
            list_gate[idx].out.nxt_gate.push_back(gate_idx);
    }
}

void Graph::add_Inv(string in_name, string out_name) {
    int gate_idx = get_gate_idx(out_name);
    if (gate_idx == -1) {
        Gate new_gate;
        new_gate.out.wire_name = out_name;
        new_gate.operation = -1;
        new_gate.out.nxt_gate.resize(0);
        list_gate.push_back(new_gate);
        gate_idx = no_gate;
        no_gate++;
    }
    else {
        list_gate[gate_idx].operation = -1;
    }

    int idx = get_gate_idx(in_name);
    if (idx == -1) {
        idx = get_input_idx(in_name);
        list_input[idx].nxt_gate.push_back(gate_idx);
    } else
        list_gate[idx].out.nxt_gate.push_back(gate_idx);
}

vector<int> Graph::get_successor_gate(string output_name) {
    vector<int> successor;
    //1st: Check index of the gate with coressponding name
    int gate_idx = get_gate_idx(output_name);
    //2nd: Check if exists gate ?
    if (gate_idx != -1) {
    //3rd: If exist gate then push_back gate.ouput_name to the successor list
        for (int i = 0; i < list_gate[gate_idx].out.nxt_gate.size(); i++) 
            successor.push_back(list_gate[gate_idx].out.nxt_gate[i]);
    }
    return successor;
}

vector<int> Graph::get_predecessor_gate(string output_name) {
    vector<int> precessor;
    //1st: Check index of the gate with coressponding name
    int gate_idx = get_gate_idx(output_name);
    
    if (gate_idx != -1) {
    //2rd: If exist gate then travel along the gate list to check the nxt_gate list of the gate then push_back to precessor
        for (int i = 0; i < no_gate; i++) {
            if (i == gate_idx)
                continue;
            else {
                for (int j = 0; j < list_gate[i].out.nxt_gate.size(); j++) {
                    int check_idx = list_gate[i].out.nxt_gate[j];
                    if (check_idx == gate_idx)
                        precessor.push_back(i);
                }
            }
        }   
    }
    return precessor;
}

void Graph::print_Gate(int idx) {
    cout << "----------" << endl;
    cout << "Gate: " << list_gate[idx].out.wire_name << endl;
    cout << "Input: ";
    for (int i = 0; i < list_gate[idx].out.nxt_gate.size(); i++) {
        cout << list_gate[idx].out.nxt_gate[i] << " ";
    }
    cout << endl << "----------" << endl;
}

// void Graph::print_Input(int idx) {
//     cout << "----------" << endl;
//     cout << "Name: " << list_input[idx].wire_name << endl;
//     cout << "Nxt_gate: ";
//     for (int i = 0; i < list_input[idx].nxt_gate.size(); i++) {
//         cout << list_input[idx].nxt_gate[i] << " ";
//     }
//     cout << endl;
// }

void Graph::print_Graph() {
    cout << "--------------------" << endl;
    cout << "Module name: " << module_name << endl;
    cout << "Number of Gate: " << no_gate << endl;
    cout << "Input: ";
    for (int i = 0; i < list_input.size(); i++)
        cout << list_input[i].wire_name << " ";
    cout << endl;
    
    cout << "Gate:" << endl;
    
    for (int i = 0; i < no_gate; i++) {
        cout << i+1 << ".\t" <<  list_gate[i].out.wire_name << "=" ;

        int op_code = list_gate[i].operation;
        string op_code_s;
        if (op_code == -1)
            op_code_s = "\'";
        else if (op_code == 0)
            op_code_s = "."; 
        else
            op_code_s = "+";

        vector<string> inputs = get_gate_inputs(list_gate[i].out.wire_name);     
        for (int j = 0; j < inputs.size()-1; j++)
            cout << inputs[j] << op_code_s;
        cout << inputs.back() << endl;
    }

    cout << "--------------------" << endl;
}