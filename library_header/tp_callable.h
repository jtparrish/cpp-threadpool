#pragma once

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