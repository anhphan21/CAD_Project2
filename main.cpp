#include <iostream>
#include <sstream>
#include <fstream>
#include "./include/Graph.h"
using namespace std;



int main(int argc, char* argv[]) {

	int AND_MAX;
	int OR_MAX;
	int INV_MAX;

	string module_name;
	vector<string> in_list, out_list;

	// Read blif file and extract data
	ifstream 	blif_file(argv[1]);
	int 		mode = 0;						//-2: read comment, -1: read module name, 0: read normal, 1: parse input, 2: parse output, 3: parse function
	string 		temp;

	
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
			continue;
		}
		else if(temp == ".end") break;

		if (mode == 1) {
			if (temp == "\\")
				continue;
			else
				in_list.push_back(temp);
		}
		else if (mode == 2) {
			if (temp == "\\")
				continue;
			else
				out_list.push_back(temp);
		}
		else if (mode == 3) {
			
		}
	}

	blif_file.close();
	cout << "Done read file" << endl;

	// Print file result
	cout << module_name << endl;
	cout << "Inputs: " ;
	for (int i = 0; i < in_list.size(); i++)
		cout << in_list[i] << " ";
	cout << endl;
	cout << "Outputs: ";
	for (int i = 0; i < out_list.size(); i++)
		cout << out_list[i] << " ";
	cout << endl;
	cout << "-----------------------------" << endl;
	return 0;
}