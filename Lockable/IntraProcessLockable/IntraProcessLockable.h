//
// Created by hspark on 2/20/18.
//

#ifndef SPINLOCK_SPINLOCK_H
#define SPINLOCK_SPINLOCK_H

#include <chrono>

#include "boost/interprocess/detail/atomic.hpp"
#include "boost/thread.hpp"
#include "boost/atomic.hpp"

typedef boost::mutex Mutex;
typedef boost::condition_variable ConditionVariable;
typedef boost::chrono::microseconds MicroSeconds;

class IntraProcessLockable {
public:
    volatile boost::uint32_t lockval; /* 0 == unlocked & available */

    /* TUNING PARAMETERS */
    MicroSeconds LOCK_ACQ_RETRY_INTERVAL;
    MicroSeconds WAIT_TIMEOUT;
    unsigned int LOCK_ACQ_TRIAL_BEFORE_WAIT;

    /* FOR THE USE OF CONDITION VARIABLE */
    Mutex mtx;
    ConditionVariable cd;

    /* FOR ATOMIC LIBRARY */
    boost::atomic_int32_t lockval2;


    IntraProcessLockable() :
            lockval(0),
            LOCK_ACQ_RETRY_INTERVAL(10),
            WAIT_TIMEOUT(10),
            LOCK_ACQ_TRIAL_BEFORE_WAIT(10) {}

    ////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////PERFORMANCE COMPARISON START/////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////

    /////////////////////
    ///* MTX VERSION *///
    /////////////////////
    void lock_busy_mtx() {
        mtx.lock();
        while (lockval != 0) {
            mtx.unlock();
            boost::this_thread::sleep_for(LOCK_ACQ_RETRY_INTERVAL);
            mtx.lock();
        }
        lockval++;
        mtx.unlock();
    }

    bool lock_try_mtx() {
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

    void lock_wait_mtx() {
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

    void unlock_mtx() {
        boost::unique_lock<Mutex> ul(mtx);
        if (lockval < 1) {
            //TODO: assert or throw a critical error
        }
        lockval--;
        ul.unlock();
        cd.notify_all();
    }

    //////////////////////////////////
    ///* BOOST IPC DETAIL VERSION *///
    //////////////////////////////////
    void lock_busy_boost() {
        while (boost::interprocess::ipcdetail::atomic_cas32(&lockval, 1, 0) != 0) {
            boost::this_thread::sleep_for(LOCK_ACQ_RETRY_INTERVAL);
        }
    }

    bool lock_try_boost() {
        if (boost::interprocess::ipcdetail::atomic_cas32(&lockval, 1, 0) == 0) {
            return true;
        } else {
            return false;
        }
    }

    void lock_wait_boost() {
        for (int i = 0; i < LOCK_ACQ_TRIAL_BEFORE_WAIT; i++) {
            if (lock_try_boost() != true) {
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

    void unlock_boost() {
        if (boost::interprocess::ipcdetail::atomic_read32(&lockval) < 1) {
            //TODO: assert or throw a critical error
        }
        boost::interprocess::ipcdetail::atomic_cas32(&lockval, 0, lockval);
        cd.notify_all();
    }

    //////////////////////////////////
    ///* BOOST ATOMIC LIB VERSION *///
    //////////////////////////////////
    void lock_busy_atomic() {
        //REQUIRES AN ADDITIONAL VARIABLE :(
        int available = 0;
        while (!lockval2.compare_exchange_strong(available, 1)) {
            boost::this_thread::sleep_for(LOCK_ACQ_RETRY_INTERVAL);
            //Compare current value with expected, change it to desired if matches.
            //Returns true if an exchange has been performed, and always writes the previous value back in expected
            available = 0;
        }
    }

    bool lock_try_atomic() {
        //REQUIRES AN ADDITIONAL VARIABLE :(
        int available = 0;
        return lockval2.compare_exchange_strong(available, 1);
    }

    void lock_wait_atomic() {
        for (int i = 0; i < LOCK_ACQ_TRIAL_BEFORE_WAIT; i++) {
            if (lock_try_boost() != true) {
                boost::this_thread::sleep_for(LOCK_ACQ_RETRY_INTERVAL);
                continue;
            } else {
                /* LOCK ACQUIRED */
                lockval2++;
                return;
            }
        }
        /* FAILED TO ACQUIRE THE LOCK */
        boost::unique_lock<Mutex> ul(mtx);
        while (lockval2.load() != 0) { /* In case awakened, but someone took it before me */
            cd.wait_for(ul, WAIT_TIMEOUT);
        }

        /* LOCK ACQUIRED */
        lockval2++;
        return;
    }

    void unlock_atomic() {
        if (lockval2.load() < 1) {
            //TODO: assert or throw a critical error
        }
        lockval2.exchange(0);
        cd.notify_all();
    }

    /////////////////////////////////////////////////
    ///* BOOST IPC DETAIL VERSION WITH SEMAPHORE *///
    /////////////////////////////////////////////////


    /////////////////////////////////////////////////
    ///* BOOST ATOMIC LIB VERSION WITH SEMAPHORE *///
    /////////////////////////////////////////////////









    //////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////PERFORMANCE COMPARISON END/////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////

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


#endif //SPINLOCK_SPINLOCK_H
