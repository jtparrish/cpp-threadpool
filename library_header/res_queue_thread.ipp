#include "res_queue_thread_def.h"

template <class T>
return_callable<T>::~return_callable() {}

template <class T>
res_queue_thread<T>::res_queue_thread(return_callable<T> *in, locked_queue<T> &lq)
    : inner(in), q_access(lq.get_access()), delete_inner(false) {}

template <class T>
res_queue_thread<T>::res_queue_thread(return_callable<T> *in, locked_queue<T> &lq, bool del_inner)
    : inner(in), q_access(lq.get_access()), delete_inner(del_inner) {}

template <class T>
void res_queue_thread<T>::call() {
    queue_result(inner->call());
    if (delete_inner) {
        delete inner;
    }
}

template <class T>
void res_queue_thread<T>::queue_result(T result) {
    q_access.lock.lock();
    q_access.q.push(result);
    q_access.cond.notify_one();
    q_access.lock.unlock();
}

template <class T>
result_queue<T>::result_queue(unsigned int num_threads)
    : tp(num_threads), rq_access(rq.get_access()), jobs_queued(0), delete_inner(false) {}

template <class T>
result_queue<T>::result_queue(unsigned int num_threads, bool del_inner)
    : tp(num_threads), rq_access(rq.get_access()), jobs_queued(0), delete_inner(del_inner) {}

template <class T>
void result_queue<T>::queue_job(return_callable<T> *job) {
    queue_job(job, delete_inner);
}

template <class T>
void result_queue<T>::queue_job(return_callable<T> *job, bool del_inner) {
    tp.queue_job(
        tp_callable_wrapper(
            new res_queue_thread<T>(
                job,
                rq,
                del_inner
            ),
            true
        )
    );
    jobs_queued++;
}

template <class T>
result_queue<T>::rq_infinite_wait_exception::rq_infinite_wait_exception(void) {}

template <class T>
const char *result_queue<T>::rq_infinite_wait_exception::what() const noexcept(true) {
    return "tried to get a result from a result_queue which has no more results and no more"
    "jobs to complete running; this will lead to an infinite wait";
}


// TODO: CONSIDER ADDITIONAL CLASS FOR MULTIPLE ADD THREADS (i.e. producer consumer)
template <class T>
T result_queue<T>::get_result(void) {
    if (jobs_queued == 0) {
        throw rq_infinite_wait_exception();
    }

    rq_access.lock.lock();
    while (rq_access.q.empty()) {
      rq_access.cond.wait(rq_access.lock);
    }

    T res = rq_access.q.front();
    rq_access.q.pop();
    rq_access.lock.unlock();

    jobs_queued--;
    return res;
}