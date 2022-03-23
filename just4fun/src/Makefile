#
# Makefile for out-of-tree kernel module
#
KDIR := /lib/modules/`uname -r`/build

obj-m += fun.o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
