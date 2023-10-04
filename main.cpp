#include <iostream>
#include <sstream>
#include <fstream>
#include "./include/Graph.h"
using namespace std;

vector<string> input, output;

int main(int argc, char* argv[]) {

	int AND_MAX;
	int OR_MAX;
	int INV_MAX;

	string module_name;

	// Read blif file and extract data
	ifstream 	blif_file(argv[1]);
	int 		mode = 0;						//-2: read comment, -1: read module name, 0: read normal, 1: parse input, 2: parse output, 3: parse function
	string 		temp;

	while(1) {
		blif_file >> temp;
		if (temp[0] == '#') {					//check if we get the comment part
			mode = -2;
		}
		else if (temp == ".model")	{			//get module name
			// if (blif_file.peek() != '\n')
			// 	blif_file >> module_name;
			// else
			// 	module_name = argv[1];
			mode = -1;
		}
		else if (temp == ".inputs") {
			mode = 1;
		}
		else if (temp == ".outputs") {
			mode = 2;
		}
		else if (temp == ".names") {
			mode = 3;
		}
		else if(temp == ".end") break;
		else
			mode = 0;
		cout << temp << endl;
		
		if (mode == -2) {
			getline(blif_file, temp);
			continue;
		}
		else if (mode == -1) {
			if (blif_file.peek() != '\n')
				blif_file >> module_name;
			else
				module_name = argv[1];
		}
		else if (mode == 0) {
			continue;
		}
		else if (mode == 1) {
			if (temp == "\\")
				continue;
			else
				input.push_back(temp);
		}
		else if (mode == 2) {
			if (temp == "\\")
				continue;
			else
				input.push_back(temp);
		}
		else if (mode == 3) {
			cout << "f" << endl;
		}
		
	}
	blif_file.close();

	// Print file result
	cout << module_name << endl;
	cout << "Inputs: " ;
	for (int i = 0; i < input.size()-1; i++)
		cout << input[i] << " ";
	cout << endl;
	cout << "Outputs: ";
	for (int i = 0; i < output.size()-1; i++)
		cout << input[i] << " ";
	cout << endl;
	cout << "-----------------------------" << endl;
	return 0;
}