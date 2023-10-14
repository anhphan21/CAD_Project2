#include <iostream>
#include "./include/Graph.h"
using namespace std;

int main(int argc, char* argv[]) {
	int AND_MAX;
	int OR_MAX;
	int INV_MAX;

	Graph circuit;
	// Read blif file and extract data
	circuit.read_blif_file(argv[1]);

	//Check result
	circuit.print_Graph();
	
	vector<int> pre_check = circuit.get_predecessor_gate(8);
	cout << circuit.get_wire_name(8) << ": ";
	for (int i = 0; i < pre_check.size(); i++)
		cout << circuit.get_wire_name(pre_check[i]) << " ";
	cout << endl;

	vector<int> suc_check = circuit.get_successor_gate(5);
	cout << circuit.get_wire_name(5) << ": ";
	for (int i = 0; i < suc_check.size(); i++)
		cout << circuit.get_wire_name(suc_check[i]) << " ";
	cout << endl;

	vector<int> out_check = circuit.get_circuit_outputs();
	for (int i = 0; i < out_check.size(); i++)
		cout << circuit.get_wire_name(out_check[i]) << " ";
	cout << endl;

	// circuit.topology_sort();
	// // circuit.cal_alap();
	// // circuit.cal_asap();
	// // circuit.cal_slack();
	// circuit.list_scheduling(2, 1, 1);
	// circuit.print_Schedule();

	return 0;
}