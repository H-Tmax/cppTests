//
// Created by hspark on 2/20/18.
//

#ifndef SPINLOCK_SPINLOCK_H
#define SPINLOCK_SPINLOCK_H

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <unistd.h>

class Spinlock {
private:
    int lockval; // 1 unlocked
    std::mutex mtx;
    std::condition_variable cd;

public:
    Spinlock() : lockval(1){}

    void lock_busy() {
        mtx.lock();
        while (lockval != 1) {
            mtx.unlock();
            usleep(1);
            mtx.lock();
        }
        lockval--;
        mtx.unlock();
    }

    bool lock_try() {
        return mtx.try_lock();
    }

    void sleep_lock() {
        std::unique_lock<std::mutex> ul(mtx);
        while (lockval != 1) {
            cd.wait(ul);
        }
        lockval--;
    }

    void lock_try_n(int n) {

    }

    void unlock() {

        std::unique_lock<std::mutex> ul(mtx);
        lockval++;
        ul.unlock();
        cd.notify_all();
    }
};


#endif //SPINLOCK_SPINLOCK_H
