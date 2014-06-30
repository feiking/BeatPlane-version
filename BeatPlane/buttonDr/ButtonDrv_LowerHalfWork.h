#ifndef _BUTTON_DRV_LOWER_HALF_WORK_H
#define _BUTTON_DRV_LOWER_HALF_WORK_H

#include "OS_Base.h"

int  LowerPart_WorkThreadFun(void * context);

EBool Get_One_Button_Cmd(char * cmdPtr);


#endif

