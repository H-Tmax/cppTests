//
// Created by hspark on 3/21/18.
//

#ifndef INTRAPROCESSLOCKABLE_IPL_BOOST_CDV_H
#define INTRAPROCESSLOCKABLE_IPL_BOOST_CDV_H

#include <chrono>

#include "boost/interprocess/detail/atomic.hpp"
#include "boost/thread.hpp"
#include "boost/atomic.hpp"

typedef boost::mutex Mutex;
typedef boost::condition_variable ConditionVariable;
typedef boost::chrono::microseconds MicroSeconds;

class IPL_BST_CDV {
public:
    IPL_BST_CDV() :
            lockval(0),
            LOCK_ACQ_RETRY_INTERVAL(10),
            WAIT_TIMEOUT(10),
            LOCK_ACQ_TRIAL_BEFORE_WAIT(10) {}

    void lock_busy() {
        while (boost::interprocess::ipcdetail::atomic_cas32(&lockval, 1, 0) != 0) {
            boost::this_thread::sleep_for(LOCK_ACQ_RETRY_INTERVAL);
        }
    }

    bool lock_try() {
        if (boost::interprocess::ipcdetail::atomic_cas32(&lockval, 1, 0) == 0) {
            return true;
        } else {
            return false;
        }
    }

    void lock_wait() {
        for (int i = 0; i < LOCK_ACQ_TRIAL_BEFORE_WAIT; i++) {
            if (lock_try() != true) {
                boost::this_thread::sleep_for(LOCK_ACQ_RETRY_INTERVAL);
                continue;
            } else {
                /* LOCK ACQUIRED*/
                boost::interprocess::ipcdetail::atomic_inc32(&lockval);
                return;
            }
        }
        /* FAILED TO ACQUIRE THE LOCK */
        boost::unique_lock<Mutex> ul(mtx);
        while (boost::interprocess::ipcdetail::atomic_read32(&lockval) != 0) {
            cd.wait_for(ul, WAIT_TIMEOUT);
        }

        /* Acquired the Lock */
        boost::interprocess::ipcdetail::atomic_inc32(&lockval);
        return;
    }

    void unlock() {
        if (boost::interprocess::ipcdetail::atomic_read32(&lockval) < 1) {
            //TODO: assert or throw a critical error
        }
        boost::interprocess::ipcdetail::atomic_cas32(&lockval, 0, lockval);
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
    volatile boost::uint32_t lockval; /* 0 == unlocked & available */

    /* TUNING PARAMETERS */
    MicroSeconds LOCK_ACQ_RETRY_INTERVAL;
    MicroSeconds WAIT_TIMEOUT;
    unsigned int LOCK_ACQ_TRIAL_BEFORE_WAIT;

    /* FOR THE USE OF CONDITION VARIABLE */
    Mutex mtx;
    ConditionVariable cd;
};


#endif //INTRAPROCESSLOCKABLE_IPL_BOOST_CDV_H
