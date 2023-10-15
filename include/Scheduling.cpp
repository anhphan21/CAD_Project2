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

    if (debug) {
    cout << "rdy list before scheduling:";
    for (int i = 0; i < rdy.size(); i++)
        cout << " " << list_gate[rdy[i]].out.wire_name;
    cout << endl;
    
    cout << "not_schdedule list before scheduling:";
    for (int i = 0; i < not_scheduled_list.size(); i++)
        cout << " " << list_gate[not_scheduled_list[i]].out.wire_name;
    cout << endl;
    }

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
                if (list_gate[j].step < min)
                    min = list_gate[suc_check[j]].step;
            }

            list_gate[*i].step = min - list_gate[*i].delay;

            sche_this_step.push_back(*i);
            cout << "Schedule gate " << get_wire_name(*i) << " at " << step << endl;
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
                if (list_gate[j].step > max)
                    max = list_gate[pre_check[j]].step;
            }

            list_gate[*i].step = max + list_gate[*i].delay;

            sche_this_step.push_back(*i);
            // cout << "Schedule gate " << get_wire_name(*i) << " at " << step << endl;
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