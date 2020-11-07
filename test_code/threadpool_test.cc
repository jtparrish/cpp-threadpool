#include "threadpool.h"
#include <iostream>
#include "call_class.h"
#include "tp_callable.h"
// #include <string>

#define NUM_THREADS 8
#define NUM_TASKS (2 * NUM_THREADS)

using std::cout;

int main(void) {
    threadpool<tp_callable_wrapper> tp(NUM_THREADS);
    for (int i = 0; i < NUM_TASKS; i++) {
        cout << ("queuing job: " + std::to_string(i) + "\n");
        tp.queue_job(tp_callable_wrapper(new call_class(i), true));
    }

    return 0;
}