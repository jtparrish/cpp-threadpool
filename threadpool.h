#pragma once
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

class tp_callable {
    public:
        virtual void call(void) = 0;
        virtual ~tp_callable(void);
};

class tp_callable_wrapper {
    public:
        tp_callable_wrapper(tp_callable *c);
        tp_callable_wrapper(tp_callable *c, bool cu);
        void call(void);
        void delete_internal(void);
        void finalize();
    private:
        tp_callable *inner;
        const bool cleanup;
};


template<class T>
struct lq_accessor {
    unique_lock<mutex> lock;
    condition_variable &cond;
    queue<T> &q;

    lq_accessor(mutex &raw_lock, condition_variable &cond_arg, queue<T> &q);

    lq_accessor(void) = delete;
};

template <class T>
class locked_queue {
    public:
        lq_accessor<T> get_access(void);
    
    private:
        mutex raw_lock;
        condition_variable cond;
        queue<T> q;
};

struct switched_thread {
    bool dead;
    thread t;
    bool switch_bool;

    switched_thread(thread t_arg);

    // https://thispointer.com/c11-how-to-use-stdthread-as-a-member-variable-in-class/

    //Delete the copy constructor
    switched_thread(const switched_thread&) = delete;

    //Delete the Assignment opeartor
    switched_thread& operator=(const switched_thread&) = delete;

    // Move Constructor
    switched_thread(switched_thread && obj);

    //Move Assignment Operator
    switched_thread & operator=(switched_thread && obj);

    //Destructor
    ~switched_thread();
};

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