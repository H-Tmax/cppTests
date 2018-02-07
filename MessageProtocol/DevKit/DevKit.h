#ifndef MESSAGEPROTOCOL_TIMER_H
#define MESSAGEPROTOCOL_TIMER_H

/**
 * @file DevKit.h
 * @author hspark
 * @date 2018-FEB-07
 * @version 1.0
 * @brief Macro library for development phase
 *
 * THIS HEADER FILE CONTAINS A KIT OF TOOLS FOR DEVELOPERS (I HOPE)
 */
#include <chrono>
#include <iostream>

/**
 * PRINT OUT MACROS
 * I know they are ugly, but they work.
 *
 * USE: PO(things to print out, up to 20 arguments)
 * USE: POL(things to print out, up to 20 arguments)
 */

// Overloading the macro by the number of arguments
#define WHICH_PO(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,ARGS,...) ARGS
#define PO(...) WHICH_PO(__VA_ARGS__, PO20, PO19, PO18, PO17, PO16, PO15, PO14, PO13, PO12, PO11, PO10, PO9, PO8, PO7, PO6, PO5, PO4, PO3, PO2, PO1)(__VA_ARGS__)

#define WHICH_POL(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,ARGS,...) ARGS
#define POL(...) WHICH_POL(__VA_ARGS__, POL20, POL19, POL18, POL17, POL16, POL15, POL14, POL13, POL12, POL11, POL10, POL9, POL8, POL7, POL6, POL5, POL4, POL3, POL2, POL1)(__VA_ARGS__)

// ACTUAL PRINT OUT MACROS
#define PO1(in){ std::cout << in; } void foo()
#define PO2(in1, in2){ std::cout << in1 << in2; } void foo()
#define PO3(in1, in2, in3){ std::cout << in1 << in2 << in3; } void foo()
#define PO4(in1, in2, in3, in4){ std::cout << in1 << in2 << in3 << in4; } void foo()
#define PO5(in1, in2, in3, in4, in5){ std::cout << in1 << in2 << in3 << in4 << in5; } void foo()
#define PO6(in1, in2, in3, in4, in5, in6){ std::cout << in1 << in2 << in3 << in4 << in5 <<in6; } void foo()
#define PO7(in1, in2, in3, in4, in5, in6, in7){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7; } void foo()
#define PO8(in1, in2, in3, in4, in5, in6, in7, in8){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8; } void foo()
#define PO9(in1, in2, in3, in4, in5, in6, in7, in8, in9){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9; } void foo()
#define PO10(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10; } void foo()
#define PO11(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11; } void foo()
#define PO12(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << in12; } void foo()
#define PO13(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12, in13){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << in12 << in13; } void foo()
#define PO14(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12, in13, in14){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << in12 << in13 << in14; } void foo()
#define PO15(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12, in13, in14, in15){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << in12 << in13 << in14 << in15; } void foo()
#define PO16(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12, in13, in14, in15, in16){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << in12 << in13 << in14 << in15 << in16; } void foo()
#define PO17(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12, in13, in14, in15, in16, in17){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << in12 << in13 << in14 << in15 << in16 << in17; } void foo()
#define PO18(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12, in13, in14, in15, in16, in17, in18){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << in12 << in13 << in14 << in15 << in16 << in17 << in18; } void foo()
#define PO19(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12, in13, in14, in15, in16, in17, in18, in19){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << in12 << in13 << in14 << in15 << in16 << in17 << in18 << in19; } void foo()
#define PO20(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12, in13, in14, in15, in16, in17, in18, in19, in20){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << in12 << in13 << in14 << in15 << in16 << in17 << in18 << in19 << in20; } void foo()

