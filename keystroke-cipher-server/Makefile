all: kernel userspace

kernel:
	$(MAKE) -C kernel/

userspace:
	$(MAKE) -C userspace/

clean:
	$(MAKE) -C kernel/ clean
	$(MAKE) -C userspace/ clean

.PHONY: all kernel userspace clean
