#include <linux/device.h>  // for driver member from  platform_driver 
#include <linux/init.h>    // late_initcall 
#include <linux/module.h>  // module_init
#include <linux/platform_device.h>    // platform_driver_register
#include <linux/mod_devicetable.h>    // for of_device_id
#include <linux/of.h>      // parse_dt of function 
#include "star_core.h"

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

/* 
 * struct platform_device {
 * 	const char	*name;
 * 	int		id;
 * 	bool		id_auto;
 * 	struct device	dev;
 * 	u32		num_resources;
 * 	struct resource	*resource;
 * 	
 * 	const struct platform_device_id	*id_entry;
 * 	char *driver_override;  // Driver name to force a match 
 * 	
 * 	// MFD cell pointer
 * 	struct mfd_cell *mfd_cell;
 * 	
 * 	// arch specific additions
 * 	struct pdev_archdata	archdata;
 * };
 */
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

const struct of_device_id star_match_table[] = {
	{
		.compatible = "xhlu,starload",
	},
	{},
};

/*
 * struct platform_driver {
 * 	int (*probe)(struct platform_device *);
 * 	int (*remove)(struct platform_device *);
 * 	void (*shutdown)(struct platform_device *);
 * 	int (*suspend)(struct platform_device *, pm_message_t state);
 * 	int (*resume)(struct platform_device *);
 * 	struct device_driver driver;
 * 	const struct platform_device_id *id_table;
 * 	bool prevent_deferred_probe;
 * };
 */ 
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


int __init start_load_driver_init(void){
	LOGI("E");
	
	platform_driver_register(&star_driver);
	
	LOGI("X");
	return 0;
}

void __exit start_laod_driver_exit(void){
	LOGI("E");
	
	platform_driver_unregister(&star_driver);
	
	LOGI("X");
}

module_init(start_load_driver_init);
module_exit(start_laod_driver_exit);

MODULE_DESCRIPTION("support for start load demo driver");
MODULE_LICENSE("GPL v2");
