#include "./Graph.h"
//Done
Graph::Graph() {
    list_gate.resize(0);
    no_node = 0;
    no_input = 0;
}

Graph::~Graph() {
    list_gate.clear();
}

void Graph::setModuleName(string name)  { module_name = name; }

string Graph::getModuleName()   { return module_name; }

int Graph::get_wire_idx(string output_name) {
    for (int i = 0; i < no_node; i++) {
        if (list_gate[i].out.wire_name == output_name)
            return i;
    }
    return -1;
}

string Graph::get_wire_name(int idx) { return list_gate[idx].out.wire_name; } 

vector<int> Graph::get_gate_inputs(int gate_idx) {
    return list_gate[gate_idx].pre_gate;
}

void Graph::add_Input(string in_name) {
    Gate new_input;
    new_input.operation = -1;               // -1 for input
    new_input.out.wire_name = in_name;
    new_input.out.nxt_gate.resize(0);
    new_input.out.arr_time = 0;
    new_input.out.req_time = 0;
    new_input.pre_gate.resize(0);
    new_input.out.slack = new_input.out.req_time - new_input.out.arr_time;
    list_gate.push_back(new_input);
    no_node++;
    no_input++;
}

void Graph::add_Output(string out_name) {
    Gate new_gate;
    new_gate.out.wire_name = out_name;
    new_gate.out.nxt_gate.resize(0);  
    list_gate.push_back(new_gate);
    no_node++;
}

void Graph::add_Gate(vector<string> in_name, string out_name, int op) {
    //1st: Check if exist gate with same output or ifnot then create Gate with the init parameter
    int gate_idx = get_wire_idx(out_name);
    if (gate_idx == -1) {
        Gate new_gate;
        new_gate.out.wire_name = out_name;
        new_gate.operation = op; 
        new_gate.out.nxt_gate.resize(0);
        list_gate.push_back(new_gate);
        gate_idx = no_node;
        no_node++;
    }
    else 
        list_gate[gate_idx].operation = op;     //for case gate is output 

    //2nd: Add connection to the new gate to previous inputs/gates
    for (int i = 0; i < in_name.size(); i++) {
        int idx = get_wire_idx(in_name[i]);
        list_gate[gate_idx].pre_gate.push_back(idx);
        list_gate[idx].out.nxt_gate.push_back(gate_idx);
    }
}

void Graph::add_Inv(string in_name, string out_name) {
    int gate_idx = get_wire_idx(out_name);
    if (gate_idx == -1) {
        Gate new_gate;
        new_gate.out.wire_name = out_name;
        new_gate.operation = 2;
        new_gate.out.nxt_gate.resize(0);
        list_gate.push_back(new_gate);
        gate_idx = no_node;
        no_node++;
    }
    else {
        list_gate[gate_idx].operation = 2;
    }

    int idx = get_wire_idx(in_name);
    list_gate[idx].out.nxt_gate.push_back(gate_idx);
    list_gate[gate_idx].pre_gate.push_back(idx);
}

vector<int> Graph::get_successor_gate(int gate_idx) {
    vector<int> successor;
    for (int i = 0; i < list_gate[gate_idx].out.nxt_gate.size(); i++) {
        successor.push_back(list_gate[gate_idx].out.nxt_gate[i]);
    }
    return successor;
}

vector<int> Graph::get_predecessor_gate(int gate_idx) {
    vector<int> precessor;
    for (int i = 0; i < list_gate[gate_idx].pre_gate.size(); i++) {
        if (list_gate[list_gate[gate_idx].pre_gate[i]].operation != -1)
            precessor.push_back(list_gate[gate_idx].pre_gate[i]);
    }
    return precessor;
}

vector<int> Graph::get_predecessor_gate_n_input(int gate_idx) {
    return list_gate[gate_idx].pre_gate;
}

vector<int> Graph::get_circuit_outputs() {
    vector<int> outputs;
    for (int i = 0; i < no_node; i++) {
        if (list_gate[i].out.nxt_gate.size() == 0) {
            outputs.push_back(i);
        }
    }
    return outputs;
}

void Graph::print_Graph() {
    cout << "----------------------------------------" << endl;
    cout << "Module name: " << module_name << endl;
    cout << "Input: ";
    for (int i = 0; i < no_node; i++)
        if (list_gate[i].operation == -1)
            cout << list_gate[i].out.wire_name << " ";
    cout << endl;

    vector<int> outputs = get_circuit_outputs();
    cout << "Output: ";
    for (int i = 0; i < outputs.size(); i++)
        cout << get_wire_name(outputs[i]) << " ";
    cout << endl;

    cout << "Gate:" << endl;
    for (int i = 0; i < no_node; i++) {
        if (list_gate[i].operation != -1) {
            cout << " " << i << ". " << list_gate[i].out.wire_name << "=" ;
            
            int op_code = list_gate[i].operation;
            string op_code_s;
            if (op_code == 2)
                op_code_s = "'";
            else if (op_code == 0)
                op_code_s = "."; 
            else if (op_code == 1)
                op_code_s = "+";
            else
                op_code_s = "";

            vector<int> inputs = get_gate_inputs(get_wire_idx(list_gate[i].out.wire_name));     
            for (int j = 0; j < inputs.size()-1; j++)
                cout << get_wire_name(inputs[j]) << op_code_s;
            cout << get_wire_name(inputs.back());
            if (inputs.size() == 1) cout << op_code_s;
            cout << endl;
        }
    }
}

