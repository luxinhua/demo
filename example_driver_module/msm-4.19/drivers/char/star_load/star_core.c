#include "star_core.h"

int star_probe(struct platform_device * pdev) {
	printk("xhlu : %s , %s\n",__func__,__LINE__);
	return 0;
}

int star_remove(struct platform_device * pdev) {
	printk("xhlu : %s , %s\n",__func__,__LINE__);
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
	platform_driver_register(&star_driver);
	return 0;
}

void __exit start_laod_driver_exit(void){
	platform_driver_unregister(&star_driver);
}

module_init(start_load_driver_init);
module_exit(start_laod_driver_exit);

MODULE_DESCRIPTION("support for start load demo driver");
MODULE_LICENSE("GPL v2");
