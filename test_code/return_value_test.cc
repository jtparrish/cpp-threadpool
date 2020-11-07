#include "threadpool.h"
#include "res_queue_thread.h"
#include "locked_queue.h"

#include<iostream>

using namespace std;

#define NUM_THREADS 8
#define NUM_JOBS (2 * NUM_THREADS)


class rqt_int_impl : public return_callable<int> {
    public:
        rqt_int_impl(int arg_a, int arg_b);
        int call(void);

    private:
        int a;
        int b;
};

rqt_int_impl::rqt_int_impl(int arg_a, int arg_b)
    :a(arg_a), b(arg_b) {}

int rqt_int_impl::call(void) {
    return a + b;
}

int main(void) {
    threadpool<tp_callable_wrapper> tp(NUM_THREADS);
    locked_queue<int> q;
    lq_accessor<int> q_access = q.get_access();

    for (int i = 0; i < NUM_JOBS; i++) {
        tp.queue_job(
            tp_callable_wrapper(
                new res_queue_thread<int>(
                    new rqt_int_impl(i, i),
                    q,
                    true
                ),
                true
            )
        );
    }
    
    int res_received = 0;
    while (res_received < NUM_JOBS) {
        q_access.lock.lock();
        while (q_access.q.empty()) {
          q_access.cond.wait(q_access.lock);
        }
        int res = q_access.q.front();
        q_access.q.pop();
        q_access.lock.unlock();

        cout << "res received: " << res << std::endl;
        res_received++;
    }

    return 0;
}