obj-m += virtual_homekey.o
PWD := $(shell pwd)

CC := $(CROSS_COMPILE)gcc

all:
	$(MAKE) -C $(KDIR) M=${shell pwd} modules

clean:
	$(MAKE) -C $(KDIR) M=${shell pwd} clean || true
	-rm *.o *.ko *.mod.c *.mod.o modules.order Module.symvers || true

