/*******************************************************
* @brief                 define deal memory pool's function
********************************************************/
#include <assert.h>

#include "App_PoolManager_File.h"
#include "App_Init_File.h"
#include "App_OS_File.h"

SComputerPlanePool*
PlaneMemoryPool_Create(U32 poolSize)
{
    assert(poolSize>0);

    SComputerPlanePool* pool = NULL;

    pool = (SComputerPlanePool*)App_OS_Malloc(sizeof(SComputerPlanePool)*1+
                poolSize*sizeof(SComputerStatu));

    App_OS_ZeroMenset(pool,sizeof(SComputerPlanePool)*1+
                poolSize*sizeof(SComputerStatu));

    if(pool)
    {
        U32 i = 0;
        pool->poolOfAllocated = 0;
        pool->poolOfSize        = poolSize;
        App_OS_InitializeListHead(&pool->freeList);

        for(i=0;i<poolSize;++i)
        {
            pool->computerPlaneData[i].pool              = pool;
            pool->computerPlaneData[i].flagsForShape     = 0;
            pool->computerPlaneData[i].health            = 0;
            pool->computerPlaneData[i].pos.y             = 0;
            pool->computerPlaneData[i].pos.x             = 0;
            pool->computerPlaneData[i].score             = 0;

            App_OS_InsertTailList(&pool->freeList, &pool->computerPlaneData[i].Link);
        }
    }

    return pool;
}

void PlaneMemoryPool_Destoye(PVOID pool)
{
    if(pool)
    {
        App_OS_Free(pool);
    }

}

SComputerStatu* PlaneMemoryPool_Alloc(SComputerPlanePool * pool)
{
    assert(NULL != pool);
    SList_Link * p = NULL;

    p = (SList_Link*)App_OS_RemoveHeadList((PVOID)(&pool->freeList));

    if(p)
    {
        pool->poolOfAllocated++;
        return (SComputerStatu*)p;
    }

    return NULL;
}

void PlaneMemoryPool_Free(SComputerStatu * dataPlane)
{
    assert(NULL != dataPlane);

    App_OS_InsertTailList(&dataPlane->pool->freeList,
                                     &dataPlane->Link);

    dataPlane->pool->poolOfAllocated--;
}