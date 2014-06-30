#ifndef _APP_VIEW_FILE_H_
#define _APP_VIEW_FILE_H_
/*******************************************************
* @brief                 declare all displayUI function
********************************************************/


void App_View_InitUI();

void App_View_DisplayLoginUI(char * userID);

void App_View_CloseLoginUI();

void App_View_DisplaySelectUI(char flags);

void App_View_CloseSelectUI();

void App_View_DisplayFightUI(void * planeStatus,
                             void * currentComputerListHead,
                             void * planePool);

void App_View_CloseFightUI();

void App_View_DestoyeUI();




#endif

