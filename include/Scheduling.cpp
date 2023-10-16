#include "./Scheduling.h"

Scheduling::Scheduling() {
    schedule.resize(0);
}

Scheduling::~Scheduling() {
    schedule.clear();
}

void Scheduling::reset_schedule() {
    for (int i = 0; i < no_node; i++) {
        list_gate[i].step = 0;
    }
    schedule.clear();
}

void Scheduling::alap_scheduling(int max_latency) {
    schedule.clear();
    reset_schedule();
    int no_gate = no_node - no_input;
    int step = max_latency;
    int no_scheduled_gate = 0;
    vector<int> sche_this_step;                         //temp_var to push to schedule vector
    vector<int> not_scheduled_list = get_gate_only();
    vector<int> rdy;
    stack<vector<int>>  reverse_schedule;

    // remove output from not_scheduled_list
    for (auto i = not_scheduled_list.begin(); i != not_scheduled_list.end();) {
        
        if (list_gate[*i].out.nxt_gate.size() == 0) {
            list_gate[*i].step = step;
            no_scheduled_gate++;
            sche_this_step.push_back(*i);
            i = not_scheduled_list.erase(i);
        }
        else
            i++;
    }
    reverse_schedule.push(sche_this_step);

    while (no_scheduled_gate < no_gate) {
        sche_this_step.clear();
        step--;
        //Select vi with all scheduled successors
        for (auto i = not_scheduled_list.begin(); i != not_scheduled_list.end();) {
            vector<int> suc_check = get_successor_gate(*i);
            int j = 0;
            for (j = 0; j < suc_check.size(); j++) {
                if (list_gate[suc_check[j]].step != 0)
                    continue;
                else
                    break;
            }
            if (j == suc_check.size()) {
                rdy.push_back(*i);
                i = not_scheduled_list.erase(i);
            }
            else
                i++;
        }
        //Schedule vi
        for (auto i = rdy.begin(); i != rdy.end();) {
            vector<int> suc_check = get_successor_gate(*i);
            
            int min = list_gate[suc_check[0]].step;
            for (int j = 1; j < suc_check.size(); j++) {
                if (list_gate[suc_check[j]].step < min)
                    min = list_gate[suc_check[j]].step;
            }

            list_gate[*i].step = min - list_gate[*i].delay;
            sche_this_step.push_back(*i);
            i = rdy.erase(i);
            no_scheduled_gate++;
        }

        reverse_schedule.push(sche_this_step);
    }

    while (!reverse_schedule.empty()) {
        schedule.push_back(reverse_schedule.top());
        reverse_schedule.pop();
    }
}

void Scheduling::asap_scheduling() {
    schedule.clear();
    reset_schedule();
    int no_gate = no_node - no_input;
    int step = 0;
    int no_scheduled_gate = 0;                         //check for end of schedule    
    vector<int> sche_this_step;                        //temp_var to push to schedule vector
    vector<int> not_scheduled_list = get_gate_only();
    vector<int> rdy;

    while (no_scheduled_gate < no_gate) {
        sche_this_step.clear();
        step++;
        //Select vi with all scheduled predecessors
        for (auto i = not_scheduled_list.begin(); i != not_scheduled_list.end();) {
            vector<int> pre_check = get_predecessor_gate(*i);
            int j = 0;
            for (j = 0; j < pre_check.size(); j++) {
                if (list_gate[pre_check[j]].step != 0)
                    continue;
                else
                    break;
            }

            if (j == pre_check.size()) {
                rdy.push_back(*i);
                i = not_scheduled_list.erase(i);
            }
            else
                i++;
        }

        //Schedule vi
        for (auto i = rdy.begin(); i != rdy.end();) {
            vector<int> pre_check = get_predecessor_gate_n_input(*i);
            
            int max = list_gate[pre_check[0]].step;
            for (int j = 1; j < pre_check.size(); j++) {
                if (list_gate[pre_check[j]].step > max)
                    max = list_gate[pre_check[j]].step;
            }
            list_gate[*i].step = max + list_gate[*i].delay;

            sche_this_step.push_back(*i);
            i = rdy.erase(i);
            no_scheduled_gate++;
            
        }

        //Update gate list
        for (auto i = not_scheduled_list.begin(); i != not_scheduled_list.end();) {
            if (list_gate[*i].step != 0)
                i = not_scheduled_list.erase(i);
            else
                i++;
        }
        
        schedule.push_back(sche_this_step);
    }
}

