#include <linux/module.h>

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/input.h>

struct input_dev *input;
static struct platform_device *homekey;

static ssize_t homekey_click(struct device *dev, struct device_attribute *attr, 
				const char *buffer, size_t count){
	int i;	
	int len = strlen(buffer);
	int KEY_VAL;

	//default length is 2 (byte)
	if(len == 2) // Home key
		KEY_VAL = KEY_XFER;

	else if(len == 3) // Back key
		KEY_VAL = KEY_PROG1;
	else // Menu key
		KEY_VAL = KEY_PROG2;

	
	input_report_key(input, KEY_VAL, 1);
	input_report_key(input, KEY_VAL, 0);
	input_sync(input);
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

static int __init homekey_init(void){
	
	int error;

	homekey = platform_device_register_simple("homekey", -1, NULL, 0);
	if(IS_ERR(homekey)){
		PTR_ERR(homekey);
		printk("homekey_init : error during platfrom device register\n");
	}

	sysfs_create_group(&homekey->dev.kobj, &homekey_attr_group);	

	input = input_allocate_device();
	if(!input){
		printk("allocation failed\n");
	}	
	
	input->evbit[0] = BIT_MASK(EV_KEY);
	input->keybit[BIT_WORD(KEY_XFER)] = BIT_MASK(KEY_XFER) | BIT_MASK(KEY_PROG1) | BIT_MASK(KEY_PROG2);
	
	input->name = "homekey";
	input->id.bustype = BUS_HOST;
	input->id.vendor = 0x0001;
	input->id.product = 0x0001;
	input->id.version = 0x0001;

	error = input_register_device(input);
	if(error){
		printk("input device register failed\n");
		goto err_free_dev;
	}

	return 0;

err_free_dev:
	input_free_device(input);
	return error;
	
}

static void __exit homekey_exit (void){
	input_unregister_device(input);
	sysfs_remove_group(&homekey->dev.kobj, &homekey_attr_group);

	platform_device_unregister(homekey);

	return;
}

module_init(homekey_init);
module_exit(homekey_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wangmir <wangmir@gmail.com>");
MODULE_DESCRIPTION("virtual input device");