void Graph::topology_sort_util(int v, bool visited[], stack<int>& Stack) {
    visited[v] = true;

    vector<int>::iterator i;
    for (i = list_gate[v].out.nxt_gate.begin(); i != list_gate[v].out.nxt_gate.end(); i++)
        if (!visited[*i])
            topology_sort_util(*i, visited, Stack);
    
    Stack.push(v);
}

void Graph::topology_sort() {
    stack<int> Stack;
    bool* visited = new bool[no_node];

    for (int i = 0; i < no_node; i++)
        visited[i] = false;

    for (int i = 0; i < no_node; i++)
        if (visited[i] == false)
            topology_sort_util(i, visited, Stack);

    while (Stack.empty() == false) {
        sorted_gate_list.push_back(Stack.top());
        Stack.pop();
    }

    delete [] visited;
}

void Graph::read_blif_file(string dir_path) {
    ifstream 	blif_file(dir_path);
	int 		mode = 0;						//-2: read comment, -1: read module name, 0: read normal, 1: parse input, 2: parse output, 3: parse function
	bool		func_flag = 0;					//0: parse variables, 1: parse operation
	string 		temp;							//Store word from file waiting for process
	vector<string> temp_func;
	vector<string> vari_list;

	
	while(1) {
		blif_file >> temp;

		if (temp[0] == '#') {					//check if we get the comment part
			getline(blif_file, temp);
			continue;
		}
		else if (temp == ".model")	{			//get module name
			if (blif_file.peek() != '\n')
				blif_file >> module_name;
			else
				module_name = dir_path;
				setModuleName(module_name);
			continue;
		}
		else if (temp == ".inputs") {
			mode = 1;
			continue;
		}
		else if (temp == ".outputs") {
			mode = 2;
			continue;
		}
		else if (temp == ".names") {
			mode = 3;
			if(func_flag) {											//Check if ther's still previous equation then add it to the data structure
				if (temp_func.size() == 1) {		
					if (vari_list.size() == 2)
						add_Inv(vari_list[0], vari_list[1]);
					else {
						for (int i = 0; i < vari_list.size()-1; i++) {		//Ignore output variable
							if(temp_func[0][i] == '0') {
								string out_name = vari_list[i] + "\'";
								add_Inv(vari_list[i], out_name);
								vari_list[i] = out_name;
							}
						}
						string out_name = vari_list.back();
						vari_list.pop_back();
						add_Gate(vari_list, out_name, 0);
					}
				}
				else {
					for (int i = 0; i < temp_func.size(); i++) {
						for (int j = 0; j < vari_list.size()-1; j++) {
							if (temp_func[i][j] == '0') {
								string out_name = vari_list[i] + "\'";
								add_Inv(vari_list[i], out_name);
								vari_list[i] = out_name;
							}	
						}
					}
					string out_name = vari_list.back();
					vari_list.pop_back();
					add_Gate(vari_list, out_name, 1);
				}
			}

			func_flag = 0;
			vari_list.clear();
			temp_func.clear();
			continue;
		}
		else if(temp == ".end") {
			if(func_flag) {											//Check if ther's still previous equation then add it to the data structure
				if (temp_func.size() == 1) {		
					if (vari_list.size() == 2)
						add_Inv(vari_list[0], vari_list[1]);
					else {
						for (int i = 0; i < vari_list.size()-1; i++) {		//Ignore output variable
							if(temp_func[0][i] == '0') {
								string out_name = vari_list[i] + "\'";
								add_Inv(vari_list[i], out_name);
								vari_list[i] = out_name;
							}
						}
						string out_name = vari_list.back();
						vari_list.pop_back();
						add_Gate(vari_list, out_name, 0);
					}
				}
				else {
					for (int i = 0; i < temp_func.size(); i++) {
						for (int j = 0; j < vari_list.size()-1; j++) {
							if (temp_func[i][j] == '0') {
								string out_name = vari_list[i] + "\'";
								add_Inv(vari_list[i], out_name);
								vari_list[i] = out_name;
							}	
						}
					}
					string out_name = vari_list.back();
					vari_list.pop_back();
					add_Gate(vari_list, out_name, 1);
				}
			}
			break;
		}
        else if ((temp == ".default_input_arrival") || (temp == ".default_output_required") || (temp == ".default_input_drive") 
                || (temp == ".default_output_load ") || (temp == ".default_max_input_load")) {
            mode = 0;
            getline(blif_file, temp);
			continue;
        }
		
		if (mode == 1) {
			if (temp == "\\")
				continue;
			else
				add_Input(temp);
		}
		else if (mode == 2) {
			if (temp == "\\")
				continue;
			else
				add_Output(temp);
		}
		else if (mode == 3) {
			if (!func_flag) {													//1st time, code read the variables, if meet the \ continure, if meet 
				if (temp == "\\") {
					continue;
				} else {
					vari_list.push_back(temp);
					if ((blif_file.peek() == '\n'))
						func_flag = 1;
				}
			}
			else {
				temp_func.push_back(temp);
				blif_file >> temp;
			}
		}
	}
	blif_file.close();
    topology_sort();
}