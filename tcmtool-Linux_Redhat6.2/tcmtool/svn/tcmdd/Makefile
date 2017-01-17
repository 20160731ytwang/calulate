#
# Makefile for building the kernel tcm device drivers outside of the kernel
# Will build for the currently booted kernel and the drivers will remain in
# the current directory
#

ifneq ($(KERNELRELEASE),)

obj-$(CONFIG_TCG_TCM) += tcm.o
obj-$(CONFIG_TCG_TIS) += tcm_tis.o

else

KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)

CONFIG	= CONFIG_TCG_TCM=m \
	  CONFIG_TCG_TIS=m \

modules:
	$(MAKE) CFLAGS="-g -DCONFIG_SMP -I$(KDIR)/include/asm-i386/mach-default -I$(PWD)/../../../include" -C $(KDIR) SUBDIRS=$(PWD) $(CONFIG) modules

clean:; $(RM) *.o *.ko *.mod.? *.markers *.symvers *.order

endif

