#pragma once
#include "switched_thread.h"
#include "locked_queue.h"

using namespace std;

template <class T>
class threadpool {
	public:
        threadpool(unsigned int n_threads);
        void finalize();
        ~threadpool();
        void queue_job(T);
    private:
        vector<switched_thread> pool;
        locked_queue<T> job_q;
        lq_accessor<T> jq_access;
};