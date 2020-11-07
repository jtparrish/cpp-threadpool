#include "switched_thread.h"

switched_thread::switched_thread() {}

switched_thread::switched_thread(thread t_arg) : dead(false), t(std::move(t_arg)), switch_bool(true) {}

// Move Constructor
switched_thread::switched_thread(switched_thread && obj) : dead(obj.dead), t(std::move(obj.t)), switch_bool(obj.switch_bool) {
    obj.dead = true;
}

//Move Assignment Operator
switched_thread &switched_thread::operator=(switched_thread && obj) {
    t = std::move(obj.t);
    switch_bool = obj.switch_bool;
    dead = obj.dead;
    obj.dead = true;
    return *this;
}

//Destructor
switched_thread::~switched_thread() {
    if (!dead) {
        switch_bool = false;
        t.join();
    }
}