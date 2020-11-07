#pragma once
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

template <class T>
class locked_queue {
    public:
        struct lq_accessor {
            unique_lock<mutex> lock;
            condition_variable &cond;
            queue<T> &q;
            lq_accessor(void) = delete;
            private:
                lq_accessor(mutex &raw_lock, condition_variable &cond_arg, queue<T> &q);

            friend class locked_queue; // to make instantiable only in containing class
        };

        lq_accessor get_access(void);
    
    private:
        mutex raw_lock;
        condition_variable cond;
        queue<T> q;
};

template <class T>
using lq_accessor = typename locked_queue<T>::lq_accessor;