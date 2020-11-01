#include "threadpool.h"
#include <iostream>
// #include <string>

#define NUM_THREADS 8

using std::cout;

template <int a>
void job(void);

int main(void) {
    threadpool tp(NUM_THREADS);
    cout << ("queuing job: " + std::to_string(1) + "\n");
    tp.queue_job(job<1>);

    cout << ("queuing job: " + std::to_string(2) + "\n");
    tp.queue_job(job<2>);

    cout << ("queuing job: " + std::to_string(3) + "\n");
    tp.queue_job(job<3>);

    cout << ("queuing job: " + std::to_string(4) + "\n");
    tp.queue_job(job<4>);

    cout << ("queuing job: " + std::to_string(5) + "\n");
    tp.queue_job(job<5>);

    cout << ("queuing job: " + std::to_string(6) + "\n");
    tp.queue_job(job<6>);

    cout << ("queuing job: " + std::to_string(7) + "\n");
    tp.queue_job(job<7>);

    cout << ("queuing job: " + std::to_string(8) + "\n");
    tp.queue_job(job<8>);

    cout << ("queuing job: " + std::to_string(9) + "\n");
    tp.queue_job(job<9>);

    cout << ("queuing job: " + std::to_string(10) + "\n");
    tp.queue_job(job<10>);

    cout << ("queuing job: " + std::to_string(11) + "\n");
    tp.queue_job(job<11>);

    cout << ("queuing job: " + std::to_string(12) + "\n");
    tp.queue_job(job<12>);

    cout << ("queuing job: " + std::to_string(13) + "\n");
    tp.queue_job(job<13>);

    cout << ("queuing job: " + std::to_string(14) + "\n");
    tp.queue_job(job<14>);

    cout << ("queuing job: " + std::to_string(15) + "\n");
    tp.queue_job(job<15>);

    cout << ("queuing job: " + std::to_string(16) + "\n");
    tp.queue_job(job<16>);
}

template <int a>
void job(void) {
    string s = ("running job: " + std::to_string(a) +"\n");
    cout << s;
}