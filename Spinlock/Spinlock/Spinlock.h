//
// Created by hspark on 2/20/18.
//

#ifndef SPINLOCK_SPINLOCK_H
#define SPINLOCK_SPINLOCK_H

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <chrono>

class Spinlock {
private:
    unsigned int lockval; /* 0 == unlocked & available */
    std::mutex mtx; /* protecting lockval */
    std::condition_variable cd;

    /* SPINLOCK IDENTIFICATION INFORMATION */
    int spinlockType;
    int id1;
    int id2;

    /* TUNING PARAMETERS */
    std::chrono::microseconds LOCK_ACQ_RETRY_INTERVAL;
    std::chrono::microseconds WAIT_TIMEOUT;
    unsigned int LOCK_ACQ_TRIAL_BEFORE_WAIT;

public:
    Spinlock(int type, int id1, int id2) :
            lockval(0),
            spinlockType(type),
            id1(id1),
            id2(id2),
            LOCK_ACQ_RETRY_INTERVAL(10),
            WAIT_TIMEOUT(10),
            LOCK_ACQ_TRIAL_BEFORE_WAIT(10) {}

    Spinlock(int type, int id1) :
            lockval(0),
            spinlockType(type),
            id1(id1),
            id2(0),
            LOCK_ACQ_RETRY_INTERVAL(10),
            WAIT_TIMEOUT(10),
            LOCK_ACQ_TRIAL_BEFORE_WAIT(10) {}

    Spinlock(int type) :
            lockval(0),
            spinlockType(type),
            id1(0),
            id2(0),
            LOCK_ACQ_RETRY_INTERVAL(10),
            WAIT_TIMEOUT(10),
            LOCK_ACQ_TRIAL_BEFORE_WAIT(10) {}

    void lock_busy() {
        mtx.lock();
        while (lockval != 0) {
            mtx.unlock();
            std::this_thread::sleep_for(LOCK_ACQ_RETRY_INTERVAL);
            mtx.lock();
        }
        lockval++;
        mtx.unlock();
    }

    bool lock_try() {
        mtx.lock();
        if (lockval == 0) {
            lockval++;
            mtx.unlock();
            return true;
        } else {
            mtx.unlock();
            return false;
        }
    }

    void lock_wait() {
        for (int i = 1; i < LOCK_ACQ_TRIAL_BEFORE_WAIT; i++) {
            mtx.lock();
            if (lockval != 0) {
                mtx.unlock();
                std::this_thread::sleep_for(LOCK_ACQ_RETRY_INTERVAL);
                continue;
            } else {
                /* LOCK ACQUIRED*/
                lockval++;
                mtx.unlock();
                return;
            }
        }
        /* FAILED TO ACQUIRE THE LOCK */
        std::unique_lock<std::mutex> ul(mtx);
        while (lockval != 0) {
            cd.wait_for(ul, WAIT_TIMEOUT);
        }
        lockval++;
        return;
    }

    void unlock() {
        std::unique_lock<std::mutex> ul(mtx);
        if (lockval < 1) {
            //TODO: assert or throw a critical error
        }
        lockval--;
        ul.unlock();
        cd.notify_all();
    }

    void setLockAcqInterval(unsigned int ms) {
        this->LOCK_ACQ_RETRY_INTERVAL = std::chrono::microseconds(ms);
    }

    void setWaitTimeout(unsigned int ms) {
        this->WAIT_TIMEOUT = std::chrono::microseconds(ms);
    }

    void setLockAcqTrial(unsigned int n) {
        this->LOCK_ACQ_TRIAL_BEFORE_WAIT = n;
    }
};


#endif //SPINLOCK_SPINLOCK_H
