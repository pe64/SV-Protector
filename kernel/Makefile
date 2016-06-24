
EXTRA_CFLAGS += -I/root/github/SV-Protector/include

obj-m += sv_protect.o
sv_protect-objs := sv_control.o sv_process.o sv_file.o sv_frame.o sv_chdrv.o
#generate the path
CURRENT_PATH:=$(shell pwd)
#the current kernel version number
	LINUX_KERNEL:=$(shell uname -r)
#the absolute path
	LINUX_KERNEL_PATH:=/usr/src/kernels/$(LINUX_KERNEL)
	#LINUX_KERNEL_PATH:=/root/linux-2.6.32.68/
#complie object
all:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules

clean:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean

install:
	cp defense.ko ../../../lib/64/
