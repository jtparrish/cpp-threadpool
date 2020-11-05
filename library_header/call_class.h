#pragma once
#include "tp_callable.h"

class call_class : public tp_callable {
    public:
        call_class(int a);
        void call(void);
    private:
        int i;

};