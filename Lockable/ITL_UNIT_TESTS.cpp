#include <iostream>
#include "gtest/gtest.h"

#include <thread>
#include "../CommunicationProtocol/DevKit/DevKit.h"
#include "InterThreadLockable/InterThreadLockable.h"
#include "InterThreadLockable/ITL-ATOMIC-CDV.h"
#include "InterThreadLockable/ITL-ATOMIC-SEM.h"
#include "InterThreadLockable/ITL-BOOST-CDV.h"
#include "InterThreadLockable/ITL-BOOST-SEM.h"
#include "InterThreadLockable/ITL-MTX-CDV.h"
#include "InterThreadLockable/ITL-MTX-SEM.h"
#include "InterThreadLockable/ITL-BOOST-SEM-TST.h"

void job_no_lock(int n, int *result) {
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
}

TEST(LOCK, noLockTest) {
    int trial = 1000000;
    int thread_cnt = 10;
    int result = 0;

    boost::thread_group thread_group;
    for (int i = 0; i < thread_cnt; i++) {
        thread_group.create_thread(boost::bind(job_no_lock, trial, &result));
    }
    thread_group.join_all();

    int desired = trial * thread_cnt;
    EXPECT_NE(desired, result);
}

//////////////////////
////LOCKS WITH CDV////
//////////////////////
IPL_MTX_CDV lock_mtx_cdv;
IPL_BST_CDV lock_bst_cdv;
IPL_ATM_CDV lock_atm_cdv;


////////////////
////WITH MTX////
////////////////
void job_with_busy_lock_mtx(int n, int *result) {
    lock_mtx_cdv.lock_busy();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    lock_mtx_cdv.unlock();
}


TEST(LOCK, BusyLockTest_MTX) {
    int trial = 10000000;
    int thread_cnt = 10;
    int result = 0;

    boost::thread_group thread_group;
    for (int i = 0; i < thread_cnt; i++) {
        thread_group.create_thread(boost::bind(job_with_busy_lock_mtx, trial, &result));
    }
    thread_group.join_all();

    int desired = trial * thread_cnt;
    EXPECT_EQ(desired, result);
}


