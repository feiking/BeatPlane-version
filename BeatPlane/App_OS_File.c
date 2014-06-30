/*******************************************************
* @brief                 define OSAL function
********************************************************/
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stdio.h>

#include "App_OS_File.h"
#include "App_Init_File.h"

U32 App_OS_IsListEmpty(void * list_head)
{
    assert(NULL != list_head);

    SList_Link * listHead = (SList_Link*)list_head;

    if(listHead->next == listHead && listHead->prev == listHead)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

U32 App_OS_GetListLen(void * list_head)
{
    assert(NULL != list_head);

    U32 count = 0;

    SList_Link * listHead = (SList_Link*)list_head;

    SList_Link * traversePtr = listHead->next;

    while(traversePtr != listHead )
    {
        traversePtr = traversePtr->next;
        count++;
        printf("count = %d\n",count);
    }

    return count;

}

PVOID App_OS_Malloc(U32 size)
{
    assert(size>0);

    return  malloc(size);
}

void App_OS_Free(PVOID mem)
{
    free(mem);
}

void App_OS_ZeroMenset(void *address,unsigned int size)
{
    assert(NULL !=address);
    assert(size>0);

    memset(address,0,size);
}

void App_OS_InitializeListHead(void * list_head)
{
    SList_Link* listPtr = (SList_Link*)list_head;

    listPtr->prev = listPtr;
    listPtr->next = listPtr;
}

void App_OS_InsertTailList(void * list_head,void * list_entry)
{
    assert(NULL != list_head);
    assert(NULL != list_entry);

    SList_Link* listPtrForhead = list_head;

    SList_Link* listPtrForEntry = list_entry;

    listPtrForEntry->prev = listPtrForhead->prev;

    listPtrForhead->prev->next = listPtrForEntry;

    listPtrForEntry->next =  listPtrForhead;

    listPtrForhead->prev = listPtrForEntry;

}

inline void* App_OS_RemoveHeadList(void * list_head)
{
    assert(NULL != list_head);

    if (App_OS_IsListEmpty(list_head))
    {
        return NULL;
    }

    SList_Link * listHead = list_head;
    SList_Link * entry     = NULL;
    SList_Link * next      = NULL;

    entry = listHead->next;
    next  = entry->next;
    listHead->next = next;
    next->prev = listHead;

    return (PVOID)entry;


}