void Scheduling::check_resource_available(vector<int> &vec, int step) {
    for (auto i = vec.begin(); i != vec.end();) {
        if ((step - list_gate[*i].step) == list_gate[*i].delay)
            i = vec.erase(i);
        else
            i++;
    }
}

vector<int> Scheduling::get_gate_only() {
    vector<int> not_scheduled;
    for (int i = 0; i < no_node; i++) {
        if (list_gate[sorted_gate_list[i]].operation != -1)         //-1 for input
            not_scheduled.push_back(sorted_gate_list[i]);
    }
    return not_scheduled;
}

void Scheduling::remove_scheduled_gate(vector<int> &gate_list) {
    for (auto i = gate_list.begin(); i != gate_list.end();) {
            if (list_gate[*i].step != 0)
                i = gate_list.erase(i);
            else
                i++;
    }
}

void Scheduling::list_scheduling(int and_c, int or_c, int not_c) {
    schedule.clear();
    reset_schedule();
    set_AND_CONST(and_c);
    set_OR_CONST(or_c);
    set_NOT_CONST(not_c);
    
    int no_gate = no_node - no_input;
    int step = 0;
    vector<int> sche_this_step;                        //temp_var to push to schedule vector
    vector<int> rdy;                                   //rdy: list node, which its precessors are scheduled 
    int no_scheduled_gate = 0;                         //check for end of schedule
    vector<int> and_rsc(0), not_rsc(0), or_rsc(0);     //chekc for RSC
    vector<int> not_scheduled_list = get_gate_only(); 

    //Get initial rdy vector which gate's inputs are only the inputs of circuit
    for (int i = 0; i < no_gate; i++) {
        vector<int> pre_check = get_predecessor_gate(not_scheduled_list[i]);
        if (pre_check.size() == 0)
            rdy.push_back(not_scheduled_list[i]);
    }
    //Remove the gate which in rdy vector from not_scheduled_list
    for (int i = 0; i < rdy.size(); i++) {
        for (int j = 0; j < not_scheduled_list.size(); j++) {
            if (rdy[i] == not_scheduled_list[j]) {
                not_scheduled_list.erase(not_scheduled_list.begin()+j);
                break;
            }
        }    
    }
    
    while (no_scheduled_gate < no_gate) {
        sche_this_step.clear();
        step++;
        // Check resource are free from pre step
        check_resource_available(and_rsc, step);
        check_resource_available(or_rsc, step);
        check_resource_available(not_rsc, step);
        
        // Schedule gate from rdy list
        for (int i = 0; i < rdy.size(); i++) {
            bool _schedule = 0;
            switch (list_gate[rdy[i]].operation) {
            case 0:
                if (and_rsc.size() < AND_CONST) {
                    _schedule = 1;
                    and_rsc.push_back(rdy[i]);
                }
                break;
            case 1:
                if (or_rsc.size() < OR_CONST) {
                    _schedule = 1;
                    or_rsc.push_back(rdy[i]);
                }
                break;
            case 2:
                if (not_rsc.size() < NOT_CONST) {
                    _schedule = 1;
                    not_rsc.push_back(rdy[i]);
                }
                break;
            default:
                break;
            }
            if (_schedule) {
                no_scheduled_gate++;
                list_gate[rdy[i]].step = step;
                sche_this_step.push_back(rdy[i]);
            }
        }

        //update rdy list and not_schdule_list after scheduling
        remove_scheduled_gate(not_scheduled_list);
        remove_scheduled_gate(rdy);

        //update rdy list with the new gates from not_scheduled_list
        for (int i = 0; i < not_scheduled_list.size(); i++) {
            vector<int> pre_check = get_predecessor_gate(not_scheduled_list[i]);
            int j = 0;
            for (j = 0; j < pre_check.size(); j++) {
                if (list_gate[pre_check[j]].step != 0)
                    continue;
                else
                    break;
            }
            if (j == pre_check.size())
                rdy.push_back(not_scheduled_list[i]);
        }

        schedule.push_back(sche_this_step);
    }
}

