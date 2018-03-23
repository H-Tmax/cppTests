//
// Created by hspark on 3/21/18.
//

#ifndef INTRAPROCESSLOCKABLE_INTERPROCESSLOCKABLE_H
#define INTRAPROCESSLOCKABLE_INTERPROCESSLOCKABLE_H

#include <chrono>

#include "boost/interprocess/detail/atomic.hpp"
#include "boost/thread.hpp"
#include "boost/atomic.hpp"

/* FOR GLOBALLYLOCKABLE*/
#include "boost/interprocess/sync/interprocess_semaphore.hpp"
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

//typedef boost::interprocess::mutex Mutex;
//typedef boost::condition_variable ConditionVariable;
typedef boost::chrono::microseconds MicroSeconds;

class IntraProcessLockable {
public:
    volatile boost::uint32_t lockval; /* 0 == unlocked & available */

    /* TUNING PARAMETERS */
    MicroSeconds LOCK_ACQ_RETRY_INTERVAL;
    MicroSeconds WAIT_TIMEOUT;
    unsigned int LOCK_ACQ_TRIAL_BEFORE_WAIT;

    /* FOR THE USE OF CONDITION VARIABLE */
//    Mutex mtx;
//    ConditionVariable cd;

    /* FOR ATOMIC LIBRARY */
    boost::atomic_int32_t lockval2;



    /* FOR GLOBALLYLOCKABLE*/
    typedef boost::interprocess::interprocess_mutex Mutex;
    typedef boost::interprocess::interprocess_condition  ConditionVariable;
};

#endif //INTRAPROCESSLOCKABLE_INTERPROCESSLOCKABLE_H
