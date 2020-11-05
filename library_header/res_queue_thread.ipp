#include "res_queue_thread_def.h"

template <class T>
res_queue_thread<T>::res_queue_thread(bool d_inner, locked_queue<T> &lq, return_callable<T> *in)
    : delete_inner(d_inner), q_access(lq.get_access()), inner(in) {}

template <class T>
void res_queue_thread<T>::call() {
    queue_result(inner->call());
}

template <class T>
void res_queue_thread<T>::queue_result(T result) {
    q_access.lock.lock();
    q_access.q.push(result);
    q_access.cond.notify_one();
    q_access.lock.unlock();
}