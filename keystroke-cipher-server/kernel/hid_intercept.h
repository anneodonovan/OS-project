#ifndef HID_INTERCEPT_H
#define HID_INTERCEPT_H

/*
 * hid_intercept_init - register input handler with kernel input subsystem
 * - allocate and zero an input_handler struct
 * - set .event callback to hid_event_handler
 * - set .id_table to match USB HID keyboard devices
 * - call input_register_handler()
 * returns 0 on success, negative error code on failure
 */
int hid_intercept_init(void);

/*
 * hid_intercept_exit - unregister the input handler cleanly
 * - call input_unregister_handler()
 */
void hid_intercept_exit(void);

#endif /* HID_INTERCEPT_H */
