#include "./Scheduling.h"

void Scheduling:: update_wire_slack(int gate_idx) {
    update_gate_arr_time(gate_idx);
    update_gate_req_time(gate_idx);
    list_gate[gate_idx].out.slack = list_gate[gate_idx].out.req_time - list_gate[gate_idx].out.arr_time;
}

void Scheduling::update_gate_req_time(int gate_idx) {
    int req1, req2;

    vector<int> check_list = get_gate_inputs_from_in(gate_idx);
    for (int i = 0; i < check_list.size(); i++) {
        if (i == 0)
            req1 = list_input[check_list[i]].req_time;
        else
            if (list_input[check_list[i]].req_time < req1)
                req1 = list_input[check_list[i]].req_time;
    }

    check_list = get_gate_inputs_from_gate(gate_idx);
    for (int i = 0; i < check_list.size(); i++) {
        if (i == 0)
            req2 = list_input[check_list[i]].req_time;
        else
            if (list_input[check_list[i]].req_time < req2)
                req2 = list_input[check_list[i]].req_time;
    }
    if (req1 < req2)
        list_gate[gate_idx].out.req_time = req1 + list_gate[gate_idx].delay;
    else
        list_gate[gate_idx].out.req_time = req2 + list_gate[gate_idx].delay;
}

void Scheduling::update_gate_arr_time(int gate_idx) {
    int arr1, arr2;

    vector<int> check_list = get_gate_inputs_from_in(gate_idx);
    for (int i = 0; i < check_list.size(); i++) {
        if (i == 0)
            arr1 = list_input[check_list[i]].arr_time;
        else
            if (list_input[check_list[i]].arr_time > arr1)
                arr1 = list_input[check_list[i]].arr_time;
    }

    check_list = get_gate_inputs_from_gate(gate_idx);
    for (int i = 0; i < check_list.size(); i++) {
        if (i == 0)
            arr2 = list_input[check_list[i]].arr_time;
        else
            if (list_input[check_list[i]].arr_time > arr2)
                arr2 = list_input[check_list[i]].arr_time;
    }
    
    if (arr1 > arr2)
        list_gate[gate_idx].out.arr_time = arr1 + list_gate[gate_idx].delay;
    else
        list_gate[gate_idx].out.arr_time = arr2 + list_gate[gate_idx].delay;
}

bool Scheduling::check_predecessor_scheduled(vector<int> list_scheduled, int gate_idx) {
    vector<int> check_list = get_gate_inputs_from_gate(gate_idx);
    for (int i = 0; i < check_list.size(); i++) {
        for (int j = 0; j < list_scheduled.size(); j++) {
            if (list_scheduled[j] == check_list[i])
                break;
            else {
                if (j == (list_scheduled.size()-1))
                    return 0;
            }
        }
    }
    return 1;
}

void Scheduling::asap() {
    schedule.clear();

    int step = 1;
    string temp_schedule;
    vector<int> already_scheduled;
    vector<int> nxt_rdy, rdy;
    
    int check_schedule = 0;

    for (int i = 0; i < no_gate; i++) {
        // get independent gate and then schedule to the 0 step 
        if (get_predecessor_gate(list_gate[i].out.wire_name).size() == 0) {
            already_scheduled.push_back(i);
            temp_schedule = temp_schedule + to_string(i) + " ";
            update_gate_arr_time(i);
            check_schedule++;
        }
    }
    //Schedule 1st step
    schedule.push_back(temp_schedule);
    
    //Get nxt_ready
    for (int i = 0; i < already_scheduled.size(); i++) {
        for (int j = 0; j < list_gate[already_scheduled[i]].out.nxt_gate.size(); j++) {
            nxt_rdy.push_back(list_gate[already_scheduled[i]].out.nxt_gate[j]);
        }
    }
    sort(nxt_rdy.begin(), nxt_rdy.end());
    auto it = unique(nxt_rdy.begin(), nxt_rdy.end());
    nxt_rdy.erase(it, nxt_rdy.end());
    
    //Get ready
    for (int i = 0; i < nxt_rdy.size(); i++) {
        if(check_predecessor_scheduled(already_scheduled, nxt_rdy[i])) {
            rdy.push_back(nxt_rdy[i]);
        }
    }
    for (int i = nxt_rdy.size()-1; i > 0; i--) {
        for (int j = 0; j < rdy.size(); j++) {
            if (nxt_rdy[i] == rdy[j])
                nxt_rdy.pop_back();
        }
    }
    cout << "a" << endl;

    do {
        temp_schedule.clear();
        while (rdy.size() != 0) {
            for (int i = 0; i < rdy.size(); i++) {
                if (list_gate[rdy[i]].out.arr_time >= step) {
                    temp_schedule = temp_schedule + to_string(rdy[i]) + " ";
                    check_schedule++;
                }
            }
            cout << step << endl;

            for (int i = 0; i < already_scheduled.size(); i++) {
                for (int j = 0; j < list_gate[already_scheduled[i]].out.nxt_gate.size(); j++) {
                    nxt_rdy.push_back(list_gate[already_scheduled[i]].out.nxt_gate[j]);
                }
            }
            sort(nxt_rdy.begin(), nxt_rdy.end());
            auto it = unique(nxt_rdy.begin(), nxt_rdy.end());
            nxt_rdy.erase(it, nxt_rdy.end());
            
            //Get ready
            for (int i = 0; i < nxt_rdy.size(); i++) {
                if(check_predecessor_scheduled(already_scheduled, nxt_rdy[i])) {
                    rdy.push_back(nxt_rdy[i]);
                }
            }
            for (int i = nxt_rdy.size()-1; i > 0; i--) {
                for (int j = 0; j < rdy.size(); j++) {
                    if (nxt_rdy[i] == rdy[j])
                        nxt_rdy.pop_back();
                }
            }
        }
        schedule.push_back(temp_schedule);
        step++;
    } while (check_schedule < no_gate);
}

