#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>

#define TAG "[ cdev Platform_driver ] "
#define LEVEL_E  "Error: " 
#define LEVEL_D  "Debug: "  
#define LEVEL_I  "Infor: "
#define LOGE(a,arg...)\
	do{\
		printk(TAG  LEVEL_E "%s:%d: "a" \n", \
		__FUNCTION__, __LINE__,##arg);\
	}while(0)
#define LOGD(a,arg...)\
	do{\
		printk(TAG  LEVEL_D "%s:%d: "a" \n", \
		__FUNCTION__, __LINE__,##arg);\
	}while(0)
#define LOGI(a,arg...)\
	do{\
		printk(TAG  LEVEL_I "%s:%d: "a" \n", \
		__FUNCTION__, __LINE__,##arg);\
	}while(0)

struct star_platform_data {
	const char* artificial_reg_name;
	const char* digital_reg_name;
};

#define STAR_LAOD_NAME "star_load_platform_driver" 
#define SIZE_MAX_ARRAY 10

static int arrayOperation[SIZE_MAX_ARRAY] = {};


ssize_t cdev_read(struct file * filp, char __user * data, size_t dataSize, loff_t * offset) {
	LOGI("E");
	if (copy_to_user(data, (const void *)arrayOperation, sizeof(arrayOperation[SIZE_MAX_ARRAY]))) {
		LOGE("Failed to copy data to user");
		return -EPERM;
	}
	LOGI("X");
	return 0;
}

ssize_t cdev_write(struct file *filp, const char __user * data, size_t dataSize, loff_t * offset){
	LOGI("E");
	if (copy_from_user(arrayOperation, data, sizeof(arrayOperation[SIZE_MAX_ARRAY]))) {
		LOGE("Failed to copy data from user");
		return -EPERM;
	}
	LOGI("Write data successfully");
	LOGI("X");
	return 0;
}

int cdev_open(struct inode *np, struct file *filp) {
	int i;
	
	LOGI("E");
	for (i = 0 ; i < SIZE_MAX_ARRAY ; i++ ) 
		arrayOperation[i] = 0;
	LOGI("X");
	return 0;
}

int cdev_release(struct inode *np, struct file *filp) {
	int i;
	
	LOGI("E");
	for (i = 0 ; i < SIZE_MAX_ARRAY ; i++ ) 
		arrayOperation[i] = 0;
	LOGI("X");
	return 0;
}

long cdev_unlocked_ioctl (struct file *filp, unsigned int cmd , unsigned long value) {
	LOGI("E");
	
	LOGI("X");
	return 0;
}

int parse_dt(struct device *pdev,struct star_platform_data *pdata){
	int ret; 
	struct device_node *np = pdev->of_node;
	
	ret = of_property_read_string(np,"star,regulator_avdd",&(pdata->artificial_reg_name));
	if (ret == -EINVAL)
		LOGE("Failed to read dt string");
	else if (ret < 0)
		return ret;
	
	ret = of_property_read_string(np,"star,regulator_dvdd",&(pdata->digital_reg_name));
	if (ret == -EINVAL)
		LOGE("Failed to read dt string");
	else if (ret < 0)
		return ret;
	
	return 0;
}

int star_probe(struct platform_device * pdev) {
	struct star_platform_data * pStar_data;
	LOGI("E");
	
	pStar_data = devm_kzalloc(&pdev->dev,sizeof(struct star_platform_data),
		GFP_KERNEL);  // GFP_KERNEL gfp.h ( get free page )
	if (!pStar_data)
		LOGE("Failed to malloc mem for star_platform_data");
	parse_dt(&pdev->dev,pStar_data);
	

	LOGI("X");
	return 0;
}

int star_remove(struct platform_device * pdev) {
	LOGI("E");
	
	LOGI("X");
	return 0;
}

/*
struct file_operations {
 	struct module *owner;
 	loff_t (*llseek) (struct file *, loff_t, int);
 	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
 	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
 	ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
 	ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
 	int (*iterate) (struct file *, struct dir_context *);
 	int (*iterate_shared) (struct file *, struct dir_context *);
 	__poll_t (*poll) (struct file *, struct poll_table_struct *);
 	long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
 	long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
 	int (*mmap) (struct file *, struct vm_area_struct *);
 	unsigned long mmap_supported_flags;
 	int (*open) (struct inode *, struct file *);
 	int (*flush) (struct file *, fl_owner_t id);
 	int (*release) (struct inode *, struct file *);
 	int (*fsync) (struct file *, loff_t, loff_t, int datasync);
 	int (*fasync) (int, struct file *, int);
 	int (*lock) (struct file *, int, struct file_lock *);
 	ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
 	unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
 	int (*check_flags)(int);
 	int (*flock) (struct file *, int, struct file_lock *);
 	ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
 	ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
 	int (*setlease)(struct file *, long, struct file_lock **, void **);
 	long (*fallocate)(struct file *file, int mode, loff_t offset,
 			  loff_t len);
 	void (*show_fdinfo)(struct seq_file *m, struct file *f);
 #ifndef CONFIG_MMU
 	unsigned (*mmap_capabilities)(struct file *);
 #endif
 	ssize_t (*copy_file_range)(struct file *, loff_t, struct file *,
 			loff_t, size_t, unsigned int);
 	int (*clone_file_range)(struct file *, loff_t, struct file *, loff_t,
 			u64);
 	int (*dedupe_file_range)(struct file *, loff_t, struct file *, loff_t,
 			u64);
 	int (*fadvise)(struct file *, loff_t, loff_t, int);
 } __randomize_layout;
*/ 
struct file_operations cdev_fp = {
	.read     = cdev_read,
	.write    = cdev_write,
	.open     = cdev_open,
	.release  = cdev_release,
	.unlocked_ioctl = cdev_unlocked_ioctl,
};

const struct of_device_id star_match_table[] = {
	{
		.compatible = "xhlu,vmdriver",
	},
	{}
};

struct platform_driver star_driver = {
	.remove = star_remove,
	.probe = star_probe,
	/*
	 * struct device_driver {
	 * 	const char		*name;
	 * 	struct bus_type		*bus;
	 * 
	 * 	struct module		*owner;
	 * 	const char		*mod_name;	
	 * 
	 * 	bool suppress_bind_attrs;	
	 * 	enum probe_type probe_type;
	 * 
	 * 	const struct of_device_id	*of_match_table;
	 * 	const struct acpi_device_id	*acpi_match_table;
	 * 
	 * 	int (*probe) (struct device *dev);
	 * 	int (*remove) (struct device *dev);
	 * 	void (*shutdown) (struct device *dev);
	 * 	int (*suspend) (struct device *dev, pm_message_t state);
	 * 	int (*resume) (struct device *dev);
	 * 	const struct attribute_group **groups;
	 * 
	 * 	const struct dev_pm_ops *pm;
	 * 	void (*coredump) (struct device *dev);
	 * 
	 * 	struct driver_private *p;
	 * };
	 */
	.driver = {
		.name = STAR_LAOD_NAME,
		.owner = THIS_MODULE,
		/*
		 * struct of_device_id {
		 * 	char	name[32];
		 * 	char	type[32];
		 * 	char	compatible[128];
		 * 	const void *data;
		 * };
		 */ 
		.of_match_table = star_match_table,
	}
};

int major;

// struct class *virtual_array_class;
// struct device *virtual_array_class_dev;

static int __init virtual_driver_init(void) {
	major = register_chrdev(0,"virtual_driver",&cdev_fp);
	
	// virtual_array_class = class_create(THIS_MODULE,"ccls");
	// virtual_array_class_dev = device_create(virtual_array_class,NULL,MKDEV(major,0),NULL,"VmDriver");
	
	platform_driver_register(&star_driver);
	return 0;
}

static void __exit virtual_driver_exit(void) {
	unregister_chrdev(major,"virtual_driver");

	// device_destroy(virtual_array_class,MKDEV(major,0));
	// class_destroy(virtual_array_class);
	
	platform_driver_unregister(&star_driver);
}

module_init(virtual_driver_init);
module_exit(virtual_driver_exit);


















