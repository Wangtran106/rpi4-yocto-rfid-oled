obj-m += rc522_spi.o

# When cross-compiling with Buildroot, KDIR will be passed as an argument.
# If compiling natively on Pi, it defaults to local kernel headers.
KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