// ACTUAL PRINT OUT LINE MACROS
#define POL1(in){ std::cout << in << std::endl; } void foo()
#define POL2(in1, in2){ std::cout << in1 << in2 << std::endl; } void foo()
#define POL3(in1, in2, in3){ std::cout << in1 << in2 << in3 << std::endl; } void foo()
#define POL4(in1, in2, in3, in4){ std::cout << in1 << in2 << in3 << in4 << std::endl; } void foo()
#define POL5(in1, in2, in3, in4, in5){ std::cout << in1 << in2 << in3 << in4 << in5 << std::endl; } void foo()
#define POL6(in1, in2, in3, in4, in5, in6){ std::cout << in1 << in2 << in3 << in4 << in5 <<in6 << std::endl; } void foo()
#define POL7(in1, in2, in3, in4, in5, in6, in7){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << std::endl; } void foo()
#define POL8(in1, in2, in3, in4, in5, in6, in7, in8){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << std::endl; } void foo()
#define POL9(in1, in2, in3, in4, in5, in6, in7, in8, in9){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << std::endl; } void foo()
#define POL10(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << std::endl; } void foo()
#define POL11(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << std::endl; } void foo()
#define POL12(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << in12 << std::endl; } void foo()
#define POL13(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12, in13){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << in12 << in13 << std::endl; } void foo()
#define POL14(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12, in13, in14){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << in12 << in13 << in14 << std::endl; } void foo()
#define POL15(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12, in13, in14, in15){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << in12 << in13 << in14 << in15 << std::endl; } void foo()
#define POL16(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12, in13, in14, in15, in16){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << in12 << in13 << in14 << in15 << in16 << std::endl; } void foo()
#define POL17(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12, in13, in14, in15, in16, in17){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << in12 << in13 << in14 << in15 << in16 << in17 << std::endl; } void foo()
#define POL18(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12, in13, in14, in15, in16, in17, in18){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << in12 << in13 << in14 << in15 << in16 << in17 << in18 << std::endl; } void foo()
#define POL19(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12, in13, in14, in15, in16, in17, in18, in19){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << in12 << in13 << in14 << in15 << in16 << in17 << in18 << in19 << std::endl; } void foo()
#define POL20(in1, in2, in3, in4, in5, in6, in7, in8, in9, in10, in11, in12, in13, in14, in15, in16, in17, in18, in19, in20){ std::cout << in1 << in2 << in3 << in4 << in5 << in6 << in7 << in8 << in9 << in10 << in11 << in12 << in13 << in14 << in15 << in16 << in17 << in18 << in19 << in20 << std::endl; } void foo()


/**
 * TIME MEASURE MACROS
 * This one is actually not THAT ugly.
 *
 * USE: START(); //code block to execute END();
 */

// REQUIRES TWO GLOBAL VARIABLES UNFORTUNATELY
auto starting_point1 = std::chrono::high_resolution_clock::now();
clock_t starting_point2;

#define START() {\
starting_point1 = std::chrono::high_resolution_clock::now();\
starting_point2 = clock();\
}void foo()

#define END() {\
clock_t end = clock();\
double elapsed_ticks = double(end - starting_point2);\
\
auto done = std::chrono::high_resolution_clock::now();\
std::cout << "\n--------------------[CODE BLOCK EXECUTION TIME]------------------------" << std::endl;\
std::cout << "| " << std::chrono::duration_cast<std::chrono::seconds>(done-starting_point1).count() << " seconds" << std::endl;\
std::cout << "| " << std::chrono::duration_cast<std::chrono::milliseconds>(done-starting_point1).count() << " milliseconds" << std::endl;\
std::cout << "| " << std::chrono::duration_cast<std::chrono::microseconds>(done-starting_point1).count() << " microseconds" <<std::endl;\
std::cout << "| " << std::chrono::duration_cast<std::chrono::nanoseconds>(done-starting_point1).count() << " nanoseconds" << std::endl;\
std::cout << "| " << elapsed_ticks << " CPU CLOCKS (This machine's CPU clocks / Second = " << CLOCKS_PER_SEC << ")" << std::endl;\
std::cout << "-----------------------------------------------------------------------" << std::endl;\
}void foo()


#endif //MESSAGEPROTOCOL_TIMER_H
