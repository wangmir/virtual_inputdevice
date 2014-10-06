obj-m += virtual_homekey.o
KDIR := /home/ari/tizen_2.2/kernel/linux-3.0
PWD := $(shell pwd)

CC := $(CROSS_COMPILE)gcc

all:
	$(MAKE) -C $(KDIR) M=${shell pwd} modules

clean:
	$(MAKE) -C $(KDIR) M=${shell pwd} clean || true
	-rm *.p *.ko *.mode.{c,o} modules.order Module.symvers || true

