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
	vector<string> funct;

	// Read blif file and extract data
	ifstream 	blif_file(argv[1]);
	int 		mode = 0;						//-2: read comment, -1: read module name, 0: read normal, 1: parse input, 2: parse output, 3: parse function
	int			func_flag = 0;					//0: parse variables, 1: parse operation
	string 		temp, temp_func;
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
			if(func_flag == 1) {
				temp_func.pop_back();
				temp_func = temp_func + "="	+ vari_list[vari_list.size()-1];
				funct.push_back(temp_func);
			}
			func_flag = 0;
			vari_list.clear();
			temp_func.clear();
			continue;
		}
		else if(temp == ".end") {
			if(func_flag == 1) {
				temp_func.pop_back();
				temp_func = temp_func + "="	+ vari_list[vari_list.size()-1];
				funct.push_back(temp_func);
			}
			break;
		}
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
			if (func_flag == 0) {													//1st time, code read the variables, if meet the \ continure, if meet 
				if (temp == "\\") {
					continue;
				} else {
					vari_list.push_back(temp);
					if ((blif_file.peek() == '\n'))
						func_flag = 1;
				}
			}
			else if (func_flag == 1) {
				for (int i = 0; i < temp.size(); i++) {
					if (temp[i] == '0')
						temp_func = temp_func + vari_list[i] +'\'';
					else if (temp[i] == '1')
						temp_func = temp_func + vari_list[i];
				}
				temp_func += "+";
				blif_file >> temp;
			}
		}
	}

	blif_file.close();
	cout << "Done reading file" << endl;

	// Print file result
	cout << "-----------------------------" << endl;
	cout << module_name << endl;
	cout << "Inputs: " ;
	for (int i = 0; i < in_list.size(); i++)
		cout << in_list[i] << " ";
	cout << endl;
	cout << "Outputs: ";
	for (int i = 0; i < out_list.size(); i++)
		cout << out_list[i] << " ";
	cout << endl;
	for (int i = 0; i < funct.size(); i++)
		cout << funct[i] << endl;
	cout << "-----------------------------" << endl;
	return 0;
}

// Changelog
// Still error when have the comment at the end of line