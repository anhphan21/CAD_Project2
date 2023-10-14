#include "./Scheduling.h"

Scheduling::Scheduling() {

}

Scheduling::~Scheduling() {

}

void Scheduling::reset_schedule() {
    for (int i = 0; i < no_gate; i++) {
        list_gate[i].step = -1;
    }
    schedule.clear();
}

void Scheduling::cal_alap() {
    for (int i = 0; i < sorted_gate_list.size(); i++) {
        vector<int> req;
        vector<int> check_list = get_gate_inputs_from_in(sorted_gate_list[i]);
        
        for (int j = 0; j < check_list.size(); j++)
            req.push_back(list_input[check_list[j]].req_time);

        check_list = get_gate_inputs_from_gate(sorted_gate_list[i]);
        for (int j = 0; j < check_list.size(); j++)
            req.push_back(list_gate[check_list[j]].out.req_time);

        int max = req[0];
        for (int j = 1; j < req.size(); j++)
            if (req[j] > max)
                max = req[j];
        
        list_gate[sorted_gate_list[i]].out.req_time = max + list_gate[sorted_gate_list[i]].delay;
    }
    
    cout << "---------------" << endl;
    for (int i = 0; i < no_gate; i++) {
        cout << sorted_gate_list[i] << ". " << list_gate[sorted_gate_list[i]].out.req_time << endl;
    }
    cout << "---------------" << endl;
}

void Scheduling::cal_asap() {
    for (int i = 0; i < sorted_gate_list.size(); i++) {
        vector<int> arr;
        vector<int> check_list = get_gate_inputs_from_in(sorted_gate_list[i]);
        
        for (int j = 0; j < check_list.size(); j++)
            arr.push_back(list_input[check_list[j]].arr_time);

        check_list = get_gate_inputs_from_gate(sorted_gate_list[i]);
        for (int j = 0; j < check_list.size(); j++)
            arr.push_back(list_gate[check_list[j]].out.arr_time);

        int max = arr[0];
        for (int j = 1; j < arr.size(); j++)
            if (arr[j] > max)
                max = arr[j];
        
        list_gate[sorted_gate_list[i]].out.arr_time = max + list_gate[sorted_gate_list[i]].delay;
    }
    
    cout << "---------------" << endl;
    for (int i = 0; i < no_gate; i++) {
        cout << sorted_gate_list[i] << ". " << list_gate[sorted_gate_list[i]].out.arr_time << endl;
    }
    cout << "---------------" << endl;
}

void Scheduling::cal_slack() {
    for (int i = 0; i < no_gate; i++)
        list_gate[i].out.slack = list_gate[i].out.req_time - list_gate[i].out.arr_time;

    cout << "---------------" << endl;
    for (int i = 0; i < no_gate; i++) {
        cout << sorted_gate_list[i] << ". " << list_gate[sorted_gate_list[i]].out.slack << endl;
    }
    cout << "---------------" << endl;
}

void Scheduling::check_resource_available(vector<int> &vec, int step) {
    for (auto i = vec.begin(); i != vec.end();) {
        if ((step - list_gate[*i].step) == list_gate[*i].delay)
            i = vec.erase(i);
        else
            i++;
    }
}

