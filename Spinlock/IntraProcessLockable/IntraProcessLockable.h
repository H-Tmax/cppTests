//
// Created by hspark on 2/20/18.
//

#ifndef SPINLOCK_SPINLOCK_H
#define SPINLOCK_SPINLOCK_H

#include <chrono>

#include "boost/interprocess/detail/atomic.hpp"
#include "boost/thread.hpp"
#include <boost/atomic.hpp>
#include <mutex>

/* FOR GLOBALLYLOCKABLE*/
//#include "boost/interprocess/sync/interprocess_semaphore.hpp"
//#include <boost/interprocess/sync/interprocess_mutex.hpp>
//#include <boost/interprocess/sync/interprocess_condition.hpp>

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

    /* FOR GLOBALLYLOCKABLE*/
//    typedef boost::interprocess::interprocess_mutex Mutex;
//    typedef boost::interprocess::interprocess_condition  ConditionVariable;

    /* FOR SEMAPHORE VERISON */
    //boost::int


public:
    IntraProcessLockable() :
            lockval(0),
            LOCK_ACQ_RETRY_INTERVAL(10),
            WAIT_TIMEOUT(10),
            LOCK_ACQ_TRIAL_BEFORE_WAIT(10) {}


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
        for (int i = 1; i < LOCK_ACQ_TRIAL_BEFORE_WAIT; i++) {
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
        lockval++;
        return;
    }

    void unlock_mtx() {
        std::unique_lock<Mutex> ul(mtx);
        if (lockval < 1) {
            //TODO: assert or throw a critical error
        }
        lockval--;
        ul.unlock();
        cd.notify_all();
    }

    ///////////////////////
    ///* BOOST VERSION *///
    ///////////////////////
    void lock_busy_boost() {
        while (boost::interprocess::ipcdetail::atomic_cas32(&lockval, 1, 0) != 0) {
            boost::this_thread::sleep_for(LOCK_ACQ_RETRY_INTERVAL);
            boost::atomics::atomic_int32_t a;
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
        for (int i = 1; i < LOCK_ACQ_TRIAL_BEFORE_WAIT; i++) {
            if (lock_try_boost() != true) {
                boost::this_thread::sleep_for(LOCK_ACQ_RETRY_INTERVAL);
                continue;
            } else {
                /* LOCK ACQUIRED*/
                return;
            }
        }
        /* FAILED TO ACQUIRE THE LOCK */
        boost::unique_lock<Mutex> ul(mtx);
        while (lockval != 0) {
            cd.wait_for(ul, WAIT_TIMEOUT);
        }
        lockval++;
        return;
    }

    void unlock_boost() {
        if (boost::interprocess::ipcdetail::atomic_read32(&lockval) < 1) {
            //TODO: assert or throw a critical error
        }
        cd.notify_all();
        boost::interprocess::ipcdetail::atomic_cas32(&lockval, 0, lockval);
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


#endif //SPINLOCK_SPINLOCK_H
