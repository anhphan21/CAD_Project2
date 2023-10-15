#include <iostream>
#include "./include/Scheduling.h"
using namespace std;

int main(int argc, char* argv[]) {
	int AND_MAX;
	int OR_MAX;
	int INV_MAX;

	Scheduling circuit;
	circuit.read_blif_file(argv[1]);

	//Check result
	circuit.print_Graph();
	
	// vector<int> pre_check = circuit.get_predecessor_gate(14);
	// cout << circuit.get_wire_name(14) << ": ";
	// for (int i = 0; i < pre_check.size(); i++)
	// 	cout << circuit.get_wire_name(pre_check[i]) << " ";
	// cout << endl;

	// vector<int> suc_check = circuit.get_successor_gate(5);
	// cout << circuit.get_wire_name(5) << ": ";
	// for (int i = 0; i < suc_check.size(); i++)
	// 	cout << circuit.get_wire_name(suc_check[i]) << " ";
	// cout << endl;

	// vector<int> out_check = circuit.get_circuit_outputs();
	// for (int i = 0; i < out_check.size(); i++)
	// 	cout << circuit.get_wire_name(out_check[i]) << " ";
	// cout << endl;

	// circuit.list_scheduling(stoi(argv[2]), stoi(argv[3]), stoi(argv[4]));
	// circuit.print_Schedule();
	// circuit.asap_scheduling();
	// circuit.print_Schedule();
	circuit.alap_scheduling(4);
	circuit.print_Schedule();

	return 0;
}