#ifndef _KERNEL_POOL_MANAGER_H_
#define _KERNEL_POOL_MANAGER_H_

#include "OS_Base.h"


struct SPoolMenager;


struct SPoolNode
{
    OS_LIST_ENTRY            Link;
    struct SPoolMenager *    Pool;
    unsigned char*           Data;
    int                      Size;
};


struct SPoolMenager
{
   OS_LIST_ENTRY          FreeList;
   int                    BuffSize;
   int                    NumOfBuffers;
   int                    NumAllocated;
   struct SPoolNode       PoolNode[1]; // !!! must be the last member
};

typedef struct SPoolNode SPoolNode;

typedef struct SPoolMenager SPoolMenager;


#endif

