#include <linux/input.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/init.h>

#include <asm/io.h>

static struct input_dev *homekey_dev;
static struct platform_device *homekey_platform_dev;

static ssize_t homekey_click(struct device *dev, struct device_attribute *attr, const char *buffer, size_t count){
	

	input_report_key(homekey_dev, KEY_XFER, 1);
	input_report_key(homekey_dev, KEY_XFER, 0);
	input_sync(homekey_dev);

	return count;
}

DEVICE_ATTR(coordinates, 0644, NULL, homekey_click);

static struct attribute *homekey_attrs[] = {

	&dev_attr_coordinates.attr,
	NULL
};

static struct attribute_group homekey_attr_group = {
	.attrs = homekey_attrs,
};

static int __init homekey_init(void)
{
	int error;
	
	homekey_platform_dev = platform_device_register_simple("homekey", -1, NULL, 0);

	if(IS_ERR(homekey_platform_dev)){
		PTR_ERR(homekey_platform_dev);
		printk("homekey_init: error\n");
	}
	printk("platformdevice creation");	
	sysfs_create_group(&homekey_platform_dev->dev.kobj, &homekey_attr_group);

	homekey_dev = input_allocate_device();
	if (!homekey_dev) {
		printk(KERN_ERR "button.c: Not enough memory\n");
		error = -ENOMEM;
		goto err;
	}
	printk("input_allocate_device() success\n");
	
	homekey_dev->name = "homekey";
	homekey_dev->id.bustype = BUS_HOST;
	homekey_dev->id.vendor=0x0001;
	homekey_dev->id.product = 0x0001;
	homekey_dev->id.version = 0x0100;
	homekey_dev->evbit[0] = BIT_MASK(EV_KEY);
	homekey_dev->keybit[BIT_WORD(KEY_XFER)] = BIT_MASK(KEY_XFER);

	error = input_register_device(homekey_dev);
	if (error) {
		printk(KERN_ERR "button.c: Failed to register device\n");
		goto err_free_dev;
	}
	printk("input_register_device success\n");
	return 0;

 err_free_dev:
	input_free_device(homekey_dev);
err:
	return error;
}

static void __exit homekey_exit(void)
{

        input_unregister_device(homekey_dev);
	sysfs_remove_group(&homekey_platform_dev->dev.kobj, &homekey_attr_group);
	platform_device_unregister(homekey_platform_dev);
}

module_init(homekey_init);
module_exit(homekey_exit);

MODULE_DESCRIPTION("Simple homekey device as misc device");
MODULE_AUTHOR("Hyukjoong Kim <wangmir@gmail.com>");
MODULE_LICENSE("GPL");
