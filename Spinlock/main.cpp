#include <iostream>
#include "gtest/gtest.h"

#include <thread>
#include "../CommunicationProtocol/DevKit/DevKit.h"
#include "Spinlock/IntraProcessLockable.h"


void job_no_lock(int n, int* result) {
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
}

TEST(LOCK, noLockTest) {
    int trial = 1000000;
    int threadCount = 10;

    int result = 0;

    std::thread thr1(job_no_lock, trial, &result);
    std::thread thr2(job_no_lock, trial, &result);
    std::thread thr3(job_no_lock, trial, &result);
    std::thread thr4(job_no_lock, trial, &result);
    std::thread thr5(job_no_lock, trial, &result);
    std::thread thr6(job_no_lock, trial, &result);
    std::thread thr7(job_no_lock, trial, &result);
    std::thread thr8(job_no_lock, trial, &result);
    std::thread thr9(job_no_lock, trial, &result);
    std::thread thr10(job_no_lock, trial, &result);

    thr1.join();
    thr2.join();
    thr3.join();
    thr4.join();
    thr5.join();
    thr6.join();
    thr7.join();
    thr8.join();
    thr9.join();
    thr10.join();

    int desired = trial * threadCount;
    EXPECT_NE(desired, result);
}

/////////////////////
////WITH SPINLOCK////
/////////////////////
IntraProcessLockable spinlock_busy(1, 1, 1);
IntraProcessLockable spinlock_try(1, 1, 1);
IntraProcessLockable spinlock_wait(1, 1, 1);

////////////////
////WITH MTX////
////////////////
void job_with_busy_lock_mtx(int n, int* result) {
    spinlock_busy.lock_busy_mtx();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    spinlock_busy.unlock_mtx();
}


TEST(LOCK, BusyLockTest_MTX) {
    int trial = 10000000;
    int threadCount = 10;

    int result = 0;

    std::thread thr1(job_with_busy_lock_mtx, trial, &result);
    std::thread thr2(job_with_busy_lock_mtx, trial, &result);
    std::thread thr3(job_with_busy_lock_mtx, trial, &result);
    std::thread thr4(job_with_busy_lock_mtx, trial, &result);
    std::thread thr5(job_with_busy_lock_mtx, trial, &result);
    std::thread thr6(job_with_busy_lock_mtx, trial, &result);
    std::thread thr7(job_with_busy_lock_mtx, trial, &result);
    std::thread thr8(job_with_busy_lock_mtx, trial, &result);
    std::thread thr9(job_with_busy_lock_mtx, trial, &result);
    std::thread thr10(job_with_busy_lock_mtx, trial, &result);

    thr1.join();
    thr2.join();
    thr3.join();
    thr4.join();
    thr5.join();
    thr6.join();
    thr7.join();
    thr8.join();
    thr9.join();
    thr10.join();


    int desired = trial * threadCount;
    EXPECT_EQ(desired, result);
}


