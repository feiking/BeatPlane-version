#include "OS_Base.h"


void OS_kthread_run (OS_ThreadProc        threadProc,
                           void        *  parameter,
                           UCHAR          *  name)
{
    kthread_run(threadProc, parameter, name);
}


int OS_request_irq(unsigned int irq,
                         irq_handler_t handler, unsigned long flags,
                         const char *name, void *dev)
{
    return request_irq(irq, handler, flags, name, dev);
}

void OS_disable_irq(unsigned int irq)
{
    disable_irq(irq);
}

void OS_free_irq(unsigned int irq, void * dev)
{
    free_irq(irq, dev);
}



/************************************************************************
                              Doubly-linked list operations
************************************************************************/
OS_LIST_ENTRY_PTR
OS_RemoveHeadList(OS_LIST_ENTRY_PTR Head)
{
    return NULL;
}



SListEntry *
OS_InterlockedInsertTailList(
   SListEntry * Head,
   SListEntry * Node,
   SSpinlock * Lock)
{
    SListEntry * temp = NULL;

    OS_AcquireSpinLock(Lock);
    if (!list_empty(Head))
    {
        temp = Head->prev;
    }
    list_add_tail(Node, Head);
    OS_ReleaseSpinLock(Lock);

    return temp;
}

SListEntry *
OS_InterlockedRemoveHeadList(
   SListEntry * Head,
   SSpinlock * Lock)
{
    SListEntry * temp = NULL;

    OS_AcquireSpinLock(Lock);

    if (!list_empty(Head))
    {
        temp = Head->next;
        list_del(Head->next);
    }
    OS_ReleaseSpinLock(Lock);

    return temp;
}

/************************************************************************
                                  Memory management
************************************************************************/
void * OS_NormalMalloc(UINT size)
{
    return kmalloc(size, GFP_KERNEL);//内存不足的时候，线程会睡眠
}

void * OS_IntMalloc(UINT size)
{
    return kmalloc(size, GFP_ATOMIC);//内存不足的时候，线程不会睡眠
}



/***********************************************************************/
/*                                        Thread management                                                   */
/***********************************************************************/

UINT OS_InitWaitQueueHead(ButtonWaitQueHead * queueHead)
{
    init_waitqueue_head(queueHead);
    return BUTTON_OK;

}

EButtonErrStatus OS_WaitEventInterruptible(ButtonWaitQueHead *wq, EButtonBool * condition)
{
    EButtonErrStatus ret = BUTTON_OK;

    if(wait_event_interruptible(*wq, *condition) == -ERESTARTSYS)
    {
        printk("OS_WaitEventInterruptible: ERESTARTSYS!\n");
        ret = BUTTON_ERR;
    }
    return ret;
}

void OS_Wakeup(ButtonWaitQueHead * wq, EButtonBool * condition)
{
        *condition = BUTTON_TRUE;
        wake_up_interruptible(wq);
}

static __inline EButtonErrStatus OS_ThreadCreate (ButtonThreadHandle   *handle,
                                                                                   OS_ThreadProc          threadProc,
                                                                                   void                         *parameter,
                                                                                   UCHAR                     *name)
{
    EButtonErrStatus      status = BUTTON_OK;
    ButtonThreadHandle retHandle = NULL;

    if (handle == NULL)
    {
        printk("Button_ThreadCreate: NULL thread handle!\n");
        return BUTTON_ERR;
    }

    retHandle = kthread_run(threadProc, parameter, name);

    if (IS_ERR(retHandle))
    {
        printk("Button_ThreadCreate: Create thread failed!\n");
        return BUTTON_ERR;
    }
    else
    {
        *handle = retHandle;
    }

    return status;
}

EButtonErrStatus  OS_WorkThreadStart(SButton_dev           * dev,
                                                                      OS_ThreadProc          threadProc)
{
    ButtonThreadHandle handle;
    EButtonErrStatus rslt;

    rslt = OS_ThreadCreate(&handle,
                                           threadProc,
                                           (void *)dev,
                                           "Button INT Worker");
    if(rslt != BUTTON_OK)
    {
        printk("Create button interrupt working thread Failed! rslt = %d\n", rslt);
    }

    return rslt;
}