void Scheduling::print_Schedule() {
    vector<int> and_rsc;
    vector<int> not_rsc;
    vector<int> or_rsc;
    cout <<  "------ Schedule ------" << endl;
    cout << "\t<And>\t<OR>\t<NOT>" << endl;

    for (int i = 0; i < schedule.size(); i++) {
        and_rsc.clear();
        not_rsc.clear();
        or_rsc.clear();

        for (int j = 0; j < schedule[i].size(); j++) {
            if (list_gate[schedule[i][j]].operation == 0) {
                and_rsc.push_back(schedule[i][j]);
            } 
            else if (list_gate[schedule[i][j]].operation == 1) {
                or_rsc.push_back(schedule[i][j]);
            }
            else if (list_gate[schedule[i][j]].operation == 2) {
                not_rsc.push_back(schedule[i][j]);
            }
            else
                continue;
        }


        cout << "Step " << i+1 << ":\t";
        cout << "{";
        for (int j = 0; j < and_rsc.size(); j++)
            cout << " " << get_wire_name(and_rsc[j]);
        cout << " }\t{";
        for (int j = 0; j < or_rsc.size(); j++)
            cout << " " << get_wire_name(or_rsc[j]);
        cout << " }\t{";
        for (int j = 0; j < not_rsc.size(); j++)
            cout << " " << get_wire_name(not_rsc[j]);
        cout << " }" << endl;
    }
    
    cout << "Latency: " << schedule.size() << endl;
    cout << "End" << endl;
    cout <<  "----------------------" << endl;
}

