obj-m += keycipher_mod.o

keycipher_mod-objs := keycipher.o input_intercept.o fifo_buffer.o proc_stats.o cipher.o typing_stack.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
