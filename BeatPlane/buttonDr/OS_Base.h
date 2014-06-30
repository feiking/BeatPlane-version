#ifndef _OS_BASE_H
#define _OS_BASE_H

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/version.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/wireless.h>
#include <linux/ctype.h>
#include <linux/mm.h>
#include <linux/etherdevice.h>

#include <linux/if_arp.h>
#include <linux/if.h>
#include <linux/socket.h>
#include <net/iw_handler.h>
#include <linux/usb.h>

 #include <linux/kthread.h>
#include <linux/interrupt.h>
#include <asm/processor.h>
 #include <asm/io.h>
#include <asm/atomic.h>
#include <asm/hardirq.h>
#include <asm/uaccess.h>
#include <asm/page.h>
#include <asm/div64.h>

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <linux/interrupt.h>
#include <mach/hardware.h>
#include <linux/platform_device.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>

#include </usr/src/linux-2.6.38/arch/arm/plat-samsung/include/plat/gpio-cfg.h>

#include </usr/src/linux-2.6.38/arch/arm/mach-s3c64xx/include/mach/map.h>
#include </usr/src/linux-2.6.38/arch/arm/mach-s3c64xx/include/mach/regs-clock.h>
#include </usr/src/linux-2.6.38/arch/arm/mach-s3c64xx/include/mach/regs-gpio.h>
#include </usr/src/linux-2.6.38/arch/arm/mach-s3c64xx/include/mach/gpio-bank-e.h>
#include </usr/src/linux-2.6.38/arch/arm/mach-s3c64xx/include/mach/gpio-bank-k.h>
#include </usr/src/linux-2.6.38/arch/arm/mach-s3c64xx/include/mach/gpio-bank-l.h>
#include </usr/src/linux-2.6.38/arch/arm/mach-s3c64xx/include/mach/gpio-bank-n.h>


typedef unsigned char UCHAR, *PUCHAR;
typedef unsigned int UINT, *PUINT;
typedef unsigned long ULONG, *PULONG;

#define BUTTON_CMD_QUEUE_SIZE 30
#define BUTTON_NUM               8

#define FALSE 0
#define TRUE 1

typedef struct task_struct *ButtonThreadHandle;

typedef wait_queue_head_t ButtonWaitQueHead;

typedef struct list_head OS_LIST_ENTRY, *OS_LIST_ENTRY_PTR;


typedef int  (*OS_ThreadProc)(void * context);



/************************************************************************/
/*                                      spin lock operations                                                        */
/************************************************************************/

#define OS_AllocateSpinLock(pLock) do{spin_lock_init(&(pLock)->lock); (pLock)->flags = 0;}while(0)
#define OS_FreeSpinLock(pLock) ((void)0)
#define OS_AcquireSpinLock(pLock) spin_lock_irqsave(&(pLock)->lock, (pLock)->flags);
#define OS_ReleaseSpinLock(pLock) spin_unlock_irqrestore(&(pLock)->lock, (pLock)->flags);

typedef struct SSpinlock {
   spinlock_t lock;
   unsigned long flags;
} SSpinlock;

/************************************************************************/
/*                               Doubly-linked list operations                                                  */
/************************************************************************/
#define OS_InitializeListHead(_list_head_p) INIT_LIST_HEAD(_list_head_p)
#define OS_IsListEmpty(_list_head_p) list_empty(_list_head_p)
#define OS_GetHeadList(_list_head_p) ((_list_head_p)->next)
#define OS_GetTailList(_list_head_p) ((_list_head_p)->prev)
#define OS_ListNext(_list_entry_p) ((_list_entry_p)->next)
#define OS_ListPrev(_list_entry_p) ((_list_entry_p)->prev)




#define OS_InsertHeadList(_list_head, _list_entry) list_add(_list_entry, _list_head)
#define OS_InsertTailList(_list_head, _list_entry) list_add_tail(_list_entry, _list_head)

