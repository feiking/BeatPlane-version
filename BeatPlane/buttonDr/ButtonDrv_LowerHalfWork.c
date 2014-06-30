#include "ButtonDrv_LowerHalfWork.h"

extern int is_kill_lower_part;
extern SButton_dev * buttons_info;

static EBool Is_Full_Queue(SButtonCmdQueue * queue)
{
    if(queue->rear + 1 == queue->front)
    {
        return EBool_TRUE;
    }
    return EBool_FALSE;
}

static EBool Is_Empty_Queue(SButtonCmdQueue * queue)
{
    if(queue->rear == queue->front)
    {
        return EBool_TRUE;
    }
    return EBool_FALSE;
}


static EBool Out_Queue(SButtonCmdQueue * queue, SButtonCmd * cmd)
{
    if(queue == NULL || cmd == NULL)
    {
        return EBool_FALSE;
    }
    if(Is_Empty_Queue(queue))
    {
        return EBool_FALSE;
    }
    *cmd = queue->cmdQueue[queue->front];
    queue->front = (queue->front+1)%BUTTON_CMD_QUEUE_SIZE;

    return EBool_TRUE;
}

static EBool Out_Queue_NoRecord(SButtonCmdQueue * queue)
{
    if(queue == NULL)
    {
        return EBool_FALSE;
    }
    if(Is_Empty_Queue(queue))
    {
        return EBool_FALSE;
    }
    queue->front = (queue->front+1)%BUTTON_CMD_QUEUE_SIZE;

    return EBool_TRUE;
}


static void In_Queue(SButtonCmdQueue * queue, SButtonCmd cmd)
{
    if(queue == NULL)
    {
        return;
    }
    if(Is_Full_Queue(queue))
    {
        Out_Queue_NoRecord(queue);
    }
    queue->cmdQueue[queue->rear] = cmd;
    queue->rear = (queue->rear + 1) % BUTTON_CMD_QUEUE_SIZE;
}

EBool Get_One_Button_Cmd(char * cmdPtr)
{
    SButtonCmd tmp;
    if(cmdPtr == NULL)
    {
        return EBool_FALSE;
    }
    if(Out_Queue(buttons_info->Buttoninfo_Queue, &tmp))
    {
        *cmdPtr = tmp.cmd;
        return EBool_TRUE;
    }
    return EBool_FALSE;
}






static void LowerPart_LedControl(int ledNum)
{

}

int  LowerPart_WorkThreadFun(void * context)
{
    UCHAR               i                = 0;
    EButtonErrStatus rslt             = BUTTON_OK;
    SButton_dev   *  button_info      = (SButton_dev *)context;
    SButtonCmd       button_cmd       = {0};
    UCHAR            key_values[BUTTON_NUM] = {0};

    for(;;)
    {
        rslt = OS_WaitEventInterruptible(&button_info->lowerpart_Wait_Queue,
                                         &button_info->condition_for_lowpart);
        button_info->condition_for_lowpart = FALSE;

        //printk(KERN_INFO "half \n"); only test

        if(rslt)
        {
            printk("CRITICAL: Wait failed!!\n");
            break;
        }
        if(is_kill_lower_part == TRUE)
        {
            printk("get message, kill lower part \n");
            is_kill_lower_part = FALSE;
            break;
        }

        OS_AcquireSpinLock(&button_info->lock);
        OS_memcpy(key_values, button_info->key_values, BUTTON_NUM);
        OS_ReleaseSpinLock(&button_info->lock);

        for(i = 0; i < BUTTON_NUM; i++)
        {
            if(button_info->key_values[i] == EButtonStatus_Button_Up)
            {
                switch(i)
                {
                case 0:
                    //printk("0 up\n"); only test
                    button_cmd.cmd = Ecmd_keyOne;
                    break;
                case 1:
                    //printk("1 up\n"); only test
                    button_cmd.cmd = Ecmd_keyTwo;
                    break;
                case 2:
                    //printk("2 up\n"); only test
                    button_cmd.cmd = Ecmd_keyThree;
                    break;
                case 3:
                    //printk("3 up\n"); only test
                    button_cmd.cmd = Ecmd_keyFour;
                    break;
                default:
                    break;
                }

                OS_memset(button_info->key_values, EButtonStatus_Button_Normal,
                           button_info->irqDescNum);
                OS_AcquireSpinLock(&button_info->lock);
                In_Queue(button_info->Buttoninfo_Queue, button_cmd);
                OS_ReleaseSpinLock(&button_info->lock);
            }
        }

    }

    return 0;
}
