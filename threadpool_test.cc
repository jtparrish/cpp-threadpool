#include "threadpool.h"
#include <iostream>
#include "call_class.h"
// #include <string>

#define NUM_THREADS 8

using std::cout;

int main(void) {
    threadpool<call_class> tp(NUM_THREADS);
    for (int i = 0; i < 16; i++) {
        cout << ("queuing job: " + std::to_string(i) + "\n");
        tp.queue_job(call_class(i));
    }

    return 0;
}