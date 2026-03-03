obj-m += keycipher.o

keycipher-objs := keycipher.c hid_intercept.c fifo_buffer.c proc_stats.c cipher.c

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
