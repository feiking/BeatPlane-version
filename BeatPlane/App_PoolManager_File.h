/*******************************************************
* @brief                 declare deal memory pool's function
********************************************************/
#ifndef _APP_POOL_MANAGER_FILE_
#define _APP_POOL_MANAGER_FILE_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <string.h>

#include "App_Init_File.h"

SComputerPlanePool*
PlaneMemoryPool_Create(U32 poolSize);

void PlaneMemoryPool_Destoye(PVOID pool);

SComputerStatu* PlaneMemoryPool_Alloc(SComputerPlanePool * pool);

void PlaneMemoryPool_Free(SComputerStatu * dataPlane);


#endif
