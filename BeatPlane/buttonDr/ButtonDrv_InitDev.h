#ifndef _BUTTON_DRV_INIT_DEV_H
#define _BUTTON_DRV_INIT_DEV_H

#include "OS_Base.h"

SButton_dev * Button_initButtonDev(void);

void Button_initLedDev(void);

void Button_deInitButtonDev(SButton_dev * buttonDev);

#endif

