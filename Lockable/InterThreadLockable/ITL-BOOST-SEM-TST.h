//
// Created by hspark on 3/28/18.
//

#ifndef INTRAPROCESSLOCKABLE_IPL_BOOST_SEM_TST_H
#define INTRAPROCESSLOCKABLE_IPL_BOOST_SEM_TST_H

#include <map>
#include <chrono>
#include <thread>
#include <boost/chrono/duration.hpp>
#include <boost/interprocess/detail/atomic.hpp>
#include "boost/interprocess/sync/interprocess_semaphore.hpp"
#include <boost/thread.hpp>
#include <iostream>

typedef boost::chrono::microseconds MicroSeconds;

class Turnstile {
public:
    Turnstile() : semaphore(nullptr), next(nullptr) {}

    std::string lock_addr;
    sem_t *semaphore;
    Turnstile *next;
};

std::map<std::string, Turnstile *> turnstiles;

class ThreadInfo {
public:
    ThreadInfo() : turnstile() { sem_init(&semaphore, 0, 0); }

    Turnstile turnstile;
    sem_t semaphore;
};

class IPL_BST_SEM_TST {
public:
    boost::mutex mtx;

    IPL_BST_SEM_TST() :
            lockval(0),
            LOCK_ACQ_RETRY_INTERVAL(10),
            WAIT_TIMEOUT(10),
            LOCK_ACQ_TRIAL_BEFORE_WAIT(10),
            retry_flag(false) {
        const void *address = static_cast<const void *>(this);
        std::stringstream ss;
        ss << address;
        key = ss.str();
    }

    bool lock_try() {
        if (boost::interprocess::ipcdetail::atomic_cas32(&lockval, 1, 0) == 0) {
            return true;
        } else {
            return false;
        }
    }

    void lock_wait(ThreadInfo *_cur_thr) {
        cur_thr = _cur_thr;
        boost::thread::id thr_id = boost::this_thread::get_id();
        std::cout << "\n" << "_cur_thr: " << _cur_thr << std::endl;
        std::cout << "cur_thr: " << cur_thr << std::endl;
        cur_thr->turnstile.lock_addr = key;
        cur_thr->turnstile.next = nullptr;
        cur_thr->turnstile.semaphore = &(cur_thr->semaphore);

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
            if (retry_flag) {
                sem_wait(&cur_thr->semaphore);
            }
            mtx.lock();
            if (turnstiles.find(key) == turnstiles.end()) {
                turnstiles[key] = &(cur_thr->turnstile);
                std::cout << "\n" << thr_id << " is adding " << &(cur_thr->turnstile) << " in the waitlist" << std::endl;
            } else {
                std::cout << "ACQ - Currently in the waitlist: " << std::endl;
                Turnstile *prev = turnstiles[key];
                std::cout << prev << ", ";
                Turnstile *for_traverse = turnstiles[key]->next;
                while (for_traverse != nullptr) {
                    std::cout << for_traverse << ", ";
                    prev = for_traverse;
                    for_traverse = for_traverse->next;
                }
                std::cout << "\n" << thr_id << " is adding " << &(cur_thr->turnstile) << " in the waitlist" << std::endl;
                prev->next = &(cur_thr->turnstile);
            }
            mtx.unlock();
            sem_wait(&cur_thr->semaphore);
            retry_flag = true;
        }

        /* Acquired the Lock */
        boost::interprocess::ipcdetail::atomic_inc32(&lockval);
        return;
    }

    void unlock() {
        if (boost::interprocess::ipcdetail::atomic_read32(&lockval) < 1) {
            //TODO: assert or throw a critical error
        }

        boost::thread::id thr_id = boost::this_thread::get_id();
        std::cout << "\n" << thr_id << " is releasing the lock" << std::endl;
        mtx.lock();
        if (turnstiles.size() > 0) {
            std::cout << "REL - Currently in the waitlist: " << std::endl;
            Turnstile *prev = turnstiles[key];
            std::cout << prev << ", ";
            Turnstile *for_traverse = turnstiles[key]->next;
            while (for_traverse != nullptr) {
                std::cout << for_traverse << ", ";
                sem_post(prev->semaphore);
                prev = for_traverse;
                for_traverse = for_traverse->next;
            }
            sem_post(prev->semaphore);
        }
        std::cout << std::endl;
        turnstiles.erase(key);
        mtx.unlock();
        boost::interprocess::ipcdetail::atomic_cas32(&lockval, 0, lockval);

    }

    ThreadInfo *cur_thr;
    std::string key;

private:
    volatile boost::uint32_t lockval; /* 0 == unlocked & available */

    /* TUNING PARAMETERS */
    MicroSeconds LOCK_ACQ_RETRY_INTERVAL;
    MicroSeconds WAIT_TIMEOUT;
    unsigned int LOCK_ACQ_TRIAL_BEFORE_WAIT;
    bool retry_flag;
};


#endif //INTRAPROCESSLOCKABLE_IPL_BOOST_SEM_TST_H
