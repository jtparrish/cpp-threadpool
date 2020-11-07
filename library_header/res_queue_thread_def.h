#pragma once
#include "locked_queue.h"
#include "tp_callable.h"
#include "threadpool.h"
#include <exception>

template <class T>
class return_callable {
    public:
        virtual T call(void) = 0;
        virtual ~return_callable();
};

template <class T>
class res_queue_thread : public tp_callable {
    public:
        res_queue_thread(return_callable<T> *in, locked_queue<T> &lq);
        res_queue_thread(return_callable<T> *in, locked_queue<T> &lq, bool del_inner);
        void call(void);
    private:
        void queue_result(T result);
        return_callable<T> *inner;
        lq_accessor<T> q_access;
        const bool delete_inner;
};

template <class T>
class result_queue {
    public:
        struct rq_infinite_wait_exception : public std::exception {
	        const char * what () const noexcept(true);

            private:
                rq_infinite_wait_exception(void);

            friend class result_queue;  // to make instantiable only in containing class
        };

        result_queue(void) = delete;
        result_queue(unsigned int num_threads);
        result_queue(unsigned int num_threads, bool del_inner);
        void queue_job(return_callable<T> *job);
        void queue_job(return_callable<T> *job, bool del_inner);
        T get_result(void);
    private:
        threadpool<tp_callable_wrapper> tp;
        locked_queue<T> rq;
        lq_accessor<T> rq_access;
        int jobs_queued;
        bool delete_inner;
};