#include "Kernel_PoolManager.h"


/// Create a new bd pool
SPoolMenager * BufferDataPool_Create(
                                       void*    Buffer,
                                       int      Num,
                                       int      Size)
{
    SPoolMenager * pool = (SPoolMenager *)OS_NormalMalloc(sizeof(SPoolMenager) + Num
*sizeof(SPoolNode));

    if(pool)
    {
        int i;

        pool->NumOfBuffers = Num;
        pool->NumAllocated = 0;
        pool->BuffSize = Size;
        OS_InitializeListHead(&pool->FreeList);

        for(i=0; i<Num; ++i)
        {
            pool->PoolNode[i].Data = (unsigned char*)Buffer + i*Size;

            pool->PoolNode[i].Size = Size;
            pool->PoolNode[i].Pool = pool;

            OS_InsertTailList(&pool->FreeList, &pool->PoolNode[i].Link);
        }
    }
    return pool;
}

/// Destroy bd pool
void BufferDataPool_Destroy(SPoolMenager * Pool)
{
   if(Pool)
   {
      OS_free(Pool);
   }
}

/// Allocate bd from pool
SPoolNode * BufferDataPool_Alloc(SPoolMenager* Pool)
{
   OS_LIST_ENTRY * p;

   p = OS_RemoveHeadList(&Pool->FreeList);
   if(p)
   {
      Pool->NumAllocated++;
      return (SPoolNode *)p;
   }

   return NULL;
}

/// Return bd to pool
void PoolNode_Free(SPoolNode * poolnode)
{

   OS_InsertTailList(
      &poolnode->Pool->FreeList,
      &poolnode->Link
      );

   poolnode->Pool->NumAllocated--;
}

