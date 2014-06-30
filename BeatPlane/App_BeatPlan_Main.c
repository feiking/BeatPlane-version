
/*******************************************************
* @brief                 App main flow path
********************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <curses.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <assert.h>

#include "App_Init_File.h"
#include "App_View_File.h"
#include "App_OS_File.h"

#define USER_PLANE_INIT_POS_X 24
#define USER_PLANE_INIT_POS_y 26
#define COMPUTER_MAX_PLANE 20
#define MAX_GENERATE_PLANE_NUM 3
#define COMPUTER_PLANE_INITPOS_Y 3
#define MAX_COMPUTER_PLANE_IN_SCREEN 8


#define ENUM_ECMD_NONSENSE 100
typedef enum EBool
{
    EBool_FALSE,
    EBool_TRUE
}EBool;


typedef enum Ecmd
{
    Ecmd_Nonsense = -1,
    Ecmd_keyOne,
    Ecmd_keyTwo,
    Ecmd_keyThree,
    Ecmd_keyFour
}Ecmd;



typedef struct SButtonCmd
{
   Ecmd        cmd;
}SButtonCmd;



static void App_RandGeneratePlane(
                                          SList_Link * currentComputerListHead,
                                          SComputerPlanePool* planePool,
                                          U32 num)
{
    assert(NULL != currentComputerListHead);
    assert(NULL != planePool);
    assert(num>0);

    U32 i                  = 0;
    U32 count              = 0;
    U32 planeGenerateNum = 0;
    U32 randNum            = 0;

    SComputerStatu* ptrComputerPlane = NULL;

    randNum = rand()%MAX_COMPUTER_PLANE_IN_SCREEN+4;

    if(planePool->poolOfAllocated<randNum)
    {
        planeGenerateNum = rand()%num;

        for(i=0;i<planeGenerateNum;i++)
        {
            ptrComputerPlane = PlaneMemoryPool_Alloc(planePool);
            ptrComputerPlane->pos.y = COMPUTER_PLANE_INITPOS_Y;
            ptrComputerPlane->pos.x = rand()%46+2;

            App_OS_InsertTailList((PVOID)(currentComputerListHead),(PVOID)(&ptrComputerPlane->Link));
        }
     }
}

static void App_DestoyeOutScreenPlane(
                                      SList_Link * currentComputerListHead,
                                      SComputerPlanePool* planePool)
{
    assert(NULL != currentComputerListHead);
    assert(NULL != planePool);

    U32 i     = 0;
    U32 count = 0;

    SList_Link     * traversePtr        = currentComputerListHead->next;
    SList_Link     * prev               = NULL;
    SList_Link     * next               = NULL;
    SComputerStatu * ptrComputerPlane = NULL;

    while(traversePtr != currentComputerListHead)
    {
        ptrComputerPlane = (SComputerStatu*)traversePtr;

        if((ptrComputerPlane->pos.y) > 30) // computer plane out of
                                           // screen,screen size y is 30
        {
            /** delete the out of screen node */
            prev = traversePtr->prev;
            next = traversePtr->next;
            next->prev = prev;
            prev->next = next;

            PlaneMemoryPool_Free(ptrComputerPlane);

            traversePtr = next;
        }
        else
        {
            traversePtr = traversePtr->next;
        }

    }

}


