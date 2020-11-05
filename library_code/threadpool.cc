#include "threadpool.h"
#include "tp_callable.h"
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

template class threadpool<tp_callable_wrapper>;