void Scheduling::list_scheduling() {
    schedule.clear();
    cout << "--------------------" << endl;
    cout << "AND\tOR\tINV" << endl;
    cout << AND_CONST << "\t" << OR_CONST << "\t" << INV_CONST << endl;

    int step = 0;

    int and_rsc, inv_rsc, or_rsc = 0;

    vector<int> rdy;
    vector<int> scheduled;  //store scheduled gate;
    string temp_schedule;

    //Get ready for 0 step
    for (int i = 0; i < list_input.size(); i++) {
        for (int j = 0; j < list_input[i].nxt_gate.size(); j++) {
            rdy.push_back(list_input[i].nxt_gate[j]);
        }
    }
    sort(rdy.begin(), rdy.end());
    auto it = unique(rdy.begin(), rdy.end());
    rdy.erase(it, rdy.end());

    cout << "------" << endl;
    for (int i = 0; i < rdy.size(); i++) {
        cout << rdy[i] << " ";
    }
    cout << endl;
    cout << "------" << endl;
    

    vector<int> this_step_done;
    int done_schedule = 0;

    while (done_schedule != no_gate) {
        and_rsc =0;
        inv_rsc = 0;
        or_rsc = 0;
        temp_schedule.clear();
        this_step_done.clear();

        for (int i = rdy.size()-1; i > 0; i--) {
            if (list_gate[rdy[i]].operation == -1) {
                if (inv_rsc < INV_CONST) {
                    inv_rsc++;
                    done_schedule++;
                    scheduled.push_back(rdy[i]);
                    this_step_done.push_back(rdy[i]);
                    temp_schedule = temp_schedule + to_string(rdy[i]) + " ";
                    cout << rdy[i] << "a" << endl;
                }
            } else if (list_gate[rdy[i]].operation == 0) {
                if (and_rsc < AND_CONST) {
                    and_rsc++;
                    done_schedule++;
                    scheduled.push_back(rdy[i]);
                    this_step_done.push_back(rdy[i]);
                    temp_schedule = temp_schedule + to_string(rdy[i]) + " ";
                    cout << rdy[i] << "b" << endl;
                }
            } else if (list_gate[rdy[i]].operation == 1) {
                if (or_rsc < OR_CONST) {
                    or_rsc++;
                    done_schedule++;
                    scheduled.push_back(rdy[i]);
                    this_step_done.push_back(rdy[i]);
                    temp_schedule = temp_schedule + to_string(rdy[i]) + " ";
                    cout << rdy[i] << "c" << endl;
                }
            }
        }

        for (int i = 0; i < this_step_done.size(); i++) {
            for (int j = 0; j < rdy.size(); j++) {
                if (this_step_done[i] == rdy[j]) {
                    rdy.erase(rdy.begin()+j);
                    break;
                }  
            }
        }

        for (int i = 0; i < rdy.size(); i++) {
            cout << rdy[i] << " ";
        }
        cout << endl;
        
        
        
        for (int i = 0; i < rdy.size(); i++) {
            for (int j = 0; j < list_gate[rdy[i]].out.nxt_gate.size(); j++) {
                            cout << "z z" << endl;

                if(check_predecessor_scheduled(scheduled, list_gate[rdy[i]].out.nxt_gate[j])) {
                    cout << "pre: " << list_gate[rdy[i]].out.nxt_gate[j];
                    rdy.push_back(list_gate[rdy[i]].out.nxt_gate[j]);
                }
            }
        }

        cout << "-----" << endl;
        cout << "Db" << endl;
        cout << rdy.size() << endl;
        cout << done_schedule << endl;
        cout << no_gate << endl;
        cout << "-----" << endl;

        sort(rdy.begin(), rdy.end());
        auto it = unique(rdy.begin(), rdy.end());
        rdy.erase(it, rdy.end());
        schedule.push_back(temp_schedule);
        
        step++;
        if (step == 5) {
            break;
        }
    }
}

void Scheduling::print_Schedule() {
    cout <<  "------ Schedule ------" << endl;
    for (int i = 0; i < schedule.size(); i++) {
        cout << i+1 << ". " << schedule[i] << endl;
    }
    cout << "-----------------------" << endl;
}