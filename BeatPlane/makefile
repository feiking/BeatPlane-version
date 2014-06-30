obj=App_BeatPlan_Main.o App_Init_File.o App_View_File.o App_PoolManager_File.o App_OS_File.o

all:$(obj)
	@gcc  -lcurses $(obj) -o BeatPlane 

App_Beatplan_Main.0:App_Init_File.h App_View_File.h App_OS_File.h App_PoolManager_File.h App_BeatPlan_Main.c
	@gcc -c -lcurses App_BeatPlan_Main.c

App_Init_File.o:App_Init_File.h App_Init_File.c
	@gcc -c -lcurses App_Init_File.c

App_View_File.o:App_View_File.h App_Init_File.h App_View_File.c
	@gcc -c -lcurses App_View_File.c

App_PoolManager_File.o:App_PoolManager_File.h App_OS_File.h App_PoolManager_File.c
	@gcc -c -lcurses App_PoolManager_File.c

App_OS_File.o:App_Init_File.h App_OS_File.h App_OS_File.c
	@gcc -c -lcurses App_OS_File.c

.PHONY:clean

clean:
	rm BeatPlane $(obj)


