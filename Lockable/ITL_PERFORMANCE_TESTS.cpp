//
// Created by hspark on 3/29/18.
//

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


///////////////////////////
/////////////////////////
//////////////////////
////PERFORMANCE TEST
//////////////////////
/////////////////////////
///////////////////////////


//////////////////////
////LOCKS WITH SEM////
//////////////////////
IPL_MTX_SEM lock_mtx_sem2;
IPL_BST_SEM lock_bst_sem2;
IPL_ATM_SEM lock_atm_sem2;

//////////////////////
////LOCKS WITH CDV////
//////////////////////
IPL_MTX_CDV lock_mtx_cdv2;
IPL_BST_CDV lock_bst_cdv2;
IPL_ATM_CDV lock_atm_cdv2;

//////////////////////
////LOCKS WITH TST////
//////////////////////
IPL_BST_SEM_TST lock_atm_sm_tst2;


void acq_release_cdv_try_MTX() {
    lock_mtx_cdv2.lock_try();
    lock_mtx_cdv2.unlock();
}

void acq_release_cdv_try_BOOST() {
    lock_bst_cdv2.lock_try();
    lock_bst_cdv2.unlock();
}

void acq_release_cdv_try_ATOMIC() {
    lock_atm_cdv2.lock_try();
    lock_atm_cdv2.unlock();
}

void acq_release_cdv_busy_MTX() {
    lock_mtx_cdv2.lock_busy();
    lock_mtx_cdv2.unlock();
}

void acq_release_cdv_busy_BOOST() {
    lock_bst_cdv2.lock_busy();
    lock_bst_cdv2.unlock();
}

void acq_release_cdv_busy_ATOMIC() {
    lock_atm_cdv2.lock_busy();
    lock_atm_cdv2.unlock();
}

void acq_release_cdv_wait_MTX() {
    lock_mtx_cdv2.lock_wait();
    lock_mtx_cdv2.unlock();
}

void acq_release_cdv_wait_BOOST() {
    lock_bst_cdv2.lock_wait();
    lock_bst_cdv2.unlock();
}

void acq_release_cdv_wait_ATOMIC() {
    lock_atm_cdv2.lock_wait();
    lock_atm_cdv2.unlock();
}

///////////
////SEM////
///////////
void acq_release_sem_wait_MTX() {
    lock_mtx_sem2.lock_wait();
    lock_mtx_sem2.unlock();
}

void acq_release_sem_wait_BOOST() {
    lock_bst_sem2.lock_wait();
    lock_bst_sem2.unlock();
}

void acq_release_sem_wait_ATOMIC() {
    lock_atm_sem2.lock_wait();
    lock_atm_sem2.unlock();
}

//////////////////////////////////TRY

