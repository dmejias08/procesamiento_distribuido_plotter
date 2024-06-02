#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/usb/serial.h>

static const struct usb_device_id arduino_table[] = {
    { USB_DEVICE(0x2341, 0x0043) }, // UNO
    { USB_DEVICE(0x2341, 0x8036) }, // Leonardo
    { } // Terminating entry
};

/*
 * Allow this driver to be automatically loaded
 * for these devices if they are present.
 */
MODULE_DEVICE_TABLE(usb, arduino_table);

static int arduino_open(struct tty_struct *tty, struct usb_serial_port *port)
{
    int retval;

    retval = usb_serial_generic_open(tty, port);
    if (retval)
        return retval;

    dev_info(&port->dev, "Arduino serial device opened\n");
    return 0;
}

static void arduino_close(struct usb_serial_port *port)
{
    usb_serial_generic_close(port);
    dev_info(&port->dev, "Arduino serial device closed\n");
}

static int arduino_probe(struct usb_serial *serial, const struct usb_device_id *id)
{
    dev_info(&serial->dev->dev, "Arduino serial device connected\n");
    return 0;
}

static void arduino_disconnect(struct usb_serial *serial)
{
    dev_info(&serial->dev->dev, "Arduino serial device disconnected\n");
}

static struct usb_serial_driver arduino_device = {
    .driver = {
        .owner = THIS_MODULE,
        .name = "arduino_serial",
    },
    .id_table = arduino_table,
    .num_ports = 1,
    .open = arduino_open,
    .close = arduino_close,
    .probe = arduino_probe,
    .disconnect = arduino_disconnect,
};

static struct usb_serial_driver * const serial_drivers[] = {
    &arduino_device, NULL
};

module_usb_serial_driver(serial_drivers, arduino_table);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Tiny Arduino USB Serial Driver");
