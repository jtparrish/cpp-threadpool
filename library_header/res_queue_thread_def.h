#pragma once
#include "locked_queue.h"
#include "tp_callable.h"

template <class T>
class return_callable {
    public:
        virtual T call(void);
        virtual ~return_callable();
};

template <class T>
class res_queue_thread : tp_callable {
    public:
        res_queue_thread(bool d_inner, locked_queue<T> &lq, return_callable<T> *in);
        void call(void);
    private:
        void queue_result(T result);
        const bool delete_inner;
        return_callable<T> *inner;
        lq_accessor<T> q_access;
};