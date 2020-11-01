#pragma once
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

struct lq_accessor {
    unique_lock<mutex> lock;
    condition_variable &cond;
    queue<void (*)(void)> &q;

    lq_accessor(mutex &raw_lock, condition_variable &cond_arg, queue<void (*)(void)> &q);

    lq_accessor(void) = delete;
};

class locked_queue {
    public:
        lq_accessor get_access(void);
    
    private:
        mutex raw_lock;
        condition_variable cond;
        queue<void (*)(void)> q;
};

struct switched_thread {
    bool dead = false;
    thread t;
    bool switch_bool = true;

    switched_thread();

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

class threadpool {
	public:
        threadpool(unsigned int n_threads);
        void finalize();
        ~threadpool();
        void queue_job(void (*job)(void));
    private:
        vector<switched_thread> pool;
        locked_queue job_q;
        lq_accessor jq_access;
};

