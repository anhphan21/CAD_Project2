#include <iostream>
#include <string>
#include <list>
#include "./include/Graph.h"
using namespace std;

bool test_eq(vector<int> in1, vector<int> in2) {
    for (int i = 0; i < in1.size(); i++) {
        for (int j = 0; j < in2.size(); j++) {
            if (in2[j] == in1[i])
                break;
            else
                if (j == (in2.size()-1))
                    return 0;
        }
    }
    return 1;
}

vector<int> unique(vector<int> pre) {
    vector<int> uni;
   
    for (int i = 0; i < pre.size(); i++) {
        int j = 0;
        for (j = 0; j < uni.size(); j++) {
            if (pre[i] == uni[j])
                break;
        }
        if (j == uni.size())
            uni.push_back(pre[i]);
    }
    return uni;
}

int main() {
    Graph test;
    test.setModuleName("test");
    vector <string> in1 {"a", "b", "c"};
    vector <int> a {1,2,3,4,5,6,6,7,8,9,5,1,2};
    vector <int> b {2,4,1,3,5, 5, 6};
    // cout << test_eq(a,b) << endl;
    // b = unique(b);
    // for (int i = 0; i < b.size(); i++) {
    //     cout << b[i] << " ";
    // }
    // cout << endl << "-------" << endl;
    

    sort(a.begin(), a.end() );
    a.erase( unique( a.begin(), a.end() ), a.end() );
    for (int i = 0; i < a.size(); i++) {
        cout << a[i] << " ";
    }
    return 0;
}