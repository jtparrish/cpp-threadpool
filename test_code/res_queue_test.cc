#include "res_queue_thread.h"

#include<iostream>
using namespace std;

#define NUM_THREADS 8
#define NUM_JOBS (2 * NUM_THREADS)

class rc_int_impl : public return_callable<int> {
    public:
        rc_int_impl(int arg_a, int arg_b);
        int call(void);

    private:
        int a;
        int b;
};

rc_int_impl::rc_int_impl(int arg_a, int arg_b) : a(arg_a), b(arg_b) {}

int rc_int_impl::call(void) {
    return a + b;
}

int main(void) {
    result_queue<int> rq(NUM_THREADS, true);
    for (int i = 0; i < NUM_JOBS; i++) {
        rq.queue_job(new rc_int_impl(i, 2));
    }

    int res_received = 0;
    while (res_received < NUM_JOBS) {
        cout << "result: " << rq.get_result() << std::endl;
        res_received++;
    }

    try {
        rq.get_result();
    } catch (const result_queue<int>::rq_infinite_wait_exception &iwe) {
        cout << "exception caught" << std::endl;
    }

    return 0;
}