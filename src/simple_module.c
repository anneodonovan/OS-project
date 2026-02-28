#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Team Name");
MODULE_DESCRIPTION("Initial placeholder kernel module");
MODULE_VERSION("0.1");

static int __init simple_init(void)
{
    printk(KERN_INFO "OS Project: module loaded\n");
    return 0;
}

static void __exit simple_exit(void)
{
    printk(KERN_INFO "OS Project: module unloaded\n");
}

module_init(simple_init);
module_exit(simple_exit);
