#include <iostream>
#include "../include/Scheduling.h"
using namespace std;

int main(int argc, char* argv[]) {
	
	Scheduling circuit;
	circuit.read_blif_file(argv[2]);
	circuit.print_Graph();
	
	string mode = argv[1];

	if (mode == "-e")
		circuit.ilp_scheduling(stoi(argv[3]), stoi(argv[4]), stoi(argv[5]));
	else if (mode == "-h")
		circuit.list_scheduling(stoi(argv[3]), stoi(argv[4]), stoi(argv[5]));
	else
		cout << "Cannot detect which algorithm used for scheduling!!" << endl;
	circuit.print_Schedule();
	
	return 0;
}