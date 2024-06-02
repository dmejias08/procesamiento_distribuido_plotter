#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/serial.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>  // For kmalloc and kfree

#define DRIVER_VERSION "0.1"
#define DRIVER_AUTHOR "Chatgpt, Armin Fuerst, Pavel Machek, Johannes Erdfelt, Vojtech Pavlik, David Kubicek, Johan Hovold, Isaac Barrios Campos"
#define DRIVER_DESC "USB to ttyACM Arduino driver"

#define DEVICE_NAME "arduino_char"
#define CLASS_NAME  "arduino"

// List of USB devices supported by this driver
static struct usb_device_id arduino_table[] = {
	{USB_DEVICE(0x2341, 0x0043)},//UNO
	{USB_DEVICE(0x2341, 0x8036)},//Leonardo
	{}
};

MODULE_DEVICE_TABLE(usb, arduino_table);

static struct usb_driver arduino_usb_driver;
static int major_number;
static struct class* arduino_class = NULL;
static struct device* arduino_device = NULL;

// Struct to hold all of our device specific stuff
struct arduino_serial {
    struct usb_device *udev;          // USB device
    struct usb_interface *interface;  // USB interface
    struct tty_port port;             // TTY port
    struct mutex io_mutex;            // Mutex for IO operations
    unsigned char bulk_in_buffer[256]; // Buffer to hold incoming data
    size_t bulk_in_size;               // Size of the incoming buffer
    __u8 bulk_in_endpointAddr;         // Address of the bulk IN endpoint
    __u8 bulk_out_endpointAddr;        // Address of the bulk OUT endpoint
};

static struct arduino_serial *arduino_dev;

// File operations for the character device
static int arduino_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Arduino char device opened\n");
    file->private_data = arduino_dev;
    return 0;
}

static int arduino_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Arduino char device closed\n");
    return 0;
}

// Read data from the device
static ssize_t arduino_read(struct file *file, char __user *buffer, size_t len, loff_t *offset)
{
    struct arduino_serial *dev = file->private_data;
    int retval;

    if (!dev) return -ENODEV;
    int Shortlen=len;
    mutex_lock(&dev->io_mutex);

    // Perform a bulk transfer to read data from the device
    retval = usb_bulk_msg(dev->udev,
                          usb_rcvbulkpipe(dev->udev, dev->bulk_in_endpointAddr),
                          dev->bulk_in_buffer,
                          min(dev->bulk_in_size, len),
                          &Shortlen, HZ*10);
    
    if (retval) {
        printk(KERN_ERR "Bulk message returned %d\n", retval);
        mutex_unlock(&dev->io_mutex);
        return retval;
    }

    // Copy the data from the kernel buffer to the user buffer
    if (copy_to_user(buffer, dev->bulk_in_buffer, len)) {
        retval = -EFAULT;
    } else {
        retval = len;
    }

    mutex_unlock(&dev->io_mutex);
    return retval;
}

// Write data to the device
static ssize_t arduino_write(struct file *file, const char __user *buffer, size_t len, loff_t *offset)
{
    struct arduino_serial *dev = file->private_data;
    int retval;

    if (!dev) return -ENODEV;

    int Shortlen=len;
    if (len > 256) Shortlen = 256;

    // Copy the data from the user buffer to the kernel buffer
    if (copy_from_user(dev->bulk_in_buffer, buffer, len)) {
        return -EFAULT;
    }

    mutex_lock(&dev->io_mutex);

    // Perform a bulk transfer to write data to the device
    retval = usb_bulk_msg(dev->udev,
                          usb_sndbulkpipe(dev->udev, dev->bulk_out_endpointAddr),
                          dev->bulk_in_buffer,
                          len, &Shortlen, HZ*10);

    if (retval) {
        printk(KERN_ERR "Bulk message returned %d\n", retval);
        mutex_unlock(&dev->io_mutex);
        return retval;
    }

    mutex_unlock(&dev->io_mutex);
    return len;
}

// File operations structure
static struct file_operations fops = {
    .open = arduino_open,
    .release = arduino_release,
    .read = arduino_read,
    .write = arduino_write,
};

// Probe function is called when a device matching the id_table is connected
static int arduino_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i;

    arduino_dev = kzalloc(sizeof(*arduino_dev), GFP_KERNEL);
    if (!arduino_dev) {
        printk(KERN_ERR "Out of memory\n");
        return -ENOMEM;
    }

    arduino_dev->udev = usb_get_dev(interface_to_usbdev(interface));
    arduino_dev->interface = interface;

    mutex_init(&arduino_dev->io_mutex);

    iface_desc = interface->cur_altsetting;

    // Find the bulk-in and bulk-out endpoints
    for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i) {
        endpoint = &iface_desc->endpoint[i].desc;

        if ((endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN) {
            arduino_dev->bulk_in_endpointAddr = endpoint->bEndpointAddress;
            arduino_dev->bulk_in_size = endpoint->wMaxPacketSize;
        } else {
            arduino_dev->bulk_out_endpointAddr = endpoint->bEndpointAddress;
        }
    }

    usb_set_intfdata(interface, arduino_dev);

    // Register the character device
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register a major number\n");
        usb_put_dev(arduino_dev->udev);
        kfree(arduino_dev);
        return major_number;
    }

    // Register the device class
    arduino_class = class_create(CLASS_NAME);
    if (IS_ERR(arduino_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        usb_put_dev(arduino_dev->udev);
        kfree(arduino_dev);
        printk(KERN_ALERT "Failed to register device class\n");
        return PTR_ERR(arduino_class);
    }

    // Create the device
    arduino_device = device_create(arduino_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(arduino_device)) {
        class_destroy(arduino_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        usb_put_dev(arduino_dev->udev);
        kfree(arduino_dev);
        printk(KERN_ALERT "Failed to create the device\n");
        return PTR_ERR(arduino_device);
    }

    printk(KERN_INFO "Arduino (%04X:%04X) plugged\n", id->idVendor, id->idProduct);
    return 0;
}

// Disconnect function is called when the device is disconnected
static void arduino_disconnect(struct usb_interface *interface)
{
    struct arduino_serial *dev = usb_get_intfdata(interface);

    device_destroy(arduino_class, MKDEV(major_number, 0));
    class_unregister(arduino_class);
    class_destroy(arduino_class);
    unregister_chrdev(major_number, DEVICE_NAME);

    usb_set_intfdata(interface, NULL);
    usb_put_dev(dev->udev);
    kfree(dev);

    printk(KERN_INFO "Arduino removed\n");
}

// USB driver structure
static struct usb_driver arduino_usb_driver = {
    .name = "arduino_driver",
    .id_table = arduino_table,
    .probe = arduino_probe,
    .disconnect = arduino_disconnect,
};

// Module initialization function
static int __init arduino_init(void)
{
    int result;

    // Register the USB driver
    result = usb_register(&arduino_usb_driver);
    if (result) {
        printk(KERN_ERR "usb_register failed. Error number %d\n", result);
        return result;
    }

    printk(KERN_INFO "Arduino driver registered\n");
    return 0;
}

// Module exit function
static void __exit arduino_exit(void)
{
    // Deregister the USB driver
    usb_deregister(&arduino_usb_driver);
    printk(KERN_INFO "Arduino driver deregistered\n");
}

module_init(arduino_init);
module_exit(arduino_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERSION);

