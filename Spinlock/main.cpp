#include <iostream>
#include "gtest/gtest.h"

//FOR STD LIBRARY
#include <thread>
#include "Spinlock/Spinlock.h"
//FOR BOOST LIBRARY


/**
 * STANDARD LIBRARY TEST
 */

//void job_no_lock(int n, char c) {
//    for (int i = 0; i < n; ++i) {
//        std::cout << c;
//    }
//    std::cout << "\n";
//}
//
//TEST(LOCK, noLockTest) {
//    std::thread thr1(job_no_lock, 100, '!');
//    std::thread thr2(job_no_lock, 100, '@');
//    std::thread thr3(job_no_lock, 100, '#');
//    std::thread thr4(job_no_lock, 100, '$');
//    std::thread thr5(job_no_lock, 100, '%');
//
//    thr1.join();
//    thr2.join();
//    thr3.join();
//    thr4.join();
//    thr5.join();
//}
//
//Spinlock spinlock(1, 1, 1);
//
//void job_with_lock(int n, char c) {
//
//
//    spinlock.lock_wait();
//
//    for (int i = 0; i < n; ++i) {
//        std::cout << c;
//    }
//    std::cout << "\n";
//    spinlock.unlock();
//
//}
//
//TEST(LOCK, LockTest) {
//    std::thread thr1(job_with_lock, 100, '!');
//    std::thread thr2(job_with_lock, 100, '@');
//    std::thread thr3(job_with_lock, 100, '#');
//    std::thread thr4(job_with_lock, 100, '$');
//    std::thread thr5(job_with_lock, 100, '%');
//
//    thr1.join();
//    thr2.join();
//    thr3.join();
//    thr4.join();
//    thr5.join();
//}

TEST(LOCK, testSpinlockUniqueness) {
    std::map<int, Spinlock *> spinlocks;
    Spinlock predefined(0, 0, 0, spinlocks);
    EXPECT_THROW(Spinlock(0, 0, 0, spinlocks), std::runtime_error);
}

TEST(LOCK, testSpinlockMapCheck) {
    std::map<int, Spinlock *> spinlocks;
    Spinlock lock1(0, 0, 0, spinlocks);
    Spinlock lock2(0, 0, 1, spinlocks);
    Spinlock lock3(0, 1, 1, spinlocks);
    Spinlock lock4(1, 1, 1, spinlocks);

    std::stringstream result;
    for (auto &lock : spinlocks) {
        result << lock.second->toString();
    }

    std::string expected(
            "spinlock type: 0, id1: 0, id2: 0\n"
            "spinlock type: 0, id1: 0, id2: 1\n"
            "spinlock type: 0, id1: 1, id2: 1\n"
            "spinlock type: 1, id1: 1, id2: 1\n"
    );

    EXPECT_EQ(expected, result.str());
}


