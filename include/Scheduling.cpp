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

void Scheduling::cal_alap() {
    // for (int i = 0; i < sorted_gate_list.size(); i++) {
    //     vector<int> req;
    //     vector<int> check_list = get_gate_inputs_from_in(sorted_gate_list[i]);
        
    //     for (int j = 0; j < check_list.size(); j++)
    //         req.push_back(list_input[check_list[j]].req_time);

    //     check_list = get_gate_inputs_from_gate(sorted_gate_list[i]);
    //     for (int j = 0; j < check_list.size(); j++)
    //         req.push_back(list_gate[check_list[j]].out.req_time);

    //     int max = req[0];
    //     for (int j = 1; j < req.size(); j++)
    //         if (req[j] > max)
    //             max = req[j];
        
    //     list_gate[sorted_gate_list[i]].out.req_time = max + list_gate[sorted_gate_list[i]].delay;
    // }
    
    // cout << "---------------" << endl;
    // for (int i = 0; i < no_gate; i++) {
    //     cout << sorted_gate_list[i] << ". " << list_gate[sorted_gate_list[i]].out.req_time << endl;
    // }
    // cout << "---------------" << endl;
}

void Scheduling::cal_asap() {
//     for (int i = 0; i < sorted_gate_list.size(); i++) {
//         vector<int> arr;
//         vector<int> check_list = get_gate_inputs_from_in(sorted_gate_list[i]);
        
//         for (int j = 0; j < check_list.size(); j++)
//             arr.push_back(list_input[check_list[j]].arr_time);

//         check_list = get_gate_inputs_from_gate(sorted_gate_list[i]);
//         for (int j = 0; j < check_list.size(); j++)
//             arr.push_back(list_gate[check_list[j]].out.arr_time);

//         int max = arr[0];
//         for (int j = 1; j < arr.size(); j++)
//             if (arr[j] > max)
//                 max = arr[j];
        
//         list_gate[sorted_gate_list[i]].out.arr_time = max + list_gate[sorted_gate_list[i]].delay;
//     }
    
//     cout << "---------------" << endl;
//     for (int i = 0; i < no_gate; i++) {
//         cout << sorted_gate_list[i] << ". " << list_gate[sorted_gate_list[i]].out.arr_time << endl;
//     }
//     cout << "---------------" << endl;
// }

// void Scheduling::cal_slack() {
//     for (int i = 0; i < no_gate; i++)
//         list_gate[i].out.slack = list_gate[i].out.req_time - list_gate[i].out.arr_time;

//     cout << "---------------" << endl;
//     for (int i = 0; i < no_gate; i++) {
//         cout << sorted_gate_list[i] << ". " << list_gate[sorted_gate_list[i]].out.slack << endl;
//     }
//     cout << "---------------" << endl;
}

void Scheduling::check_resource_available(vector<int> &vec, int step) {
    for (auto i = vec.begin(); i != vec.end();) {
        if ((step - list_gate[*i].step) == list_gate[*i].delay)
            i = vec.erase(i);
        else
            i++;
    }
}

// bool Scheduling::check_predecessor_scheduled(int gate_idx, vector<int> not_scheduled) {
//     for (int i = 0; i < list_gate[gate_idx].out.nxt_gate.size(); i++) {
//         for (int j = 0; j < not_scheduled.size(); j++)
//             if (list_gate[gate_idx].out.nxt_gate[i] == not_scheduled[j])
//                 return 0;        
//     }
//     return 1;
// }

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
            if (list_gate[schedule[i][j]].operation == 0)
                and_rsc.push_back(schedule[i][j]);
            else if (list_gate[schedule[i][j]].operation == 1)
                or_rsc.push_back(schedule[i][j]);
            else if (list_gate[schedule[i][j]].operation == 2)
                not_rsc.push_back(schedule[i][j]);
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
            cout << " " << get_wire_name(or_rsc[j]);
        cout << " }" << endl;
    }
    cout << "Latency: " << schedule.size()+1 << endl;
    cout << "End" << endl;    
}