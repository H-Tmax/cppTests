//
// Created by hspark on 3/21/18.
//

#ifndef INTRAPROCESSLOCKABLE_IPL_MTX_CDV_H
#define INTRAPROCESSLOCKABLE_IPL_MTX_CDV_H

#include <chrono>

#include "boost/thread.hpp"

typedef boost::mutex Mutex;
typedef boost::condition_variable ConditionVariable;
typedef boost::chrono::microseconds MicroSeconds;

class IPL_MTX_CDV {
private:
    volatile boost::uint32_t lockval; /* 0 == unlocked & available */

    /* TUNING PARAMETERS */
    MicroSeconds LOCK_ACQ_RETRY_INTERVAL;
    MicroSeconds WAIT_TIMEOUT;
    unsigned int LOCK_ACQ_TRIAL_BEFORE_WAIT;

    /* FOR THE USE OF CONDITION VARIABLE */
    Mutex mtx;
    ConditionVariable cd;

public:
    IPL_MTX_CDV() :
            lockval(0),
            LOCK_ACQ_RETRY_INTERVAL(10),
            WAIT_TIMEOUT(10),
            LOCK_ACQ_TRIAL_BEFORE_WAIT(10) {}

    void lock_busy() {
        mtx.lock();
        while (lockval != 0) {
            mtx.unlock();
            boost::this_thread::sleep_for(LOCK_ACQ_RETRY_INTERVAL);
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
        for (int i = 0; i < LOCK_ACQ_TRIAL_BEFORE_WAIT; i++) {
            mtx.lock();
            if (lockval != 0) {
                mtx.unlock();
                boost::this_thread::sleep_for(LOCK_ACQ_RETRY_INTERVAL);
                continue;
            } else {
                /* LOCK ACQUIRED*/
                lockval++;
                mtx.unlock();
                return;
            }
        }

        /* FAILED TO ACQUIRE THE LOCK */
        boost::unique_lock<Mutex> ul(mtx);
        while (lockval != 0) {
            cd.wait_for(ul, WAIT_TIMEOUT);
        }

        /* Acquired the lock */
        lockval++;
        return;
    }

    void unlock() {
        boost::unique_lock<Mutex> ul(mtx);
        if (lockval < 1) {
            //TODO: assert or throw a critical error
        }
        lockval--;
        ul.unlock();
        cd.notify_all();
    }

    void setLockAcqInterval(unsigned int ms) {
        this->LOCK_ACQ_RETRY_INTERVAL = MicroSeconds(ms);
    }

    void setWaitTimeout(unsigned int ms) {
        this->WAIT_TIMEOUT = MicroSeconds(ms);
    }

    void setLockAcqTrial(unsigned int n) {
        this->LOCK_ACQ_TRIAL_BEFORE_WAIT = n;
    }
};


#endif //INTRAPROCESSLOCKABLE_IPL_MTX_CDV_H
