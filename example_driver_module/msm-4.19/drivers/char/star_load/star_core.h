#ifndef __STAR_LOAD_H__
#define __STAR_LOAD_H__

#include <linux/device.h>  // for driver member from  platform_driver 
#include <linux/init.h>    // late_initcall 
#include <linux/module.h>  // module_init
#include <linux/platform_device.h>    // platform_driver_register
#include <linux/mod_devicetable.h>    // for of_device_id
#include <linux/of.h>      // parse_dt of function 

#define STAR_LAOD_NAME "star_load_platform_driver" 

#define TAG "[starload] "
#define LEVEL_E  "Error: " 
#define LEVEL_D  "Debug: "  
#define LEVEL_I  "Infor: "
#define LOGE(a,arg...)\
	do{\
		printk(TAG  LEVEL_E "%s:%s: "a" \n");\
	}while(0)
#define LOGD(a,arg...)\
	do{\
		printk(TAG  LEVEL_D "%s:%s: "a" \n");\
	}while(0)
#define LOGI(a,arg...)\
	do{\
		printk(TAG  LEVEL_I "%s:%s: "a" \n");\
	}while(0)

struct star_platform_data {
	const char* artificial_reg_name;
	const char* digital_reg_name;
};

struct platform_driver star_driver;

int __init start_load_driver_init(void);

void __exit start_laod_driver_exit(void);

#endif