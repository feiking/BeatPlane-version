#ifndef _APP_INIT_FILE_H_
#define _APP_INIT_FILE_H_

/*******************************************************
* @brief                 declare all Initialize data functions and declare all
********************************************************/

#define FALSE 0
#define TRUE 1

typedef unsigned char U8, *PU8;
typedef unsigned int U32, *PU32;
typedef unsigned long UL32, *PUL32;
typedef void *PVOID;


typedef struct SPos
{
    U32 x;
    U32 y;
}SPos;

typedef struct SList_Link
{
    struct SList_Link * prev;
    struct SList_Link * next;
}SList_Link;

typedef struct SComputerStatu
{
    SList_Link Link; //must be the first one
    struct SComputerPlanePool * pool;
    U32 score;
    U32 health;
    SPos pos;
    U32 flagsForShape;
}SComputerStatu;


typedef struct SComputerPlanePool
{
    SList_Link freeList;
    U32 poolOfAllocated;
    U32 poolOfSize;
    SComputerStatu computerPlaneData[1]; //must be last one
}SComputerPlanePool;

typedef struct SUserPlaneStatu
{
    U32 score;
    U32 health;
    SPos pos;
    U8 userID[8];
}SUserPlaneStatu;


#endif

