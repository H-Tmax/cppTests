#ifndef MESSAGEPROTOCOL_SENDABLETYPEENUMLIST_H
#define MESSAGEPROTOCOL_SENDABLETYPEENUMLIST_H
enum TCP_Types {
    //FOR C1'S USE ONLY
            TYPE_0 = 0,
    DUMMY_SENDABLE,

    //FOR C2'S USE ONLY
            TYPE_A = 1000,
    TYPE_B,

    //FOR C3'S USE ONLY
            TYPE_C = 2000,
    TYPE_D,

    //FOR C4'S USE ONLY
            TYPE_E = 3000,
    TYPE_F,

    //FOR C5'S USE ONLY
            TYPE_G = 4000,
    //For QuickSendables
    TYPE_INT = 10000,
    TYPE_DOUBLE,
    TYPE_BOOL,
    TYPE_STRING,
};
#endif //MESSAGEPROTOCOL_SENDABLETYPEENUMLIST_H


