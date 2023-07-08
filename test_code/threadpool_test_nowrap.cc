#include "../library_header/threadpool.h"
#include <iostream>
#include "../library_header/call_class.h"
#include "../library_header/tp_callable.h"
// #include <string>

#define NUM_THREADS 8
#define NUM_TASKS (2 * NUM_THREADS)

using std::cout;

int main(void) {
    threadpool<call_class> tp(NUM_THREADS);
    for (int i = 0; i < NUM_TASKS; i++) {
        cout << ("queuing job: " + std::to_string(i) + "\n");
        tp.queue_job(call_class(i));
    }

    return 0;
}
