#ifndef __STAR_LOAD_H__
#define __STAR_LOAD_H__

#define STAR_LAOD_NAME "star_load_platform_driver" 

#define TAG "[starload] "
#define LEVEL_E  "Error: " 
#define LEVEL_D  "Debug: "  
#define LEVEL_I  "Infor: "
#define LOGE(a,arg...)\
	do{\
		printk(TAG  LEVEL_E "%s:%d: "a" \n", __FUNCTION__, __LINE__,##arg);\
	}while(0)
#define LOGD(a,arg...)\
	do{\
		printk(TAG  LEVEL_D "%s:%d: "a" \n", __FUNCTION__, __LINE__,##arg);\
	}while(0)
#define LOGI(a,arg...)\
	do{\
		printk(TAG  LEVEL_I "%s:%d: "a" \n", __FUNCTION__, __LINE__,##arg);\
	}while(0)

struct star_platform_data {
	const char* artificial_reg_name;
	const char* digital_reg_name;
};

struct platform_driver star_driver;

int __init start_load_driver_init(void);

void __exit start_laod_driver_exit(void);

#endif