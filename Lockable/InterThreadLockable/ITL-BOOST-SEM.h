//
// Created by hspark on 3/21/18.
//

#ifndef INTRAPROCESSLOCKABLE_IPL_BOOST_SEM_H
#define INTRAPROCESSLOCKABLE_IPL_BOOST_SEM_H


#include <chrono>
#include "boost/interprocess/sync/interprocess_semaphore.hpp"

typedef boost::mutex Mutex;
typedef boost::condition_variable ConditionVariable;
typedef boost::chrono::microseconds MicroSeconds;

class IPL_BST_SEM {
public:
    IPL_BST_SEM() :
            lockval(0),
            LOCK_ACQ_RETRY_INTERVAL(10),
            WAIT_TIMEOUT(10),
            LOCK_ACQ_TRIAL_BEFORE_WAIT(10) {sem_init(&semaphore, 0, 0);}

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
        while (boost::interprocess::ipcdetail::atomic_read32(&lockval) != 0) {
            sem_wait(&semaphore);
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
        sem_post(&semaphore);
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
    sem_t semaphore;
};

#endif //INTRAPROCESSLOCKABLE_IPL_BOOST_SEM_H
