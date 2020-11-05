#pragma once
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

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