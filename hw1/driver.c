/***************************************************************************//**
*  \file       driver.c
*
*  \details    Simple GPIO driver explanation
*
*  \author     EmbeTronicX
*
*  \Tested with Linux raspberrypi 5.4.51-v7l+
*
*******************************************************************************/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/uaccess.h>  //copy_to/from_user()
#include <linux/gpio.h>     //GPIO
#include <linux/err.h>

//LED is connected to this GPIO
#define GPIO_0 (0)
#define GPIO_1 (1)
#define GPIO_2 (2)
#define GPIO_3 (3)
#define GPIO_4 (4)
#define GPIO_5 (5)
#define GPIO_6 (6)
#define GPIO_7 (7)
#define GPIO_8 (8)
#define GPIO_9 (9)
#define GPIO_10 (10)
#define GPIO_11 (11)
#define GPIO_12 (12)
#define GPIO_13 (13)
#define GPIO_14 (23)
#define GPIO_15 (24)

static uint8_t seven_seg[11][7] = 
{
  {0,0,0,0,0,1,0}, //0
  {1,0,0,1,1,1,1}, //1
  {0,0,1,0,0,0,1}, //2
  {0,0,0,0,1,0,1}, //3
  {1,0,0,1,1,0,0}, //4
  {0,1,0,0,1,0,0}, //5
  {0,1,0,0,0,0,0}, //6
  {0,0,0,1,1,1,1}, //7
  {0,0,0,0,0,0,0}, //8
  {0,0,0,0,1,0,0}, //9
  {1,1,1,1,1,1,1}  //:
};

static struct gpio leds[] = {
  {0, GPIOF_OUT_INIT_LOW, "LED_0"},
  {1, GPIOF_OUT_INIT_LOW, "LED_1"},
  {2, GPIOF_OUT_INIT_LOW, "LED_2"},
  {3, GPIOF_OUT_INIT_LOW, "LED_3"},
  {4, GPIOF_OUT_INIT_LOW, "LED_4"},
  {5, GPIOF_OUT_INIT_LOW, "LED_5"},
  {6, GPIOF_OUT_INIT_LOW, "LED_6"},
  {7, GPIOF_OUT_INIT_LOW, "LED_7"},
  {8, GPIOF_OUT_INIT_LOW, "LED_8"},
  {9, GPIOF_OUT_INIT_LOW, "LED_9"},
  {10, GPIOF_OUT_INIT_LOW, "LED_10"},
  {11, GPIOF_OUT_INIT_LOW, "LED_11"},
  {12, GPIOF_OUT_INIT_LOW, "LED_12"},
  {13, GPIOF_OUT_INIT_LOW, "LED_13"},
  {14, GPIOF_OUT_INIT_LOW, "LED_14"},
  {15, GPIOF_OUT_INIT_LOW, "LED_15"},
};

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;
 
static int __init etx_driver_init(void);
static void __exit etx_driver_exit(void);
 
 
/*************** Driver functions **********************/
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, 
                char __user *buf, size_t len,loff_t * off);
static ssize_t etx_write(struct file *filp, 
                const char *buf, size_t len, loff_t * off);
/******************************************************/
 
//File operation structure 
static struct file_operations fops =
{
  .owner          = THIS_MODULE,
  .read           = etx_read,
  .write          = etx_write,
  .open           = etx_open,
  .release        = etx_release,
};

/*
** This function will be called when we open the Device file
*/ 
static int etx_open(struct inode *inode, struct file *file)
{
  pr_info("Device File Opened...!!!\n");
  return 0;
}

/*
** This function will be called when we close the Device file
*/
static int etx_release(struct inode *inode, struct file *file)
{
  pr_info("Device File Closed...!!!\n");
  return 0;
}

/*
** This function will be called when we read the Device file
*/ 
static ssize_t etx_read(struct file *filp, 
                char __user *buf, size_t len, loff_t *off)
{
  uint8_t gpio_state = 0;
  
  //reading GPIO value
  gpio_state = gpio_get_value(GPIO_0);
  
  //write to user
  len = 1;
  if( copy_to_user(buf, &gpio_state, len) > 0) {
    pr_err("ERROR: Not all the bytes have been copied to user\n");
  }
  
  pr_info("Read function : GPIO_0 = %d \n", gpio_state);
  
  return 0;
}

