#include "ButtonDrv_LowerHalfWork.h"
#include "ButtonDrv_InitDev.h"

#define DEVICE_NAME "buttons"

volatile UCHAR dev_is_open = 0;
volatile UINT is_kill_lower_part = FALSE;


SButton_dev * buttons_info = NULL;

static irqreturn_t buttons_interrupt(int irq, void *dev_id)
{
    SButtonIrqDesc * button_irqs = (SButtonIrqDesc *)dev_id;
    SButton_dev    * button_info = (SButton_dev *)button_irqs->buttonDevPtr;

    int down;
    int number;

    static int old_down = 0;
    static int old_number = -1;
    unsigned tmp;

    number = button_irqs->number;

    switch(number)
    {
        case 0: case 1: case 2: case 3: case 4: case 5:
            tmp = readl(S3C64XX_GPNDAT);
            down = !(tmp & (0x1<<number));   /* get the button value */
            break;
        case 6: case 7:
            tmp = readl(S3C64XX_GPLDAT);
            down = !(tmp & (0x1 << (number + 5)));
            break;
        default:
            down = 0;
    }


    if( down == old_down || (number != old_number && old_down == 1))
    {
        return IRQ_RETVAL(IRQ_HANDLED);
    }

    OS_AcquireSpinLock(&button_info->lock);

    OS_memset(button_info->key_values, EButtonStatus_Button_Normal, BUTTON_NUM);

    switch(down)
    {
        case 0:
            button_info->key_values[number] = EButtonStatus_Button_Up;
            break;
        case 1:
            button_info->key_values[number] = EButtonStatus_Button_Down;
            break;
        default:
            button_info->key_values[number] = EButtonStatus_Button_Normal;
            break;
    }
    OS_ReleaseSpinLock(&button_info->lock);

    old_down = down;
    old_number = number;

    //printk(KERN_INFO "interrup\n"); only test

    OS_Wakeup(&button_info->lowerpart_Wait_Queue, &button_info->condition_for_lowpart);

    return IRQ_RETVAL(IRQ_HANDLED);
}


static int register_buttons_interrupt(SButton_dev * buttons_info)
{
    UCHAR i = 0;
    UCHAR err = 0;

    for (i = 0; i < buttons_info->irqDescNum; i++)
    {
        if (buttons_info->globalIrqDesc[i].irq < 0)
        {
            continue;
        }

        // use one routine to do interrupt, so you must have flag to know which key is pressed.
        // so, we pass each buttons_info->button_irq_Desc_ptr[i] to interrupt routine here.
        err = OS_request_irq(buttons_info->globalIrqDesc[i].irq,
                             buttons_interrupt, IRQ_TYPE_EDGE_BOTH,
                             buttons_info->globalIrqDesc[i].name, (void *)&
                             buttons_info->globalIrqDesc[i]);
        if (err)
        {
            break;
        }
    }

    if (err)
    {
        i--;
        for (; i >= 0; i--)
        {
            if (buttons_info->globalIrqDesc[i].irq < 0)
            {
                continue;
            }
            OS_disable_irq(buttons_info->globalIrqDesc[i].irq);
            OS_free_irq(buttons_info->globalIrqDesc[i].irq, (void *)&buttons_info->globalIrqDesc[i]);
        }
        return -EBUSY;
    }
    return -EBUSY;
}






static int s3c64xx_buttons_open(struct inode *inode, struct file *file)
{
    /* allow only open once on this device */
    if(1 == dev_is_open)
    {
        return 0;
    }

    /* init leds */
    Button_initLedDev();

    /* init buttons dev */
    buttons_info = Button_initButtonDev();

    /* register buttons interrupt */
    register_buttons_interrupt(buttons_info);


    /* create kernel thread for lower part */
    OS_kthread_run(LowerPart_WorkThreadFun, (void *)buttons_info, "button: lower part");

    file->private_data = buttons_info;

    dev_is_open = 1;

    return 0;
}

static int s3c64xx_buttons_close(struct inode *inode, struct file *file)
{
    int i = 0;

    if(dev_is_open == 0)
    {
      return 0;
    }


    if(buttons_info)
    {
        /*kill lower part*/
        is_kill_lower_part = EBool_TRUE;
        OS_Wakeup(&buttons_info->lowerpart_Wait_Queue,
                  &buttons_info->condition_for_lowpart);
        while(EBool_TRUE == is_kill_lower_part);

        for (i = buttons_info->irqDescNum - 1; i >= 0; i--)
        {
            if (buttons_info->globalIrqDesc[i].irq < 0)
            {
                continue;
            }
            OS_disable_irq(buttons_info->globalIrqDesc[i].irq);
            OS_free_irq(buttons_info->globalIrqDesc[i].irq, (void *)&
                        buttons_info->globalIrqDesc[i]);
        }

        buttons_info->condition_for_lowpart = EBool_FALSE;
        OS_FreeSpinLock(&(buttons_info->lock));

        if(buttons_info->Buttoninfo_Queue)
        {
            OS_free(buttons_info->Buttoninfo_Queue);
            buttons_info->Buttoninfo_Queue = NULL;
        }

        OS_free(buttons_info);
        buttons_info = NULL;
    }

    /* only close this device, can open next time  */
    dev_is_open = 0;

    return 0;

}

static int s3c64xx_buttons_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
    unsigned long err;

    OS_AcquireSpinLock(&buttons_info->lock);
    Get_One_Button_Cmd(buff);
    OS_ReleaseSpinLock(&buttons_info->lock);

    return sizeof(char);
}

static unsigned int s3c64xx_buttons_poll( struct file *file, struct poll_table_struct *wait)
{
    unsigned int mask = 0;
    SButton_dev * buttonDev = (SButton_dev *)file->private_data;

    poll_wait(file, &buttonDev->lowerpart_Wait_Queue, wait);

    if (buttonDev->condition_for_lowpart)
        mask |= POLLIN | POLLRDNORM;

    return mask;
}

static struct file_operations dev_fops =
{
    .owner = THIS_MODULE,
    .open = s3c64xx_buttons_open,
    .release = s3c64xx_buttons_close,
    .read = s3c64xx_buttons_read,
    .poll = s3c64xx_buttons_poll,
};

static struct miscdevice misc =
{
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &dev_fops,
};

static int __init dev_init(void)
{
    int ret;

    ret = misc_register(&misc);
    printk (DEVICE_NAME"\tinitialized\n");
    return ret;
}
static void __exit dev_exit(void)
{
    misc_deregister(&misc);
}

module_init(dev_init);
module_exit(dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Softi Inc.");
