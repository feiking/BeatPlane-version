####################################

button-objs := ButtonDrv_DrvModule.o ButtonDrv_InitDev.o ButtonDrv_LowerHalfWork.o OS_Base.o Kernel_PoolManager.o
obj-m := button.o
all: 
	make -C /usr/src/linux-2.6.38 M=$(shell pwd) modules
clean:
#	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
	make -C /usr/src/linux-2.6.38 M=$(shell pwd) clean 