void job_with_try_lock_mtx(int n, int *result) {
    if (lock_mtx_cdv.lock_try()) {
        for (int i = 0; i < n; ++i) {
            (*result)++;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        lock_mtx_cdv.unlock();
    }
}

TEST(LOCK, TryLockTest_MTX) {
    int trial = 1000000;
    int threadCount = 2;

    int result = 0;

    std::thread thr1(job_with_try_lock_mtx, trial, &result);
    std::thread thr2(job_with_try_lock_mtx, trial, &result);

    thr1.join();
    thr2.join();

    int desired = trial * threadCount;
    EXPECT_NE(desired, result);
}


void job_with_wait_lock_mtx(int n, int *result) {
    lock_mtx_cdv.lock_wait();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    lock_mtx_cdv.unlock();
}

TEST(LOCK, WaitLockTest_MTX) {
    int trial = 1000000;
    int thread_cnt = 10;
    int result = 0;

    boost::thread_group thread_group;
    for (int i = 0; i < thread_cnt; i++) {
        thread_group.create_thread(boost::bind(job_with_wait_lock_mtx, trial, &result));
    }
    thread_group.join_all();

    int desired = trial * thread_cnt;
    EXPECT_EQ(desired, result);
}


///////////////////////
/////////BOOST/////////
///////////////////////
void job_with_busy_lock_boost(int n, int *result) {
    lock_bst_cdv.lock_busy();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    lock_bst_cdv.unlock();
}


TEST(LOCK, BusyLockTest_BOOST) {
    int trial = 10000000;
    int thread_cnt = 10;
    int result = 0;

    boost::thread_group thread_group;
    for (int i = 0; i < thread_cnt; i++) {
        thread_group.create_thread(boost::bind(job_with_busy_lock_boost, trial, &result));
    }
    thread_group.join_all();

    int desired = trial * thread_cnt;
    EXPECT_EQ(desired, result);
}

void job_with_try_lock_boost(int n, int *result) {
    if (lock_bst_cdv.lock_try()) {
        for (int i = 0; i < n; ++i) {
            (*result)++;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        lock_bst_cdv.unlock();
    }
}

TEST(LOCK, TryLockTest_BOOST) {
    int trial = 1000000;
    int threadCount = 2;

    int result = 0;

    std::thread thr1(job_with_try_lock_boost, trial, &result);
    std::thread thr2(job_with_try_lock_boost, trial, &result);

    thr1.join();
    thr2.join();

    int desired = trial * threadCount;
    EXPECT_NE(desired, result);
}

void job_with_wait_lock_boost(int n, int *result) {
    lock_bst_cdv.lock_wait();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    lock_bst_cdv.unlock();
}

TEST(LOCK, WaitLockTest_BOOST) {
    int trial = 1000000;
    int thread_cnt = 10;
    int result = 0;

    boost::thread_group thread_group;
    for (int i = 0; i < thread_cnt; i++) {
        thread_group.create_thread(boost::bind(job_with_wait_lock_boost, trial, &result));
    }
    thread_group.join_all();

    int desired = trial * thread_cnt;
    EXPECT_EQ(desired, result);
}

///////////////////////
/////////ATOMIC/////////
///////////////////////
void job_with_busy_lock_atomic(int n, int *result) {
    lock_atm_cdv.lock_busy();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    lock_atm_cdv.unlock();
}


TEST(LOCK, BusyLockTest_ATOMIC) {
    int trial = 10000000;
    int thread_cnt = 10;
    int result = 0;

    boost::thread_group thread_group;
    for (int i = 0; i < thread_cnt; i++) {
        thread_group.create_thread(boost::bind(job_with_busy_lock_atomic, trial, &result));
    }
    thread_group.join_all();


    int desired = trial * thread_cnt;
    EXPECT_EQ(desired, result);
}

void job_with_try_lock_atomic(int n, int *result) {
    if (lock_atm_cdv.lock_try()) {
        for (int i = 0; i < n; ++i) {
            (*result)++;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        lock_atm_cdv.unlock();
    }
}

TEST(LOCK, TryLockTest_ATOMIC) {
    int trial = 1000000;
    int threadCount = 2;

    int result = 0;

    std::thread thr1(job_with_try_lock_atomic, trial, &result);
    std::thread thr2(job_with_try_lock_atomic, trial, &result);

    thr1.join();
    thr2.join();

    int desired = trial * threadCount;
    EXPECT_NE(desired, result);
}

void job_with_wait_lock_atomic(int n, int *result) {
    lock_atm_cdv.lock_wait();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    lock_atm_cdv.unlock();
}

TEST(LOCK, WaitLockTest_ATOMIC) {
    int trial = 1000000;
    int thread_cnt = 10;
    int result = 0;

    boost::thread_group thread_group;
    for (int i = 0; i < thread_cnt; i++) {
        thread_group.create_thread(boost::bind(job_with_wait_lock_atomic, trial, &result));
    }
    thread_group.join_all();

    int desired = trial * thread_cnt;
    EXPECT_EQ(desired, result);
}

//////////////////////
////LOCKS WITH SEM////
//////////////////////
IPL_MTX_SEM lock_mtx_sem;
IPL_BST_SEM lock_bst_sem;
IPL_ATM_SEM lock_atm_sem;

void job_with_wait_lock_mtx_sem(int n, int *result) {
    lock_mtx_sem.lock_wait();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    lock_mtx_sem.unlock();
}

TEST(LOCK, WaitLockTest_MTX_SEM) {
    int trial = 1000000;
    int thread_cnt = 10;
    int result = 0;

    boost::thread_group thread_group;
    for (int i = 0; i < thread_cnt; i++) {
        thread_group.create_thread(boost::bind(job_with_wait_lock_mtx_sem, trial, &result));
    }
    thread_group.join_all();

    int desired = trial * thread_cnt;
    EXPECT_EQ(desired, result);
}

void job_with_wait_lock_bst_sem(int n, int *result) {
    lock_bst_sem.lock_wait();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    lock_bst_sem.unlock();
}

TEST(LOCK, WaitLockTest_BST_SEM) {
    int trial = 1000000;
    int thread_cnt = 10;
    int result = 0;

    boost::thread_group thread_group;
    for (int i = 0; i < thread_cnt; i++) {
        thread_group.create_thread(boost::bind(job_with_wait_lock_bst_sem, trial, &result));
    }
    thread_group.join_all();

    int desired = trial * thread_cnt;
    EXPECT_EQ(desired, result);
}

void job_with_wait_lock_atm_sem(int n, int *result) {
    lock_atm_sem.lock_wait();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    lock_atm_sem.unlock();
}

TEST(LOCK, WaitLockTest_ATM_SEM) {
    int trial = 1000000;
    int thread_cnt = 10;
    int result = 0;

    boost::thread_group thread_group;
    for (int i = 0; i < thread_cnt; i++) {
        thread_group.create_thread(boost::bind(job_with_wait_lock_atm_sem, trial, &result));
    }
    thread_group.join_all();

    int desired = trial * thread_cnt;
    EXPECT_EQ(desired, result);
}

IPL_BST_SEM_TST lock_atm_sem_tst;

void job_with_wait_tst(int n, int *result, ThreadInfo *ti) {

    lock_atm_sem_tst.lock_wait(ti);
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    lock_atm_sem_tst.unlock();
}

TEST(LOCK, TurnsitleTest) {
    int trial = 1000000;
    int thread_cnt = 10;
    int result = 0;
    std::cout << "\n";
    boost::thread_group thread_group;
    for (int i = 0; i < thread_cnt; i++) {
        ThreadInfo *ti = new ThreadInfo();
        thread_group.create_thread(boost::bind(job_with_wait_tst, trial, &result, ti));
    }
    thread_group.join_all();


    int desired = trial * thread_cnt;
    EXPECT_EQ(desired, result);
}


//class
TEST(LOCK, InterThreadLockableTest) {

}