void job_with_try_lock_mtx(int n, int* result) {
    if (spinlock_try.lock_try_mtx()) {
        for (int i = 0; i < n; ++i) {
            (*result)++;
        }
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        spinlock_try.unlock_mtx();
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


void job_with_wait_lock_mtx(int n, int* result) {
    spinlock_wait.lock_wait_mtx();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    spinlock_wait.unlock_mtx();
}

TEST(LOCK, WaitLockTest_MTX) {
    int trial = 1000000;
    int threadCount = 10;

    int result = 0;

    std::thread thr1(job_with_wait_lock_mtx, trial, &result);
    std::thread thr2(job_with_wait_lock_mtx, trial, &result);
    std::thread thr3(job_with_wait_lock_mtx, trial, &result);
    std::thread thr4(job_with_wait_lock_mtx, trial, &result);
    std::thread thr5(job_with_wait_lock_mtx, trial, &result);
    std::thread thr6(job_with_wait_lock_mtx, trial, &result);
    std::thread thr7(job_with_wait_lock_mtx, trial, &result);
    std::thread thr8(job_with_wait_lock_mtx, trial, &result);
    std::thread thr9(job_with_wait_lock_mtx, trial, &result);
    std::thread thr10(job_with_wait_lock_mtx, trial, &result);

    thr1.join();
    thr2.join();
    thr3.join();
    thr4.join();
    thr5.join();
    thr6.join();
    thr7.join();
    thr8.join();
    thr9.join();
    thr10.join();

    int desired = trial * threadCount;
    EXPECT_EQ(desired, result);
}



///////////////////////
/////////BOOST/////////
///////////////////////
void job_with_busy_lock_boost(int n, int* result) {
    spinlock_busy.lock_busy_boost();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    spinlock_busy.unlock_boost();
}


TEST(LOCK, BusyLockTest_BOOST) {
    int trial = 10000000;
    int threadCount = 10;

    int result = 0;

    std::thread thr1(job_with_busy_lock_boost, trial, &result);
    std::thread thr2(job_with_busy_lock_boost, trial, &result);
    std::thread thr3(job_with_busy_lock_boost, trial, &result);
    std::thread thr4(job_with_busy_lock_boost, trial, &result);
    std::thread thr5(job_with_busy_lock_boost, trial, &result);
    std::thread thr6(job_with_busy_lock_boost, trial, &result);
    std::thread thr7(job_with_busy_lock_boost, trial, &result);
    std::thread thr8(job_with_busy_lock_boost, trial, &result);
    std::thread thr9(job_with_busy_lock_boost, trial, &result);
    std::thread thr10(job_with_busy_lock_boost, trial, &result);

    thr1.join();
    thr2.join();
    thr3.join();
    thr4.join();
    thr5.join();
    thr6.join();
    thr7.join();
    thr8.join();
    thr9.join();
    thr10.join();


    int desired = trial * threadCount;
    EXPECT_EQ(desired, result);
}

void job_with_try_lock_boost(int n, int* result) {
    if (spinlock_try.lock_try_boost()) {
        for (int i = 0; i < n; ++i) {
            (*result)++;
        }
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        spinlock_try.unlock_boost();
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

void job_with_wait_lock_boost(int n, int* result) {
    spinlock_wait.lock_wait_boost();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    spinlock_wait.unlock_boost();
}

TEST(LOCK, WaitLockTest_BOOST) {
    int trial = 1000000;
    int threadCount = 10;

    int result = 0;

    std::thread thr1(job_with_wait_lock_boost, trial, &result);
    std::thread thr2(job_with_wait_lock_boost, trial, &result);
    std::thread thr3(job_with_wait_lock_boost, trial, &result);
    std::thread thr4(job_with_wait_lock_boost, trial, &result);
    std::thread thr5(job_with_wait_lock_boost, trial, &result);
    std::thread thr6(job_with_wait_lock_boost, trial, &result);
    std::thread thr7(job_with_wait_lock_boost, trial, &result);
    std::thread thr8(job_with_wait_lock_boost, trial, &result);
    std::thread thr9(job_with_wait_lock_boost, trial, &result);
    std::thread thr10(job_with_wait_lock_boost, trial, &result);

    thr1.join();
    thr2.join();
    thr3.join();
    thr4.join();
    thr5.join();
    thr6.join();
    thr7.join();
    thr8.join();
    thr9.join();
    thr10.join();

    int desired = trial * threadCount;
    EXPECT_EQ(desired, result);
}

void acq_release_busy_MTX(){
    spinlock_busy.lock_busy_mtx();
    spinlock_busy.unlock_mtx();
}

void acq_release_busy_BOOST(){
    spinlock_busy.lock_busy_boost();
    spinlock_busy.unlock_boost();
}

TEST(LOCK, MTX_vs_BOOST_BUSY) {
    POL("\n\nMTX");
    START();
    std::thread thr1(acq_release_busy_MTX);
    std::thread thr2(acq_release_busy_MTX);
    std::thread thr3(acq_release_busy_MTX);
    std::thread thr4(acq_release_busy_MTX);
    std::thread thr5(acq_release_busy_MTX);
    std::thread thr6(acq_release_busy_MTX);
    std::thread thr7(acq_release_busy_MTX);
    std::thread thr8(acq_release_busy_MTX);
    std::thread thr9(acq_release_busy_MTX);
    std::thread thr10(acq_release_busy_MTX);

    thr1.join();
    thr2.join();
    thr3.join();
    thr4.join();
    thr5.join();
    thr6.join();
    thr7.join();
    thr8.join();
    thr9.join();
    thr10.join();
    END();

    POL("\n\nBOOST");
    START();
    std::thread thr11(acq_release_busy_BOOST);
    std::thread thr12(acq_release_busy_BOOST);
    std::thread thr13(acq_release_busy_BOOST);
    std::thread thr14(acq_release_busy_BOOST);
    std::thread thr15(acq_release_busy_BOOST);
    std::thread thr16(acq_release_busy_BOOST);
    std::thread thr17(acq_release_busy_BOOST);
    std::thread thr18(acq_release_busy_BOOST);
    std::thread thr19(acq_release_busy_BOOST);
    std::thread thr20(acq_release_busy_BOOST);

    thr11.join();
    thr12.join();
    thr13.join();
    thr14.join();
    thr15.join();
    thr16.join();
    thr17.join();
    thr18.join();
    thr19.join();
    thr20.join();
    END();
}

void acq_release_wait_MTX(){
    spinlock_busy.lock_wait_mtx();
    spinlock_busy.unlock_mtx();
}

void acq_release_wait_BOOST(){
    spinlock_busy.lock_wait_boost();
    spinlock_busy.unlock_boost();
}

TEST(LOCK, MTX_vs_BOOST_WAIT) {
    POL("\n\nMTX");
    START();
    std::thread thr1(acq_release_wait_MTX);
    std::thread thr2(acq_release_wait_MTX);
    std::thread thr3(acq_release_wait_MTX);
    std::thread thr4(acq_release_wait_MTX);
    std::thread thr5(acq_release_wait_MTX);
    std::thread thr6(acq_release_wait_MTX);
    std::thread thr7(acq_release_wait_MTX);
    std::thread thr8(acq_release_wait_MTX);
    std::thread thr9(acq_release_wait_MTX);
    std::thread thr10(acq_release_wait_MTX);

    thr1.join();
    thr2.join();
    thr3.join();
    thr4.join();
    thr5.join();
    thr6.join();
    thr7.join();
    thr8.join();
    thr9.join();
    thr10.join();
    END();

    POL("\n\nBOOST");
    START();
    std::thread thr11(acq_release_wait_BOOST);
    std::thread thr12(acq_release_wait_BOOST);
    std::thread thr13(acq_release_wait_BOOST);
    std::thread thr14(acq_release_wait_BOOST);
    std::thread thr15(acq_release_wait_BOOST);
    std::thread thr16(acq_release_wait_BOOST);
    std::thread thr17(acq_release_wait_BOOST);
    std::thread thr18(acq_release_wait_BOOST);
    std::thread thr19(acq_release_wait_BOOST);
    std::thread thr20(acq_release_wait_BOOST);

    thr11.join();
    thr12.join();
    thr13.join();
    thr14.join();
    thr15.join();
    thr16.join();
    thr17.join();
    thr18.join();
    thr19.join();
    thr20.join();
    END();
}