TEST(LOCK, PERFORMANCE_TRY_MTX) {
    boost::thread_group thread_group;

    int thread_cnt = 1;
    int test_trial = 1000;

    REDIRECT("TRY_MTX");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nMTX");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_cdv_try_MTX);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

TEST(LOCK, PERFORMANCE_TRY_BST) {
    boost::thread_group thread_group;

    int thread_cnt = 1;
    int test_trial = 1000;

    REDIRECT("TRY_BST");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nBOOST");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_cdv_try_BOOST);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

TEST(LOCK, PERFORMANCE_TRY_ATM) {
    boost::thread_group thread_group;

    int thread_cnt = 1;
    int test_trial = 1000;

    REDIRECT("TRY_ATM");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nATOMIC");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_cdv_try_ATOMIC);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

//////////////////////////////////BUSY_HIGH

TEST(LOCK, PERFORMANCE_BUSY_HIGH_MTX) {
    boost::thread_group thread_group;

    int thread_cnt = 100;
    int test_trial = 1000;

    REDIRECT("BUSY_HIGH_MTX");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nMTX");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_cdv_busy_MTX);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

TEST(LOCK, PERFORMANCE_BUSY_HIGH_BST) {
    boost::thread_group thread_group;

    int thread_cnt = 100;
    int test_trial = 1000;

    REDIRECT("BUSY_HIGH_BST");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nBOOST");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_cdv_busy_BOOST);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

TEST(LOCK, PERFORMANCE_BUSY_HIGH_ATM) {
    boost::thread_group thread_group;

    int thread_cnt = 100;
    int test_trial = 1000;

    REDIRECT("BUSY_HIGH_ATM");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nATOMIC");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_cdv_busy_ATOMIC);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

//////////////////////////////////BUSY_LOW

TEST(LOCK, PERFORMANCE_BUSY_LOW_MTX) {
    boost::thread_group thread_group;

    int thread_cnt = 3;
    int test_trial = 1000;

    REDIRECT("BUSY_LOW_MTX");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nMTX");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_cdv_busy_MTX);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

TEST(LOCK, PERFORMANCE_BUSY_LOW_BST) {
    boost::thread_group thread_group;

    int thread_cnt = 3;
    int test_trial = 1000;

    REDIRECT("BUSY_LOW_BST");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nBOOST");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_cdv_busy_BOOST);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

TEST(LOCK, PERFORMANCE_BUSY_LOW_ATM) {
    boost::thread_group thread_group;

    int thread_cnt = 3;
    int test_trial = 1000;

    REDIRECT("BUSY_LOW_ATM");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nATOMIC");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_cdv_busy_ATOMIC);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

//////////////////////////////////WAIT_HIGH_CDV

TEST(LOCK, PERFORMANCE_WAIT_CDV_HIGH_MTX) {
    boost::thread_group thread_group;

    int thread_cnt = 100;
    int test_trial = 1000;

    REDIRECT("WAIT_CDV_HIGH_MTX");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nMTX");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_cdv_wait_MTX);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

TEST(LOCK, PERFORMANCE_WAIT_CDV_HIGH_BST) {
    boost::thread_group thread_group;

    int thread_cnt = 100;
    int test_trial = 1000;

    REDIRECT("WAIT_CDV_HIGH_BST");
    for (int i = 1; i <= test_trial; i++) {

        POL("\nBOOST");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_cdv_wait_BOOST);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

TEST(LOCK, PERFORMANCE_WAIT_CDV_HIGH_ATM) {
    boost::thread_group thread_group;

    int thread_cnt = 100;
    int test_trial = 1000;

    REDIRECT("WAIT_CDV_HIGH_ATM");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nATOMIC");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_cdv_wait_ATOMIC);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

//////////////////////////////////WAIT_HIGH_SEM

TEST(LOCK, PERFORMANCE_WAIT_SEM_HIGH_MTX) {
    boost::thread_group thread_group;

    int thread_cnt = 100;
    int test_trial = 1000;

    REDIRECT("WAIT_SEM_HIGH_MTX");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nMTX");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_sem_wait_MTX);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

TEST(LOCK, PERFORMANCE_WAIT_SEM_HIGH_BST) {
    boost::thread_group thread_group;

    int thread_cnt = 100;
    int test_trial = 1000;

    REDIRECT("WAIT_SEM_HIGH_BST");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nBOOST");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_sem_wait_BOOST);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

TEST(LOCK, PERFORMANCE_WAIT_SEM_HIGH_ATM) {
    boost::thread_group thread_group;

    int thread_cnt = 100;
    int test_trial = 1000;

    REDIRECT("WAIT_SEM_HIGH_ATM");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nATOMIC");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_sem_wait_ATOMIC);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

//////////////////////////////////WAIT_LOW_CDV

TEST(LOCK, PERFORMANCE_WAIT_CDV_LOW_MTX) {
    boost::thread_group thread_group;

    int thread_cnt = 3;
    int test_trial = 1000;

    REDIRECT("WAIT_CDV_LOW_MTX");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nMTX");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_cdv_wait_MTX);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

TEST(LOCK, PERFORMANCE_WAIT_CDV_LOW_BST) {
    boost::thread_group thread_group;

    int thread_cnt = 3;
    int test_trial = 1000;

    REDIRECT("WAIT_CDV_LOW_BST");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nBOOST");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_cdv_wait_BOOST);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

TEST(LOCK, PERFORMANCE_WAIT_CDV_LOW_ATM) {
    boost::thread_group thread_group;

    int thread_cnt = 3;
    int test_trial = 1000;

    REDIRECT("WAIT_CDV_LOW_ATM");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nATOMIC");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_cdv_wait_ATOMIC);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

//////////////////////////////////WAIT_LOW_SEM

TEST(LOCK, PERFORMANCE_WAIT_SEM_LOW_MTX) {
    boost::thread_group thread_group;

    int thread_cnt = 3;
    int test_trial = 1000;

    REDIRECT("WAIT_SEM_LOW_MTX");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nMTX");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_sem_wait_MTX);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

TEST(LOCK, PERFORMANCE_WAIT_SEM_LOW_BST) {
    boost::thread_group thread_group;

    int thread_cnt = 3;
    int test_trial = 1000;

    REDIRECT("WAIT_SEM_LOW_BST");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nBOOST");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_sem_wait_BOOST);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

TEST(LOCK, PERFORMANCE_WAIT_SEM_LOW_ATM) {
    boost::thread_group thread_group;

    int thread_cnt = 3;
    int test_trial = 1000;

    REDIRECT("WAIT_SEM_LOW_ATM");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>BUSY LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nATOMIC");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            thread_group.create_thread(acq_release_sem_wait_ATOMIC);
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<BUSY LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}


void acq_release_wait_ASSEMBLY_TST(ThreadInfo* ti) {
    lock_atm_sm_tst2.lock_wait(ti);
    lock_atm_sm_tst2.unlock();
}

TEST(LOCK, PERFORMANCE_TEST_SEM_LOW_BST_TST) {
    boost::thread_group thread_group;

    int thread_cnt = 3;
    int test_trial = 1000;

    REDIRECT("WAIT_SEM_LOW_BST_TST");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>WAIT LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nTST");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            ThreadInfo *ti = new ThreadInfo();
            thread_group.create_thread(boost::bind(acq_release_wait_ASSEMBLY_TST, ti));
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<WAIT LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}

TEST(LOCK, PERFORMANCE_TEST_SEM_HIGH_BST_TST) {
    boost::thread_group thread_group;

    int thread_cnt = 100;
    int test_trial = 1000;

    REDIRECT("WAIT_SEM_LOW_HIGH_TST");
    for (int i = 1; i <= test_trial; i++) {

        POL("\n>>>>>>>>>>>>>>>>>>>>>>>WAIT LOCK PERFORMANCE TEST START: TRIAL # ", i);

        POL("\nTST");
        START();
        for (int i = 0; i < thread_cnt; i++) {
            ThreadInfo *ti = new ThreadInfo();
            thread_group.create_thread(boost::bind(acq_release_wait_ASSEMBLY_TST, ti));
        }
        END();

        thread_group.join_all();

        POL("\n<<<<<<<<<<<<<<<<<<<<<<<WAIT LOCK PERFORMANCE DONE: TRIAL # ", i);
    }
    REDIRECT_RESET();
}