obj-m += lab3_driver.o
 
KDIR = /home/marco/linux
ARCH = arm64
CROSS_COMPILE = aarch64-linux-gnu-
CFLAGS=-I.
 
all:
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(KDIR)  M=$(shell pwd) modules
 
clean:
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- -C $(KDIR)  M=$(shell pwd) clean
