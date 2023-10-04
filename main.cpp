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
	ifstream blif_file(argv[1]);
	string temp;
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
			getline(blif_file, temp_line);
			while (1) {
			}
		}
		else if (temp == ".outputs") {
			// do {
			// 	blif_file >> temp;
			// 	if (temp == "\\") continue;
			// 	else output.push_back(temp);
			// } while ((temp == "\\" && (blif_file.peek() == '\n')) || ((temp != "\\") && (blif_file.peek() == '\n')));
		}
		else if(temp == ".end") break;
		cout << temp << endl;
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