//
// Created by hspark on 3/21/18.
//

#ifndef INTRAPROCESSLOCKABLE_IPL_ATOMIC_SEM_H
#define INTRAPROCESSLOCKABLE_IPL_ATOMIC_SEM_H

#include <chrono>
#include "boost/thread.hpp"
#include "boost/atomic.hpp"

typedef boost::mutex Mutex;
typedef boost::condition_variable ConditionVariable;
typedef boost::chrono::microseconds MicroSeconds;

class IPL_ATM_SEM {
public:
    IPL_ATM_SEM() :
            lockval(0),
            LOCK_ACQ_RETRY_INTERVAL(10),
            WAIT_TIMEOUT(10),
            LOCK_ACQ_TRIAL_BEFORE_WAIT(10) {}


    void lock_busy() {
        //REQUIRES AN ADDITIONAL VARIABLE :(
        int available = 0;
        while (!lockval.compare_exchange_strong(available, 1)) {
            boost::this_thread::sleep_for(LOCK_ACQ_RETRY_INTERVAL);
            //Compare current value with expected, change it to desired if matches.
            //Returns true if an exchange has been performed, and always writes the previous value back in expected
            available = 0;
        }
    }

    bool lock_try() {
        //REQUIRES AN ADDITIONAL VARIABLE :(
        int available = 0;
        return lockval.compare_exchange_strong(available, 1);
    }

    void lock_wait() {
        for (int i = 0; i < LOCK_ACQ_TRIAL_BEFORE_WAIT; i++) {
            if (lock_try() != true) {
                boost::this_thread::sleep_for(LOCK_ACQ_RETRY_INTERVAL);
                continue;
            } else {
                /* LOCK ACQUIRED */
                lockval++;
                return;
            }
        }
        /* FAILED TO ACQUIRE THE LOCK */
        boost::unique_lock<Mutex> ul(mtx);
        while (lockval.load() != 0) { /* In case awakened, but someone took it before me */
            cd.wait_for(ul, WAIT_TIMEOUT);
        }

        /* LOCK ACQUIRED */
        lockval++;
        return;
    }

    void unlock() {
        if (lockval.load() < 1) {
            //TODO: assert or throw a critical error
        }
        lockval.exchange(0);
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

private:
    /* TUNING PARAMETERS */
    MicroSeconds LOCK_ACQ_RETRY_INTERVAL;
    MicroSeconds WAIT_TIMEOUT;
    unsigned int LOCK_ACQ_TRIAL_BEFORE_WAIT;

    /* FOR THE USE OF CONDITION VARIABLE */
    Mutex mtx;
    ConditionVariable cd;

    /* FOR ATOMIC LIBRARY */
    boost::atomic_int32_t lockval;
};

#endif //INTRAPROCESSLOCKABLE_IPL_ATOMIC_SEM_H
