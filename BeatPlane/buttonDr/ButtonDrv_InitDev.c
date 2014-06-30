#include "ButtonDrv_LowerHalfWork.h"
#include "ButtonDrv_InitDev.h"

#define BUTTON_INFO_QUEUE_SIZE 30

extern SButton_dev * buttons_info;


static SButtonIrqDesc button_irqs [BUTTON_NUM] =
{
    {IRQ_EINT( 0), 0, "KEY0", NULL},
    {IRQ_EINT( 1), 1, "KEY1", NULL},
    {IRQ_EINT( 2), 2, "KEY2", NULL},
    {IRQ_EINT( 3), 3, "KEY3", NULL},
    {IRQ_EINT( 4), 4, "KEY4", NULL},
    {IRQ_EINT( 5), 5, "KEY5", NULL},
    {IRQ_EINT(19), 6, "KEY6", NULL},
    {IRQ_EINT(20), 7, "KEY7", NULL},
};

SButton_dev * Button_initButtonDev()
{
    UCHAR i = 0;

    buttons_info = (SButton_dev*)OS_malloc(sizeof(SButton_dev));

    if(!buttons_info)
    {
        printk("buttonDev: kmalloc memory failed !\n");
    }
    OS_memset(buttons_info, 0, sizeof(SButton_dev));

    buttons_info->Buttoninfo_Queue = (SButtonCmdQueue *)OS_malloc(sizeof(
    SButtonCmdQueue));
    OS_memset( buttons_info->Buttoninfo_Queue, 0, sizeof(SButtonCmd) *
    BUTTON_INFO_QUEUE_SIZE);
    buttons_info->Buttoninfo_Queue->front = 0;
    buttons_info->Buttoninfo_Queue->rear  = 0;

    OS_AllocateSpinLock(&(buttons_info->lock));

    OS_InitWaitQueueHead(&(buttons_info->lowerpart_Wait_Queue));
    buttons_info->condition_for_lowpart = FALSE;


    buttons_info->globalIrqDesc = button_irqs;
    buttons_info->irqDescNum = sizeof(button_irqs)/sizeof(button_irqs[0]);
    printk(KERN_INFO "irqDescNum = %d\n", buttons_info->irqDescNum);


    for(i=0;i<buttons_info->irqDescNum;i++)
    {
        buttons_info->globalIrqDesc[i].buttonDevPtr = (void *)buttons_info;
    }

    return buttons_info;



}

void Button_initLedDev()
{
    unsigned tmp;
    tmp = readl(S3C64XX_GPKCON);
    tmp = (tmp & ~(0xffffU<<16))|(0x1111U<<16);
    writel(tmp, S3C64XX_GPKCON);
    tmp = readl(S3C64XX_GPKDAT);
    tmp |= (0xF << 4);
    writel(tmp, S3C64XX_GPKDAT);
}

void Button_deInitButtonDev(SButton_dev * buttonDev)
{

}

