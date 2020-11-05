#include "call_class.h"
#include <iostream>

using namespace std;

call_class::call_class(int a) {
    i = a;
}

void call_class::call(void) {
    string s = "running job: " + std::to_string(i) +"\n";
    cout << s;
}