void Scheduling::list_scheduling(int and_c, int or_c, int not_c) {
    reset_schedule();

    set_AND_CONST(and_c);
    set_OR_CONST(or_c);
    set_NOT_CONST(not_c);

    int step = 0;
    vector<int> sche_this_step;
    vector<int> rdy, nxt_rdy;
    int no_scheduled_gate = 0;
    vector<int> and_rsc(0);
    vector<int> not_rsc(0);
    vector<int> or_rsc(0);

    // init nxt.rdy
    for (int i = 0; i < list_input.size(); i++) {
        for (int j = 0; j < list_input[i].nxt_gate.size(); j++) {
            int k;
            int check_size = nxt_rdy.size();
            for (k = 0; k < check_size; k++)
                if (nxt_rdy[k] == list_input[i].nxt_gate[j])
                    break;
            if (k == check_size)
                nxt_rdy.push_back(list_input[i].nxt_gate[j]);
        }
    }
    cout << "-----------------------------" << endl;
    cout << "Initial nxt_rdy: ";
    for (int i = 0; i < nxt_rdy.size(); i++)
            cout << nxt_rdy[i] << " ";
    cout << endl;

    while (no_scheduled_gate != no_gate) {
        sche_this_step.clear();
        // Check resource are free from pre step
        cout << "Step no: " << step << endl;
        check_resource_available(and_rsc, step);
        check_resource_available(or_rsc, step);
        check_resource_available(not_rsc, step);
        
        // check which gate in nxt_ready is ready and create rdy
        for (int i = 0; i < nxt_rdy.size(); i++) {
            vector<int> pre_check = get_predecessor_gate(nxt_rdy[i]);
            int j = 0;
            for (j = 0; j < pre_check.size(); j++)
                if (list_gate[pre_check[i]].step == -1) 
                    break;
            if (j == pre_check.size())
                rdy.push_back(nxt_rdy[i]);
        }
        
        for (int i = 0; i < rdy.size(); i++)
            for (int j = 0; j < nxt_rdy.size(); j++)
                if (rdy[i] == nxt_rdy[j]) {
                    nxt_rdy.erase(nxt_rdy.begin()+j);
                    break;
                }
        
        if (debug) {
            cout << "Rdy update from nxt_rdy: ";
            for (int i = 0; i < rdy.size(); i++)
                cout << rdy[i] << " ";
            cout << endl;

            cout << "Nxt_rdy after transfer to rdy: ";
            for (int i = 0; i < nxt_rdy.size(); i++)
                    cout << nxt_rdy[i] << " ";
            cout << endl;
        }
        
        
                
        // Schedule gate
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
                cout << "Scheduled: " << rdy[i] << " at " << step << endl;
                sche_this_step.push_back(rdy[i]);
            }
        }

        if (debug) {
            cout << "Status of rdy after scheduling: ";
            for (int i = 0; i < rdy.size(); i++)
                cout << list_gate[rdy[i]].step << " ";
            cout << endl;
        }
        
        
        auto it = rdy.begin();
        while (it != rdy.end()) {
            if (list_gate[*it].step != -1)
                it = rdy.erase(it);
            else
                it++;
        }

        if (debug) {
            cout << "rdy after scheduling: ";
            for (int i = 0; i < rdy.size(); i++)
                cout << rdy[i] << " ";
            cout << endl;
        }

        //Update next rdy with ready gate
        for (int i = 0; i < no_gate; i++) {
            if (list_gate[sorted_gate_list[i]].step == -1) {
                vector<int> pre_gate = get_predecessor_gate(sorted_gate_list[i]);
                int check_no = pre_gate.size();
                if (check_no != 0) {
                    int j;
                    for (j = 0; j < check_no; j++) {
                        if (list_gate[pre_gate[j]].step == -1)
                            break;
                    }
                    if (j == check_no)
                        nxt_rdy.push_back(sorted_gate_list[i]);
                }
            }
        }

        if (debug) {
            cout << "Check nxt_rdy after update new gate: ";
            for (int i = 0; i < nxt_rdy.size(); i++)
                cout << nxt_rdy[i] << " ";
            cout << endl;
            cout << "---------------------" << endl;
        }
        schedule.push_back(sche_this_step);
        step++;
    }
    cout << "Done list scheduling!!!" << endl;
}

void Scheduling::print_Schedule() {
    vector<int> and_rsc;
    vector<int> not_rsc;
    vector<int> or_rsc;
    cout <<  "------ Schedule ------" << endl;
    for (int i = 0; i < schedule.size(); i++) {
        cout << "Step " << i+1 << ":";
        for (int j = 0; j < schedule[i].size(); j++)
            cout << " " << get_gate_name(schedule[i][j]);
        cout << endl;
    }
    cout << "-----------------------" << endl;
    
}