int main(int argc,char * argv[])
{
    U8 userID[8];

    char cmd;

    char flagsForExit = 0;

    char flagsForGame = 0;

    char flagsFoSuspend = 0;


    srand((U32)time(NULL));

    U32 i = 0;
    U32 selectUIViewFlag = 1;

    U32 buttons_fd         = 0;

    SList_Link currentComputerListHead;
    SList_Link * traversePtr = NULL;//only test

    SComputerStatu* ptrComputerPlane = NULL;//only test
    SComputerPlanePool* planePool      = NULL;

    SUserPlaneStatu userPlane;
    App_OS_ZeroMenset((PVOID)(&userPlane),sizeof(SUserPlaneStatu));

    SComputerStatu computerPlane;
    App_OS_ZeroMenset((PVOID)(&computerPlane),sizeof(SComputerStatu));

    userPlane.pos.x = USER_PLANE_INIT_POS_X;
    userPlane.pos.y = USER_PLANE_INIT_POS_y;

    /* 1. Init plane memory */
    App_OS_InitializeListHead((PVOID)(&currentComputerListHead));//init screen plane list

    planePool = PlaneMemoryPool_Create(COMPUTER_MAX_PLANE);//create memory pool

    /* 3. use open function enter kernel */
    buttons_fd = open("/dev/buttons", O_RDWR);
    if (buttons_fd < 0)
    {
        perror("open device buttons");
        exit(1);
    }


    #if 1
    /* 4.display login UI and get ID infomation */
    App_View_DisplayLoginUI(userID);

    clear();//clear screen
    refresh();

    /* 4.1 close LoginUI */
    App_View_CloseLoginUI();

    while(1)
    {
        cmd = Ecmd_Nonsense;

        flagsForExit = FALSE;

        flagsForGame = FALSE;

        /* 5.display game mode select UI */

        App_View_DisplaySelectUI(selectUIViewFlag);

        /* 6.accept read infomation */
        read(buttons_fd, &cmd, sizeof(cmd));

        /* 7.operate the special infomation */
        switch(cmd)
        {
            case Ecmd_keyOne:
                if(selectUIViewFlag>1)
                {
                    selectUIViewFlag--;
                }
                else
                {
                    selectUIViewFlag = 1;
                }
                break;
            case Ecmd_keyTwo:
                if(selectUIViewFlag<4)
                {
                    selectUIViewFlag++;
                }
                else
                {
                    selectUIViewFlag = 4;
                }
                break;
            case Ecmd_keyThree:
                flagsForGame = TRUE;
                break;
            case Ecmd_keyFour:
                flagsForExit = TRUE;
                break;
            default:
                break;
        }
        App_View_CloseSelectUI();
        usleep(300000);

        if(flagsForExit == TRUE)
        {
            flagsForExit = FALSE;
            break;

        }

        /* 8.whether enter game UI ,true enter - false no enter*/
        if(flagsForGame)
        {
            flagsForExit = FALSE;
            flagsFoSuspend = FALSE;

            userPlane.health = 4;
            userPlane.score = 0;

            while(1)
            {
                cmd = Ecmd_Nonsense;

                App_RandGeneratePlane(&currentComputerListHead,planePool,MAX_GENERATE_PLANE_NUM);

                App_View_InitUI();
                /* 8.1 accept read infomation and do special operation */
                read(buttons_fd, &cmd, sizeof(cmd));

                 switch(cmd)
                {
                    case Ecmd_keyOne:
                        if(userPlane.pos.x<=3)
                        {
                            userPlane.pos.x = 3;
                        }
                        else
                        {
                            userPlane.pos.x-=2;
                        }
                        break;
                    case Ecmd_keyTwo:
                        if(userPlane.pos.x>=48)
                        {
                            userPlane.pos.x = 48;
                        }
                        else
                        {
                            userPlane.pos.x+=2;
                        }
                        break;
                    case Ecmd_keyThree:
                        flagsFoSuspend = TRUE;
                        break;
                    case Ecmd_keyFour:
                        flagsForExit = TRUE;
                        break;
                    default:
                        break;
                }

                if(TRUE == flagsForExit)
                {

                    break;

                }

                if(TRUE == flagsFoSuspend)
                {
                    cmd = Ecmd_Nonsense;

                    while(1)
                    {
                        read(buttons_fd, &cmd, sizeof(cmd));

                        if(cmd == Ecmd_keyThree)
                        {
                            flagsFoSuspend = FALSE;
                            break;
                        }
                    }
                }


                /* 8.1 accept read infomation and do special operation */
                App_View_DisplayFightUI((PVOID)(&userPlane),(PVOID)(&currentComputerListHead),planePool);

                App_View_DestoyeUI();

                App_DestoyeOutScreenPlane(&currentComputerListHead,planePool);
            }
         }

        if(TRUE == flagsForExit)
        {

            break;

        }

    }
    #endif
    PlaneMemoryPool_Destoye((PVOID)planePool);
    close(buttons_fd);
    return 0;
}

