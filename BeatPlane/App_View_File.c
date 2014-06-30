#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <memory.h>
#include <assert.h>

#include "App_View_File.h"
#include "App_Init_File.h"

/*******************************************************
* @brief                 define all displayUI function
********************************************************/

static void App_View_DrawComputerPlane(WINDOW *win,PVOID content);

static void App_View_DrawUserPlane(WINDOW *win,PVOID content);

void App_View_InitUI()
{
    initscr();

}


void App_View_DisplayLoginUI(char * userID)
{
    assert(NULL != userID);

    U32 i = 0;
    U32 top_x=0,top_y=0,left_x=0,left_y=0,right_x=0,right_y=0,botton_x=0,botton_y=0;

    PU8 heads="Wellcome To BeatPlane Game !";

    U8 name[9] = {0};

    App_View_InitUI();

    top_x = (COLS-strlen(heads))/2+1;
    top_y = 13;

    left_x = (COLS-strlen(heads))/2;
    left_y = 14;

    right_x = (COLS-strlen(heads))/2+2;
    right_y = 14;

    botton_x = (COLS-strlen(heads))/2+1;
    botton_y = 15;

    box(stdscr,0,0);

    attron(A_BOLD);

    mvaddstr(6,(COLS-strlen(heads))/2,heads);

    mvhline(7,(COLS-strlen(heads))/2,0,strlen(heads));

    attroff(A_BOLD);

    mvaddstr(9,(COLS-strlen(heads))/2,"USERID:[                  ]");

    mvhline(11,(COLS-strlen(heads))/2,'*',strlen(heads));

    /* test ouput plane */
    for(i=0;i<4;i++)
    {
        mvaddch(top_y,top_x+i*8,ACS_UARROW);

        mvaddch(left_y,left_x+i*8,ACS_UARROW);

        mvaddch(right_y,right_x+i*8,ACS_UARROW);

        mvaddch(botton_y,botton_x+i*8,ACS_UARROW);

    }


    mvgetnstr(9,((COLS-strlen(heads))/2)+8,name,8);



    strcpy(userID,name);

}

void App_View_CloseLoginUI()
{
    App_View_DestoyeUI();
}

void App_View_DisplaySelectUI(char flags)
{
    U8 flagsForCurrentSelect = flags;

    PU8 heads = "Mode Select User Interface!";

    PU8 select_1 = "1. Single Fight Plane";

    PU8 select_2 = "2. Union Fight Plane";

    PU8 select_3 = "3. Display All Ranking";

    PU8 select_4 = "4. Exit";

    WINDOW *selectUIWindow;

    clear();//clear screen

    App_View_InitUI();
    box(stdscr,0,0);

    curs_set(FALSE);//set cursor invisible

    mvaddstr(6,(COLS-strlen(heads))/2,heads);

    selectUIWindow = derwin(stdscr,10,65,(LINES-10)/2,(COLS-55)/2);
    box(selectUIWindow,0,0);;

    mvaddstr((LINES-10)/2+1,(COLS-55)/2+3,select_1);

    mvaddstr((LINES-10)/2+3,(COLS-55)/2+3,select_2);

    mvaddstr((LINES-10)/2+5,(COLS-55)/2+3,select_3);

    mvaddstr((LINES-10)/2+7,(COLS-55)/2+3,select_4);

    switch(flagsForCurrentSelect)
    {
        case 1:
            mvaddch((LINES-10)/2+1,(COLS-55)/2+1,ACS_STERLING);
            break;
        case 2:
            mvaddch((LINES-10)/2+3,(COLS-55)/2+1,ACS_STERLING);
            break;
        case 3:
            mvaddch((LINES-10)/2+5,(COLS-55)/2+1,ACS_STERLING);
            break;
        case 4:
            mvaddch((LINES-10)/2+7,(COLS-55)/2+1,ACS_STERLING);
            break;
        default:
            mvaddch((LINES-10)/2+1,(COLS-55)/2+1,ACS_STERLING);
            break;
    }


    refresh();
    wrefresh(selectUIWindow);
    sleep(1);

}

void App_View_CloseSelectUI()
{
    App_View_DestoyeUI();
}

void App_View_DisplayFightUI(PVOID planeStatus,
                                       PVOID currentComputerListHead,
                                       PVOID planePool)
{
    assert(NULL != planeStatus);

    assert(NULL != currentComputerListHead);

    assert(NULL != planePool);

    clear();//clear screen ,inoder to dynamic display

    WINDOW *FightUIWindow;

    SUserPlaneStatu * tempPlaneStatus =   (SUserPlaneStatu*)planeStatus;

    /**************************************
    static unsigned int x = 2,y=3;

    SComputerStatu computerPlane;

    memset(&computerPlane,0,sizeof(SComputerStatu));

    computerPlane.pos.y = y;
    computerPlane.pos.x = x;
    **************************************/

    box(stdscr,0,0);

    mvprintw(8,50+6,"SCORE : %d",tempPlaneStatus->score);

    mvprintw(10,50+6,"HEALTH : %d",tempPlaneStatus->health);

    curs_set(FALSE);//set cursor invisible

    FightUIWindow = derwin(stdscr,30,50,2,4);
    box(FightUIWindow,0,0);

    //mvwaddch(FightUIWindow,1,1,ACS_UARROW);test

    //mvwaddch(FightUIWindow,28,48,ACS_UARROW);test

    App_View_DrawUserPlane(FightUIWindow,planeStatus);

    App_View_DrawComputerPlane(FightUIWindow,currentComputerListHead);

    /**************************************/

    /**************************************/

    refresh();
    wrefresh(FightUIWindow);
    usleep(500000);

}


void App_View_CloseFightUI()
{
    App_View_DestoyeUI();
}

void App_View_DestoyeUI()
{
    endwin();
}


static void App_View_DrawComputerPlane(WINDOW *win,PVOID content)
{
    assert(NULL != win);
    assert(NULL != content);

    SList_Link     * listHead         = (SList_Link*)content;
    SList_Link     * traversePtr      = listHead->next;
    SComputerStatu * pComputerPlane =   NULL;

    while(traversePtr != listHead)
    {
        pComputerPlane = (SComputerStatu*)traversePtr;

        mvwaddch(win,pComputerPlane->pos.y,pComputerPlane->pos.x,ACS_DARROW);
        mvwaddch(win,pComputerPlane->pos.y-1,pComputerPlane->pos.x-1,ACS_DARROW);
        mvwaddch(win,pComputerPlane->pos.y-1,pComputerPlane->pos.x+1,ACS_DARROW);
        mvwaddch(win,pComputerPlane->pos.y-2,pComputerPlane->pos.x,ACS_DARROW);
        pComputerPlane->pos.y++;

        traversePtr = traversePtr->next;
    }
    wrefresh(win);

}

static void App_View_DrawUserPlane(WINDOW *win,PVOID content)
{
    assert(NULL != win);
    assert(NULL != content);

    SUserPlaneStatu* pUserPlane = (SUserPlaneStatu*)content;

    mvwaddch(win,pUserPlane->pos.y,pUserPlane->pos.x,ACS_UARROW);

    mvwaddch(win,pUserPlane->pos.y+1,pUserPlane->pos.x-1,ACS_UARROW);

    mvwaddch(win,pUserPlane->pos.y+1,pUserPlane->pos.x+1,ACS_UARROW);

    mvwaddch(win,pUserPlane->pos.y+2,pUserPlane->pos.x,ACS_UARROW);
}




