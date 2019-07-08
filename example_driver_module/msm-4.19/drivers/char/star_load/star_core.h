#ifndef __STAR_LOAD_H__
#define __STAR_LOAD_H__

#include <linux/device.h>  // for driver member from  platform_driver 
#include <linux/init.h>    // late_initcall 
#include <linux/module.h>  // module_init
#include <linux/platform_device.h>    // platform_driver_register
#include <linux/mod_devicetable.h>    // for of_device_id

#define STAR_LAOD_NAME "star_load_platform_driver" 

struct platform_driver star_driver;

int __init start_load_driver_init(void);

void __exit start_laod_driver_exit(void);

#endif