#include <iostream>
#include "gtest/gtest.h"

#include <thread>
#include "../CommunicationProtocol/DevKit/DevKit.h"
#include "IntraProcessLockable/IntraProcessLockable.h"
#include "IntraProcessLockable/IPL-ATOMIC-CDV.h"
#include "IntraProcessLockable/IPL-ATOMIC-SEM.h"
#include "IntraProcessLockable/IPL-BOOST-CDV.h"
#include "IntraProcessLockable/IPL-BOOST-SEM.h"
#include "IntraProcessLockable/IPL-MTX-CDV.h"
#include "IntraProcessLockable/IPL-MTX-SEM.h"

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

//////////////////////
////LOCKS WITH CDV////
//////////////////////
IPL_MTX_CDV lock_mtx_cdv;
IPL_BST_CDV lock_bst_cdv;
IPL_ATM_CDV lock_atm_cdv;


////////////////
////WITH MTX////
////////////////
void job_with_busy_lock_mtx(int n, int* result) {
    lock_mtx_cdv.lock_busy();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    lock_mtx_cdv.unlock();
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


void job_with_wait_lock_mtx(int n, int* result) {
    lock_mtx_cdv.lock_wait();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    lock_mtx_cdv.unlock();
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
    lock_bst_cdv.lock_busy();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    lock_bst_cdv.unlock();
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

void job_with_wait_lock_boost(int n, int* result) {
    lock_bst_cdv.lock_wait();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    lock_bst_cdv.unlock();
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

///////////////////////
/////////ATOMIC/////////
///////////////////////
void job_with_busy_lock_atomic(int n, int* result) {
    lock_atm_cdv.lock_busy();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    lock_atm_cdv.unlock();
}


TEST(LOCK, BusyLockTest_ATOMIC) {
    int trial = 10000000;
    int threadCount = 10;

    int result = 0;

    std::thread thr1(job_with_busy_lock_atomic, trial, &result);
    std::thread thr2(job_with_busy_lock_atomic, trial, &result);
    std::thread thr3(job_with_busy_lock_atomic, trial, &result);
    std::thread thr4(job_with_busy_lock_atomic, trial, &result);
    std::thread thr5(job_with_busy_lock_atomic, trial, &result);
    std::thread thr6(job_with_busy_lock_atomic, trial, &result);
    std::thread thr7(job_with_busy_lock_atomic, trial, &result);
    std::thread thr8(job_with_busy_lock_atomic, trial, &result);
    std::thread thr9(job_with_busy_lock_atomic, trial, &result);
    std::thread thr10(job_with_busy_lock_atomic, trial, &result);

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

void job_with_try_lock_atomic(int n, int* result) {
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

void job_with_wait_lock_atomic(int n, int* result) {
    lock_atm_cdv.lock_wait();
    for (int i = 0; i < n; ++i) {
        (*result)++;
    }
    lock_atm_cdv.unlock();
}

TEST(LOCK, WaitLockTest_ATOMIC) {
    int trial = 1000000;
    int threadCount = 10;

    int result = 0;

    std::thread thr1(job_with_wait_lock_atomic, trial, &result);
    std::thread thr2(job_with_wait_lock_atomic, trial, &result);
    std::thread thr3(job_with_wait_lock_atomic, trial, &result);
    std::thread thr4(job_with_wait_lock_atomic, trial, &result);
    std::thread thr5(job_with_wait_lock_atomic, trial, &result);
    std::thread thr6(job_with_wait_lock_atomic, trial, &result);
    std::thread thr7(job_with_wait_lock_atomic, trial, &result);
    std::thread thr8(job_with_wait_lock_atomic, trial, &result);
    std::thread thr9(job_with_wait_lock_atomic, trial, &result);
    std::thread thr10(job_with_wait_lock_atomic, trial, &result);

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


///////////////////////////
/////////////////////////
//////////////////////
////PERFORMANCE TEST
//////////////////////
/////////////////////////
///////////////////////////


///////////
////CDV////
///////////
void acq_release_cdv_busy_MTX(){
    lock_mtx_cdv.lock_busy();
    lock_mtx_cdv.unlock();
}

void acq_release_cdv_busy_BOOST(){
    lock_bst_cdv.lock_busy();
    lock_bst_cdv.unlock();
}

void acq_release_cdv_busy_ATOMIC(){
    lock_atm_cdv.lock_busy();
    lock_atm_cdv.unlock();
}

TEST(LOCK, PERFORMANCE_CDV_BUSY_MTX_BST_ATM) {
    POL("\n\nBUSY LOCK PERFORMANCE TEST\n\n");
    POL("\nMTX");
    START();
    std::thread thr1(acq_release_cdv_busy_MTX);
    std::thread thr2(acq_release_cdv_busy_MTX);
    std::thread thr3(acq_release_cdv_busy_MTX);
    std::thread thr4(acq_release_cdv_busy_MTX);
    std::thread thr5(acq_release_cdv_busy_MTX);
    std::thread thr6(acq_release_cdv_busy_MTX);
    std::thread thr7(acq_release_cdv_busy_MTX);
    std::thread thr8(acq_release_cdv_busy_MTX);
    std::thread thr9(acq_release_cdv_busy_MTX);
    std::thread thr10(acq_release_cdv_busy_MTX);

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

    POL("\nBOOST");
    START();
    std::thread thr11(acq_release_cdv_busy_BOOST);
    std::thread thr12(acq_release_cdv_busy_BOOST);
    std::thread thr13(acq_release_cdv_busy_BOOST);
    std::thread thr14(acq_release_cdv_busy_BOOST);
    std::thread thr15(acq_release_cdv_busy_BOOST);
    std::thread thr16(acq_release_cdv_busy_BOOST);
    std::thread thr17(acq_release_cdv_busy_BOOST);
    std::thread thr18(acq_release_cdv_busy_BOOST);
    std::thread thr19(acq_release_cdv_busy_BOOST);
    std::thread thr20(acq_release_cdv_busy_BOOST);

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

    POL("\n\nATOMIC");
    START();
    std::thread thr21(acq_release_cdv_busy_ATOMIC);
    std::thread thr22(acq_release_cdv_busy_ATOMIC);
    std::thread thr23(acq_release_cdv_busy_ATOMIC);
    std::thread thr24(acq_release_cdv_busy_ATOMIC);
    std::thread thr25(acq_release_cdv_busy_ATOMIC);
    std::thread thr26(acq_release_cdv_busy_ATOMIC);
    std::thread thr27(acq_release_cdv_busy_ATOMIC);
    std::thread thr28(acq_release_cdv_busy_ATOMIC);
    std::thread thr29(acq_release_cdv_busy_ATOMIC);
    std::thread thr30(acq_release_cdv_busy_ATOMIC);

    thr21.join();
    thr22.join();
    thr23.join();
    thr24.join();
    thr25.join();
    thr26.join();
    thr27.join();
    thr28.join();
    thr29.join();
    thr30.join();
    END();
}

void acq_release_cdv_wait_MTX(){
    lock_mtx_cdv.lock_wait();
    lock_mtx_cdv.unlock();
}

void acq_release_cdv_wait_BOOST(){
    lock_bst_cdv.lock_wait();
    lock_bst_cdv.unlock();
}

void acq_release_cdv_wait_ATOMIC(){
    lock_atm_cdv.lock_wait();
    lock_atm_cdv.unlock();
}

TEST(LOCK, PERFORMANCE_CDV_WAIT_MTX_BST_ATM) {
    POL("\n\nWAIT LOCK PERFORMANCE TEST\n\n");

    POL("\nMTX");
    START();
    std::thread thr1(acq_release_cdv_wait_MTX);
    std::thread thr2(acq_release_cdv_wait_MTX);
    std::thread thr3(acq_release_cdv_wait_MTX);
    std::thread thr4(acq_release_cdv_wait_MTX);
    std::thread thr5(acq_release_cdv_wait_MTX);
    std::thread thr6(acq_release_cdv_wait_MTX);
    std::thread thr7(acq_release_cdv_wait_MTX);
    std::thread thr8(acq_release_cdv_wait_MTX);
    std::thread thr9(acq_release_cdv_wait_MTX);
    std::thread thr10(acq_release_cdv_wait_MTX);

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

    POL("\nBOOST");
    START();
    std::thread thr11(acq_release_cdv_wait_BOOST);
    std::thread thr12(acq_release_cdv_wait_BOOST);
    std::thread thr13(acq_release_cdv_wait_BOOST);
    std::thread thr14(acq_release_cdv_wait_BOOST);
    std::thread thr15(acq_release_cdv_wait_BOOST);
    std::thread thr16(acq_release_cdv_wait_BOOST);
    std::thread thr17(acq_release_cdv_wait_BOOST);
    std::thread thr18(acq_release_cdv_wait_BOOST);
    std::thread thr19(acq_release_cdv_wait_BOOST);
    std::thread thr20(acq_release_cdv_wait_BOOST);

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

    POL("\nATOMIC");
    START();
    std::thread thr21(acq_release_cdv_wait_ATOMIC);
    std::thread thr22(acq_release_cdv_wait_ATOMIC);
    std::thread thr23(acq_release_cdv_wait_ATOMIC);
    std::thread thr24(acq_release_cdv_wait_ATOMIC);
    std::thread thr25(acq_release_cdv_wait_ATOMIC);
    std::thread thr26(acq_release_cdv_wait_ATOMIC);
    std::thread thr27(acq_release_cdv_wait_ATOMIC);
    std::thread thr28(acq_release_cdv_wait_ATOMIC);
    std::thread thr29(acq_release_cdv_wait_ATOMIC);
    std::thread thr30(acq_release_cdv_wait_ATOMIC);

    thr21.join();
    thr22.join();
    thr23.join();
    thr24.join();
    thr25.join();
    thr26.join();
    thr27.join();
    thr28.join();
    thr29.join();
    thr30.join();
    END();
}

///////////
////SEM////
///////////
void acq_release_sem_wait_MTX(){
    lock_mtx_cdv.lock_wait();
    lock_mtx_cdv.unlock();
}

void acq_release_sem_wait_BOOST(){
    lock_bst_cdv.lock_wait();
    lock_bst_cdv.unlock();
}

void acq_release_sem_wait_ATOMIC(){
    lock_atm_cdv.lock_wait();
    lock_atm_cdv.unlock();
}

TEST(LOCK, PERFORMANCE_SEM_WAIT_MTX_BST_ATM) {
    POL("\n\nWAIT LOCK PERFORMANCE TEST\n\n");

    POL("\nMTX");
    START();
    std::thread thr1(acq_release_sem_wait_MTX);
    std::thread thr2(acq_release_sem_wait_MTX);
    std::thread thr3(acq_release_sem_wait_MTX);
    std::thread thr4(acq_release_sem_wait_MTX);
    std::thread thr5(acq_release_sem_wait_MTX);
    std::thread thr6(acq_release_sem_wait_MTX);
    std::thread thr7(acq_release_sem_wait_MTX);
    std::thread thr8(acq_release_sem_wait_MTX);
    std::thread thr9(acq_release_sem_wait_MTX);
    std::thread thr10(acq_release_sem_wait_MTX);

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

    POL("\nBOOST");
    START();
    std::thread thr11(acq_release_sem_wait_BOOST);
    std::thread thr12(acq_release_sem_wait_BOOST);
    std::thread thr13(acq_release_sem_wait_BOOST);
    std::thread thr14(acq_release_sem_wait_BOOST);
    std::thread thr15(acq_release_sem_wait_BOOST);
    std::thread thr16(acq_release_sem_wait_BOOST);
    std::thread thr17(acq_release_sem_wait_BOOST);
    std::thread thr18(acq_release_sem_wait_BOOST);
    std::thread thr19(acq_release_sem_wait_BOOST);
    std::thread thr20(acq_release_sem_wait_BOOST);

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

    POL("\nATOMIC");
    START();
    std::thread thr21(acq_release_sem_wait_ATOMIC);
    std::thread thr22(acq_release_sem_wait_ATOMIC);
    std::thread thr23(acq_release_sem_wait_ATOMIC);
    std::thread thr24(acq_release_sem_wait_ATOMIC);
    std::thread thr25(acq_release_sem_wait_ATOMIC);
    std::thread thr26(acq_release_sem_wait_ATOMIC);
    std::thread thr27(acq_release_sem_wait_ATOMIC);
    std::thread thr28(acq_release_sem_wait_ATOMIC);
    std::thread thr29(acq_release_sem_wait_ATOMIC);
    std::thread thr30(acq_release_sem_wait_ATOMIC);

    thr21.join();
    thr22.join();
    thr23.join();
    thr24.join();
    thr25.join();
    thr26.join();
    thr27.join();
    thr28.join();
    thr29.join();
    thr30.join();
    END();
}
