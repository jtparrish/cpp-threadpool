#include "threadpool.h"
#include "call_class.h"
// #include "server.h"

template <class T>
void thread_fn(lq_accessor<T> job_q, bool &running) {
    while (running) {
        job_q.lock.lock();
        while (job_q.q.empty()) {
            if (!running) {
                job_q.lock.unlock();
                return;
            }
            job_q.cond.wait(job_q.lock);
        }

        queue<T> &jobs = job_q.q;

        T job = jobs.front(); jobs.pop();
        job_q.lock.unlock();
        job.call();
    }
}

template <class T>
lq_accessor<T> locked_queue<T>::get_access(void) {
    return lq_accessor<T>(raw_lock, cond, q);
}

template<class T>
lq_accessor<T>::lq_accessor(mutex &raw_lock, condition_variable &cond_arg, queue<T> &q_arg) : lock(unique_lock<mutex>(raw_lock, defer_lock_t())), cond(cond_arg), q(q_arg) {}

switched_thread::switched_thread(thread t_arg) : dead(false), t(std::move(t_arg)), switch_bool(true) {}

// Move Constructor
switched_thread::switched_thread(switched_thread && obj) : dead(obj.dead), t(std::move(obj.t)), switch_bool(obj.switch_bool) {
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

template <class T>
threadpool<T>::threadpool(unsigned int n_threads) : jq_access(job_q.get_access()) {
    for (unsigned int i = 0; i < n_threads; i++) {
        switched_thread st(thread(thread_fn<T>, job_q.get_access(), std::ref(st.switch_bool)));
        pool.push_back(std::move(st));
    }
}

template <class T>
void threadpool<T>::queue_job(T job) {
    jq_access.lock.lock();
    jq_access.q.push(job);
    jq_access.lock.unlock();
    jq_access.cond.notify_one();
}

template <class T>
void threadpool<T>::finalize() {
    jq_access.lock.lock(); //need lock to avoid race (thread stuck b/w running check and the cond_wait)
    for (unsigned int i = 0; i < pool.size(); i++) {
        pool[i].switch_bool = false;
    }
    jq_access.lock.unlock();

    jq_access.cond.notify_all();
}
template <class T>
threadpool<T>::~threadpool() { //FIXME unnecessary
    finalize();
}

tp_callable::~tp_callable() {}

tp_callable_wrapper::tp_callable_wrapper(tp_callable *c) : inner(c), cleanup(false) {}

tp_callable_wrapper::tp_callable_wrapper(tp_callable *c, bool cu) : inner(c), cleanup(cu) {}

void tp_callable_wrapper::call() {
    inner->call();
    finalize();
}

void tp_callable_wrapper::finalize() {
    if (cleanup) {
        delete_internal();
    }
}

void tp_callable_wrapper::delete_internal() {
    delete inner;
}

template class threadpool<tp_callable_wrapper>;
template class threadpool<call_class>;