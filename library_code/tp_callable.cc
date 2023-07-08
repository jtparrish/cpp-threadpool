#include "../library_header/tp_callable.h"

tp_callable::~tp_callable() {}

tp_callable_wrapper::tp_callable_wrapper(tp_callable *c) : inner(c), cleanup(false) {}

tp_callable_wrapper::tp_callable_wrapper(tp_callable *c, bool cu) : inner(c), cleanup(cu) {}

void tp_callable_wrapper::call() {
    inner->call();
    finalize();
}

void tp_callable_wrapper::finalize() {
    if (cleanup) {
        delete_internal();
    }
}

void tp_callable_wrapper::delete_internal() {
    delete inner;
}