OS_LIST_ENTRY_PTR OS_RemoveHeadList(OS_LIST_ENTRY_PTR Head);

#define OS_RemoveEntryList(_list_entry) list_del(_list_entry)



#define OS_malloc(size) kmalloc(size, GFP_KERNEL)
#define OS_memcpy(_dest, _src, _count)   memcpy(_dest, _src, _count)
#define OS_memset(_dest, _ch, _count)    memset(_dest, _ch, _count)
#define OS_memmove(_dest, _src, _len)    memmove(_dest, _src, _len)
#define OS_memcmp(_buf1, _buf2, _count)  memcmp(_buf1, _buf2, _count)
#define OS_free(mem) kfree(mem)



typedef struct list_head SListEntry;

SListEntry *
OS_InterlockedInsertTailList(
   SListEntry * Head,
   SListEntry * Node,
   SSpinlock *Lock);

SListEntry *
OS_InterlockedRemoveHeadList(
   SListEntry * Head,
   SSpinlock *Lock);



/************************************************************************/
/*                                         data struct in this module                                             */
/************************************************************************/
typedef enum
{
    BUTTON_OK   = 0,
    BUTTON_WARN  = 4,
    BUTTON_INFO  = 2,
    BUTTON_ERR  = 14,
}EButtonErrStatus;

typedef enum EButtonBool {
    BUTTON_FALSE      = 0,
    BUTTON_TRUE       = 1
} EButtonBool;


typedef enum EBool
{
    EBool_FALSE = 0,
    EBool_TRUE
}EBool;


typedef struct SButtonIrqDesc
{
    int    irq;
    int    number;
    char * name;
    void * buttonDevPtr;
}SButtonIrqDesc;

typedef enum Ecmd
{
    Ecmd_keyOne = 0,
    Ecmd_keyTwo,
    Ecmd_keyThree,
    Ecmd_keyFour
}Ecmd;


typedef enum EButtonStatus
{
    EButtonStatus_Button_Normal,
    EButtonStatus_Button_Down,
    EButtonStatus_Button_Up
}EButtonStatus;

typedef enum ELedStatus
{
    ELedStatus_Off,
    ELedStatus_On
}ELedStatus;


typedef struct SButtonCmd
{
   Ecmd        cmd;
}SButtonCmd;


typedef struct SButtonCmdQueue
{
   SButtonCmd  cmdQueue[BUTTON_CMD_QUEUE_SIZE];
   int         front;
   int         rear;
}SButtonCmdQueue;


typedef struct SButton_dev
{
    SButtonCmdQueue   * Buttoninfo_Queue;

    SSpinlock    lock;

    ButtonWaitQueHead lowerpart_Wait_Queue;
    EButtonBool       condition_for_lowpart;

    SButtonIrqDesc *    globalIrqDesc;
    UINT                irqDescNum;
    char                key_values[BUTTON_NUM];
} SButton_dev;



void OS_kthread_run (OS_ThreadProc        threadProc,
                         void             *parameter,
                         UCHAR               *name);


int OS_request_irq(unsigned int irq, irq_handler_t handler, unsigned long
                   flags,
                   const char *name, void *dev);
void OS_disable_irq(unsigned int irq);
void OS_free_irq(unsigned int irq, void * dev);



/***********************************************************************/
/*                                        Memory management                                                  */
/***********************************************************************/
void * OS_NormalMalloc(UINT size);

void * OS_IntMalloc(UINT size);

/***********************************************************************/
/*                                        Thread management                                                   */
/***********************************************************************/
UINT OS_InitWaitQueueHead(ButtonWaitQueHead * queueHead);

EButtonErrStatus OS_WaitEventInterruptible(ButtonWaitQueHead *wq, EButtonBool * condition);

void OS_Wakeup(ButtonWaitQueHead * wq, EButtonBool * condition);

EButtonErrStatus  OS_WorkThreadStart(SButton_dev           * dev,
                                                                      OS_ThreadProc          threadProc);
#endif

