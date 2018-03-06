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
#include <map>
#include <sstream>
#include <string>

class Spinlock {
private:
    unsigned int lockval; /* 0 == unlocked & available */
    std::mutex mtx; /* protecting lockval */
    std::condition_variable cd;

    /* SPINLOCK IDENTIFICATION INFORMATION */
    int spinlockType;
    long int id1;
    long int id2;

    /* TUNING PARAMETERS */
    std::chrono::microseconds LOCK_ACQ_RETRY_INTERVAL;
    std::chrono::microseconds WAIT_TIMEOUT;
    unsigned int LOCK_ACQ_TRIAL_BEFORE_WAIT;
    unsigned int LOCK_HASH_BUCKET_SIZE = 10; /* THIS IS LIKELY TO BE A GLOBAL PARAMETER*/

    /* FOR LOCK MAP*/
    std::map<int, Spinlock *> &lockMap;
    //std::set<std::tuple<int, long int, long int>, Spinlock *> &lockMap;

    Spinlock *next; /* FOR HASH BUCKET COLLISION */

    int getMapKey(int type, long int id1, long int id2) { /* USING THE HASH OF POINTER ADDRESS AS KEY*/
        int key((type + id1 + id2) % LOCK_HASH_BUCKET_SIZE);
        return key;
    }

    void verifyDuplication(const Spinlock *s) {
        if (s->getSpinlockType() == this->getSpinlockType()
            && s->getId1() == this->getId1()
            && s->getId2() == this->getId2()) {
            throw (std::runtime_error("[CRITICAL ERROR]SPECIFIED SPINLOCK ALREADY EXISTS"));
        }
    }

    void addToMap(int type, long int id1, long int id2) {
        int key = this->getMapKey(type, id1, id2);
        auto found = this->lockMap.find(key);

        if (found == this->lockMap.end()) {
            this->lockMap[key] = this; /* insert */
        } else {
            Spinlock *s = found->second;
            while (s->next != nullptr || s == found->second) {
                verifyDuplication(s);
                s = s->next;
            }
            s->next = this;
        }
    }

public:
    Spinlock(int type, long int id1, long int id2, std::map<int, Spinlock *> &lockMap) :
            lockval(0),
            spinlockType(type),
            id1(id1),
            id2(id2),
            LOCK_ACQ_RETRY_INTERVAL(10),
            WAIT_TIMEOUT(10),
            LOCK_ACQ_TRIAL_BEFORE_WAIT(10),
            lockMap(lockMap),
            next(nullptr) { this->addToMap(type, id1, id2); }

    Spinlock(int type, long int id1, std::map<int, Spinlock *> &lockMap) :
            lockval(0),
            spinlockType(type),
            id1(id1),
            id2(0),
            LOCK_ACQ_RETRY_INTERVAL(10),
            WAIT_TIMEOUT(10),
            LOCK_ACQ_TRIAL_BEFORE_WAIT(10),
            lockMap(lockMap),
            next(nullptr) { this->addToMap(type, id1, 0); }

    Spinlock(int type, std::map<int, Spinlock *> &lockMap) :
            lockval(0),
            spinlockType(type),
            id1(0),
            id2(0),
            LOCK_ACQ_RETRY_INTERVAL(10),
            WAIT_TIMEOUT(10),
            LOCK_ACQ_TRIAL_BEFORE_WAIT(10),
            lockMap(lockMap),
            next(nullptr) { this->addToMap(type, 0, 0); }

//    ~Spinlock() {
//        int key = this->getMapKey(this->getSpinlockType(), this->getId1(), this->getId2());
//        if (this->lockMap.find(key) == this->lockMap.end()) {
//            throw ("[CRITICAL ERROR]SPECIFIED SPINLOCK DOES NOT EXIST");
//        } else {
//            Spinlock &cur = this->lockMap.at(key);
//            Spinlock &prev = cur;
//            while (cur.next != nullptr) {
//                if (cur.getSpinlockType() == this->getSpinlockType()
//                    && cur.getId1() == this->getId1()
//                    && cur.getId2() == this->getId2()) {
//                    if (cur.next == nullptr) {
//                        prev.next = nullptr;
//                        break;
//                    } else {
//                        prev.next = cur.next;
//                        break;
//                    }
//                }
//                prev = cur;
//                cur = *cur.next;
//            }
//        }
//    }

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

    int getSpinlockType() const {
        return spinlockType;
    }

    int getId1() const {
        return id1;
    }

    int getId2() const {
        return id2;
    }

    std::string toString(){
        std::stringstream s;
        s << "spinlock type: " << getSpinlockType();
        s << ", id1: " << getId1();
        s << ", id2: " << getId2();
        s << "\n";
        return s.str();
    }
};


#endif //SPINLOCK_SPINLOCK_H
