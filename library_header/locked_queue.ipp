#pragma once
#include "locked_queue_def.h"

template <class T>
typename locked_queue<T>::lq_accessor locked_queue<T>::get_access(void) {
    return lq_accessor(raw_lock, cond, q);
}

template<class T>
locked_queue<T>::lq_accessor::lq_accessor(mutex &raw_lock, condition_variable &cond_arg, queue<T> &q_arg)
    : lock(unique_lock<mutex>(raw_lock, defer_lock_t())), cond(cond_arg), q(q_arg) {}