/*
** This function will be called when we write the Device file
*/ 
static ssize_t etx_write(struct file *filp, 
                const char __user *buf, size_t len, loff_t *off)
{
  uint8_t rec_buf[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
  
  if( copy_from_user(rec_buf, buf, len ) > 0) {
    pr_err("ERROR: Not all the bytes have been copied from user\n");
  }

  uint8_t num = rec_buf[0]-48;
  gpio_set_value(GPIO_9, seven_seg[num][0]);
  gpio_set_value(GPIO_10, seven_seg[num][1]);
  gpio_set_value(GPIO_11, seven_seg[num][2]);
  gpio_set_value(GPIO_12, seven_seg[num][3]);
  gpio_set_value(GPIO_13, seven_seg[num][4]);
  gpio_set_value(GPIO_14, seven_seg[num][5]);
  gpio_set_value(GPIO_15, seven_seg[num][6]);
  gpio_set_value(GPIO_0, rec_buf[1]-48);
  gpio_set_value(GPIO_1, rec_buf[2]-48);
  gpio_set_value(GPIO_2, rec_buf[3]-48);
  gpio_set_value(GPIO_3, rec_buf[4]-48);
  gpio_set_value(GPIO_4, rec_buf[5]-48);
  gpio_set_value(GPIO_5, rec_buf[6]-48);
  gpio_set_value(GPIO_6, rec_buf[7]-48);
  gpio_set_value(GPIO_7, rec_buf[8]-48);
  gpio_set_value(GPIO_8, rec_buf[9]-48);

  return len;
}

/*
** Module Init function
*/ 
static int __init etx_driver_init(void)
{
  /*Allocating Major number*/
  if((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) <0){
    pr_err("Cannot allocate major number\n");
    goto r_unreg;
  }
  pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
  /*Creating cdev structure*/
  cdev_init(&etx_cdev,&fops);
 
  /*Adding character device to the system*/
  if((cdev_add(&etx_cdev,dev,1)) < 0){
    pr_err("Cannot add the device to the system\n");
    goto r_del;
  }
 
  /*Creating struct class*/
  if(IS_ERR(dev_class = class_create(THIS_MODULE,"etx_class"))){
    pr_err("Cannot create the struct class\n");
    goto r_class;
  }
 
  /*Creating device*/
  if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"etx_device"))){
    pr_err( "Cannot create the Device \n");
    goto r_device;
  }
  
  //Checking the GPIO is valid or not
  if(gpio_is_valid(GPIO_0) == false){
    pr_err("GPIO %d is not valid\n", GPIO_0);
    goto r_device;
  }
  
  //Requesting the GPIO
  /*if(gpio_request(GPIO_21,"GPIO_21") < 0){
    pr_err("ERROR: GPIO %d request\n", GPIO_21);
    goto r_gpio;
  }*/
  if(gpio_request_array(leds,ARRAY_SIZE(leds)) < 0){
    pr_err("ERROR: GPIO %d request\n", GPIO_0);
    goto r_gpio;
  }
  
  //configure the GPIO as output
  gpio_direction_output(GPIO_0, 0);
  gpio_direction_output(GPIO_1, 0);
  gpio_direction_output(GPIO_2, 0);
  gpio_direction_output(GPIO_3, 0);
  gpio_direction_output(GPIO_4, 0);
  gpio_direction_output(GPIO_5, 0);
  gpio_direction_output(GPIO_6, 0);
  gpio_direction_output(GPIO_7, 0);
  gpio_direction_output(GPIO_8, 0);
  gpio_direction_output(GPIO_9, 0);
  gpio_direction_output(GPIO_10, 0);
  gpio_direction_output(GPIO_11, 0);
  gpio_direction_output(GPIO_12, 0);
  gpio_direction_output(GPIO_13, 0);
  gpio_direction_output(GPIO_14, 0);
  gpio_direction_output(GPIO_15, 0);
  
  /* Using this call the GPIO 21 will be visible in /sys/class/gpio/
  ** Now you can change the gpio values by using below commands also.
  ** echo 1 > /sys/class/gpio/gpio21/value  (turn ON the LED)
  ** echo 0 > /sys/class/gpio/gpio21/value  (turn OFF the LED)
  ** cat /sys/class/gpio/gpio21/value  (read the value LED)
  ** 
  ** the second argument prevents the direction from being changed.
  */
  gpio_export(GPIO_0, false);
  gpio_export(GPIO_1, false);
  gpio_export(GPIO_2, false);
  gpio_export(GPIO_3, false);
  gpio_export(GPIO_4, false);
  gpio_export(GPIO_5, false);
  gpio_export(GPIO_6, false);
  gpio_export(GPIO_7, false);
  gpio_export(GPIO_8, false);
  gpio_export(GPIO_9, false);
  gpio_export(GPIO_10, false);
  gpio_export(GPIO_11, false);
  gpio_export(GPIO_12, false);
  gpio_export(GPIO_13, false);
  gpio_export(GPIO_14, false);
  gpio_export(GPIO_15, false);
  

  
  pr_info("Device Driver Insert...Done!!!\n");
  return 0;
 
r_gpio:
  gpio_free_array(leds, ARRAY_SIZE(leds));
r_device:
  device_destroy(dev_class,dev);
r_class:
  class_destroy(dev_class);
r_del:
  cdev_del(&etx_cdev);
r_unreg:
  unregister_chrdev_region(dev,1);
  
  return -1;
}

/*
** Module exit function
*/ 
static void __exit etx_driver_exit(void)
{
  gpio_unexport(GPIO_0);
  gpio_unexport(GPIO_1);
  gpio_unexport(GPIO_2);
  gpio_unexport(GPIO_3);
  gpio_unexport(GPIO_4);
  gpio_unexport(GPIO_5);
  gpio_unexport(GPIO_6);
  gpio_unexport(GPIO_7);
  gpio_unexport(GPIO_8);
  gpio_unexport(GPIO_9);
  gpio_unexport(GPIO_10);
  gpio_unexport(GPIO_11);
  gpio_unexport(GPIO_12);
  gpio_unexport(GPIO_13);
  gpio_unexport(GPIO_14);
  gpio_unexport(GPIO_15);
  
  gpio_free_array(leds, 16);
  device_destroy(dev_class,dev);
  class_destroy(dev_class);
  cdev_del(&etx_cdev);
  unregister_chrdev_region(dev, 1);
  pr_info("Device Driver Remove...Done!!\n");
}
 
module_init(etx_driver_init);
module_exit(etx_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
MODULE_DESCRIPTION("A simple device driver - GPIO Driver");
MODULE_VERSION("1.32");