void Scheduling::ilp_scheduling(int and_c, int or_c, int not_c) {
    struct Operation {
        int gate_idx;           //gate_indx in list gate
        int t_soon, t_late;     //start time for asap and alap
        vector<int> pre_gate;   //contains predescessor gate idx
    };
    vector<Operation> ilp_extract;
    vector<int> gate_list = get_gate_only();
    vector<string>  and_rcs;
    vector<string>  or_rcs;
    vector<string>  not_rcs;
    vector<vector<string>> binary_var;
    vector<string> start_time_constr;
    string  objective;


    asap_scheduling();
    vector<vector<int>> asap = get_schedule();
    int min_step = asap.size();
    for (int i = 0; i < gate_list.size(); i++) {
        Operation new_op;
        new_op.gate_idx = gate_list[i];
        new_op.t_soon = list_gate[gate_list[i]].step;
        new_op.pre_gate = get_predecessor_gate(gate_list[i]);
        ilp_extract.push_back(new_op);
    }
    print_Schedule();

    alap_scheduling(min_step);
    vector<vector<int>> alap = get_schedule();
    for (int i = 0; i < gate_list.size(); i++) {
        ilp_extract[i].t_late = list_gate[gate_list[i]].step;
    }
    print_Schedule();

    if (debug) {
    cout << "--------------------" << endl;
    for (int i = 0; i < ilp_extract.size(); i++)
        cout << get_wire_name(ilp_extract[i].gate_idx) << " " << ilp_extract[i].t_soon << " " << ilp_extract[i].t_late << endl;
    }

    //get binary variable list
    for (int i = 0; i < ilp_extract.size(); i++) {
        vector<string> temp_var, temp_1;
        for (int j = ilp_extract[i].t_soon; j <= ilp_extract[i].t_late; j++) {
            temp_var.push_back("x_" + to_string(ilp_extract[i].gate_idx) + "_" + to_string(j));
            objective = objective + to_string(j) + " " + "x_" + to_string(ilp_extract[i].gate_idx) + "_" + to_string(j) + " + ";
        }
        binary_var.push_back(temp_var);
    }
    
    objective.pop_back();
    objective.pop_back();
    objective.pop_back();

    if (debug) {
        cout << "--------------------" << endl;
        for (int i = 0; i < binary_var.size(); i++) {
            for (int j = 0; j < binary_var[i].size(); j++)
                cout << binary_var[i][j] << " ";
            cout << endl;
        }
        cout << "Objective: " << objective << endl;
    }
    
    //get start time constraint
    cout << "--------------------" << endl;
    for (int i = 0; i < binary_var.size(); i++) {
        string temp_var = binary_var[i][0];
        for (int j = 1; j < binary_var[i].size(); j++)
            temp_var = temp_var + " + " + binary_var[i][j];
        temp_var = temp_var + " = 1";
        start_time_constr.push_back(temp_var);
    }

    //Get resource constraint
    for (int i = 0; i < min_step; i++) {
        string and_line, or_line, not_line;

        for (int j = 0; j < ilp_extract.size(); j++) {
            if ((i+1 >= ilp_extract[j].t_soon) && (i+1 <= ilp_extract[j].t_late)) {
                switch (list_gate[ilp_extract[j].gate_idx].operation) {
                case 0:
                    and_line = and_line + "x_" + to_string(ilp_extract[j].gate_idx) + "_" + to_string(i+1) + " + ";
                    break;
                case 1:
                    or_line = or_line + "x_" + to_string(ilp_extract[j].gate_idx) + "_" + to_string(i+1) + " + ";
                    break;
                case 2:
                    not_line = not_line + "x_" + to_string(ilp_extract[j].gate_idx) + "_" + to_string(i+1) + " + ";
                    break;
                default:
                    break;
                }
            }
        }
        
        if (and_line.size() != 0) {
            and_line.pop_back();
            and_line.pop_back();
            and_line = and_line + "<= " + to_string(and_c);
            and_rcs.push_back(and_line);
        }

        if (or_line.size() != 0) {
            or_line.pop_back();
            or_line.pop_back();
            or_line = or_line + "<= " + to_string(or_c);
            or_rcs.push_back(or_line);
        }

        if (not_line.size() != 0) {
            not_line.pop_back();
            not_line.pop_back();
            not_line = not_line + "<= " + to_string(not_c);
            not_rcs.push_back(not_line);
        }
    }

    if (debug) {
        cout << "And constraint ineq" << endl;
        for (int i = 0; i < and_rcs.size(); i++)
            cout << and_rcs[i] << endl;
        
        cout << "OR constraint ineq" << endl;
        for (int i = 0; i < or_rcs.size(); i++)
            cout << or_rcs[i] << endl;

        cout << "Not constraint ineq" << endl;
        for (int i = 0; i < not_rcs.size(); i++)
            cout << not_rcs[i] << endl;
    }

    //Dependency constraint
    cout << "--------------------" << endl;
    vector<string> dependency_constr;
    for (int i = 0; i < ilp_extract.size(); i++) {
        // cout << list_gate[ilp_extract[i].gate_idx].out.wire_name << endl;
        if ((ilp_extract[i].t_late - ilp_extract[i].t_soon) != 0 ) {
            string temp_var;
            // Check the predecessor of the gate
            vector<int> pre_check = get_predecessor_gate(ilp_extract[i].gate_idx);
            // Check the sucessor of the gate
            vector<int> suc_check = get_successor_gate(ilp_extract[i].gate_idx);
            
            // If size of the pre_check != 0 => for each pre, conduct a inequation
            for (int j = 0; j < pre_check.size(); j++) {
                temp_var.clear();
                int k;
                for (k = 0; k < ilp_extract.size(); k++)
                    if (pre_check[j] == ilp_extract[k].gate_idx) break;
                for (int m = 0; m < binary_var[i].size(); m++)
                    temp_var = temp_var + to_string(m+ilp_extract[i].t_soon) + " " + binary_var[i][m] + " + ";
                temp_var.pop_back();
                temp_var.pop_back();
                temp_var.pop_back();
                for (int m = 0; m < binary_var[k].size(); m++)
                    temp_var = temp_var + " - " + to_string(m+ilp_extract[k].t_soon) + " " + binary_var[k][m];
                temp_var = temp_var + " >= " + to_string(list_gate[ilp_extract[i].gate_idx].delay);
                
                dependency_constr.push_back(temp_var);
            }

            // If size of the suc_check != 0 => for each suc, conduct a ineq
            for (int j = 0; j < suc_check.size(); j++) {
                temp_var.clear();
                int k;
                for (k = 0; k < ilp_extract.size(); k++)
                    if (suc_check[j] == ilp_extract[k].gate_idx) break;
                for (int m = 0; m < binary_var[k].size(); m++)
                    temp_var = temp_var + to_string(m+ilp_extract[k].t_soon) + " " + binary_var[k][m] + " + ";
                temp_var.pop_back();
                temp_var.pop_back();
                temp_var.pop_back();
                for (int m = 0; m < binary_var[i].size(); m++)
                    temp_var = temp_var + " - " + to_string(m+ilp_extract[i].t_soon) + " " + binary_var[i][m];
                temp_var = temp_var + " >= " + to_string(list_gate[ilp_extract[i].gate_idx].delay);
                dependency_constr.push_back(temp_var);
            }
        }
    }

    sort(dependency_constr.begin(), dependency_constr.end());
    dependency_constr.erase(unique(dependency_constr.begin(), dependency_constr.end()), dependency_constr.end());

    if (debug) {
    for (int i = 0; i < dependency_constr.size(); i++)
        cout << dependency_constr[i] << endl;
    }

    //Export ILP file
    string output_file_name = getModuleName() + ".lp";
    ofstream output_file(output_file_name);
    output_file << "Minimize" << endl;
    output_file << " obj: " << objective << endl;
    output_file << "Subject To" << endl;
    int no = 1;
    for (int i = 0; i < start_time_constr.size(); i++) {
        output_file << " c" << no << ": " << start_time_constr[i] << endl;
        no++;
    }
    for (int i = 0; i < dependency_constr.size(); i++) {
        output_file << " c" << no << ": " << dependency_constr[i] << endl;
        no++;
    }
    for (int i = 0; i < and_rcs.size(); i++) {
        output_file << " c" << no << ": " << and_rcs[i] << endl;
        no++;
    }
    for (int i = 0; i < or_rcs.size(); i++) {
        output_file << " c" << no << ": " << or_rcs[i] << endl;
        no++;
    }
    for (int i = 0; i < not_rcs.size(); i++) {
        output_file << " c" << no << ": " << not_rcs[i] << endl;
        no++;
    }
    output_file << "Binary" << endl;
    for (int i = 0; i < binary_var.size(); i++)
        for (int j = 0; j < binary_var[i].size(); j++)
            output_file << " " << binary_var[i][j] << endl;
    output_file << "End" << endl;
    output_file.close();

    string cmd = "gurobi_cl ResultFile=" + getModuleName() + ".sol " + getModuleName() + ".lp";
    cout << cmd << endl;
    const char * c = cmd.c_str();
    system(c);
    schedule = read_sol_file(min_step);
}

vector<vector<int>> Scheduling::read_sol_file(int step) {
    string sol_file_name = getModuleName() + ".sol";
    ifstream sol_file(sol_file_name);
    string temp;
    vector<vector<int>> schedule_temp(step);

    while (sol_file) {
        sol_file >> temp;
        if (temp[0] == '#') {					//check if we get the comment part
			getline(sol_file, temp);
			continue;
		}
        else {
            string _schedule;
            sol_file >> _schedule;
            if (_schedule == "1") {
                temp.erase(temp.begin());
                temp.erase(temp.begin());
                auto i = temp.begin();
                string part;
                while (*i != '_') {
                    part.push_back(*i);
                    i = temp.erase(i);
                }
                temp.erase(temp.begin());
                int step = stoi(temp);
                int idx = stoi(part);
                schedule_temp[step-1].push_back(idx);
            }
            else
                continue;
        }
    }
    return schedule_temp;
}