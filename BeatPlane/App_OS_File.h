/*******************************************************
* @brief                  declare OSAL function
********************************************************/
#ifndef _APP_OS_FILE_H_
#define  _APP_OS_FILE_H_



void* App_OS_Malloc(unsigned int size);

void App_OS_Free(void* mem);

void App_OS_ZeroMenset(void * address,unsigned int size);

void App_OS_InitializeListHead(void * list_head);

void App_OS_InsertTailList(void * list_head,void * list_entry);

inline void* App_OS_RemoveHeadList(void * list_head);

unsigned int  App_OS_IsListEmpty(void* list_head);

unsigned int App_OS_GetListLen(void* list_head);





#endif
