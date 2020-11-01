#include "threadpool.h"

void thread_fn(lq_accessor job_q, bool &running) {
    while (running) {
        job_q.lock.lock();
        while (job_q.q.empty()) {
            if (!running) {
                job_q.lock.unlock();
                return;
            }
            job_q.cond.wait(job_q.lock);
        }

        if (running) {
            queue<void (*) (void)> &jobs = job_q.q;

            void (*job) (void) = jobs.front(); jobs.pop();
            job_q.lock.unlock();
            (*job)();
        }
    }
}

lq_accessor locked_queue::get_access(void) {
    return lq_accessor(raw_lock, cond, q);
}

lq_accessor::lq_accessor(mutex &raw_lock, condition_variable &cond_arg, queue<void (*)(void)> &q_arg) : lock(unique_lock<mutex>(raw_lock, defer_lock_t())), cond(cond_arg), q(q_arg) {}

switched_thread::switched_thread() : t(thread()), switch_bool(true) {}

// Move Constructor
switched_thread::switched_thread(switched_thread && obj) : t(std::move(obj.t)), switch_bool(obj.switch_bool), dead(obj.dead) {
    obj.dead = true;
}

//Move Assignment Operator
switched_thread &switched_thread::operator=(switched_thread && obj) {
    t = std::move(obj.t);
    switch_bool = obj.switch_bool;
    dead = obj.dead;
    obj.dead = true;
    return *this;
}

//Destructor
switched_thread::~switched_thread() {
    if (!dead) {
        switch_bool = false;
        t.join();
    }
}

threadpool::threadpool(unsigned int n_threads) : jq_access(job_q.get_access()) {
    for (unsigned int i = 0; i < n_threads; i++) {
        switched_thread st;
        st.t = thread(thread_fn, job_q.get_access(), std::ref(st.switch_bool));
        pool.push_back(std::move(st));
    }
}

void threadpool::queue_job(void (*job)(void)) {
    jq_access.lock.lock();
    jq_access.q.push(job);
    jq_access.lock.unlock();
    jq_access.cond.notify_one();
}

void threadpool::finalize() {
    for (unsigned int i = 0; i < pool.size(); i++) {
        pool[i].switch_bool = false;
    }

    jq_access.cond.notify_all();
}

threadpool::~threadpool() { //FIXME unnecessary
    finalize();
}