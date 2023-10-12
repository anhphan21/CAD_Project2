#include <iostream>
#include <sstream>
#include <fstream>
#include "./include/Graph.h"
using namespace std;

int main(int argc, char* argv[]) {
	int AND_MAX;
	int OR_MAX;
	int INV_MAX;

	Graph circuit;
	string module_name;

	// Read blif file and extract data
	ifstream 	blif_file(argv[1]);
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
				module_name = argv[1];
				circuit.setModuleName(module_name);
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
						circuit.add_Inv(vari_list[0], vari_list[1]);
					else {
						for (int i = 0; i < vari_list.size()-1; i++) {		//Ignore output variable
							if(temp_func[0][i] == '0') {
								string out_name = vari_list[i] + "\'";
								circuit.add_Inv(vari_list[i], out_name);
								vari_list[i] = out_name;
							}
						}
						string out_name = vari_list.back();
						vari_list.pop_back();
						circuit.add_Gate(vari_list, out_name, 0);
					}
				}
				else {
					for (int i = 0; i < temp_func.size(); i++) {
						for (int j = 0; j < vari_list.size()-1; j++) {
							if (temp_func[i][j] == '0') {
								string out_name = vari_list[i] + "\'";
								circuit.add_Inv(vari_list[i], out_name);
								vari_list[i] = out_name;
							}	
						}
					}
					string out_name = vari_list.back();
					vari_list.pop_back();
					circuit.add_Gate(vari_list, out_name, 1);
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
						circuit.add_Inv(vari_list[0], vari_list[1]);
					else {
						for (int i = 0; i < vari_list.size()-1; i++) {		//Ignore output variable
							if(temp_func[0][i] == '0') {
								string out_name = vari_list[i] + "\'";
								circuit.add_Inv(vari_list[i], out_name);
								vari_list[i] = out_name;
							}
						}
						string out_name = vari_list.back();
						vari_list.pop_back();
						circuit.add_Gate(vari_list, out_name, 0);
					}
				}
				else {
					for (int i = 0; i < temp_func.size(); i++) {
						for (int j = 0; j < vari_list.size()-1; j++) {
							if (temp_func[i][j] == '0') {
								string out_name = vari_list[i] + "\'";
								circuit.add_Inv(vari_list[i], out_name);
								vari_list[i] = out_name;
							}	
						}
					}
					string out_name = vari_list.back();
					vari_list.pop_back();
					circuit.add_Gate(vari_list, out_name, 1);
				}
			}
			break;
		}
		
		if (mode == 1) {
			if (temp == "\\")
				continue;
			else
				circuit.add_Input(temp);
		}
		else if (mode == 2) {
			if (temp == "\\")
				continue;
			else
				circuit.add_Output(temp);
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
	cout << "Done reading file" << endl;

	//Check result
	circuit.print_Graph();
	
	vector<int> pre_check = circuit.get_predecessor_gate("o");
	for (int i = 0; i < pre_check.size(); i++)
		cout << circuit.get_gate_name(pre_check[i]) << " ";
	cout << endl;

	vector<int> suc_check = circuit.get_successor_gate("i");
	for (int i = 0; i < suc_check.size(); i++)
		cout << circuit.get_gate_name(suc_check[i]) << " ";
	cout << endl;

	vector<int> out_check = circuit.get_circuit_outputs();
	for (int i = 0; i < out_check.size(); i++)
		cout << circuit.get_gate_name(out_check[i]) << " ";
	cout << endl;

	// circuit.list_scheduling();
	// circuit.print_Schedule();

	return 0;
}