CONFIG_MODULE_SIG=n

obj-m += my_debug.o

KROOT=/lib/modules/$(shell uname -r)/build
modules:
	$(MAKE) -C $(KROOT) M=$(shell pwd) modules

mouldes_install:
	$(MAKE) -C $(KROOT) M=$(shell pwd) modules_install

kernel_clean:
	$(MAKE) -C $(KROOT) M=$(shell pwd) clean

clean: kernel_clean
	rm -rf Module.symvers modules.order
