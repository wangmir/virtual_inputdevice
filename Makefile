obj-m += virtual_homekey.o
PWD := $(shell pwd)

CC := $(CROSS_COMPILE)gcc

all:
	$(MAKE) -C $(KDIR) M=${shell pwd} modules

clean:
	$(MAKE) -C $(KDIR) M=${shell pwd} clean || true
	-rm *.p *.ko *.mode.{c,o} modules.order Module.symvers || true

