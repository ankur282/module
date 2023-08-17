#include<linux/module.h>
#include<linux/platform_device.h>
#include "platform.h"


void pcdev_release(struct device *dev)
{
	pr_info("Device released \n");
};

struct pcdev_platform_data pcdev_pdata[] = {
	[0]={.size=1024, .perm = RDWR, .serial_number = "IMX8MP1A111"},
	[1]={.size=1024, .perm = RDONLY, .serial_number = "IMX8MP2B222"},
};


struct platform_device platform_pcdev_1 = {
	.name = "pcdev-imx8mp1",
	.id = 0,
	.dev = {
		.platform_data = &pcdev_pdata[0],
		.release = pcdev_release
	}
};


struct platform_device platform_pcdev_2 = {
	.name = "pcdev-imx8mp2",
	.id = 1,
	.dev = {
		.platform_data = &pcdev_pdata[1],
		.release = pcdev_release
	}
};




struct platform_device *platform_pcdevs[] = 
{
	&platform_pcdev_1,
	&platform_pcdev_2,
};

static int __init pcdev_platform_init(void)
{
	platform_add_devices(platform_pcdevs,ARRAY_SIZE(platform_pcdevs) );
	pr_info("Device setup module loaded \n");

	return 0;
}
static void __exit pcdev_platform_cleanup(void)
{

	platform_device_unregister(&platform_pcdev_1);
	platform_device_unregister(&platform_pcdev_2);
	pr_info("Device setup module unloaded \n");


}

module_init(pcdev_platform_init);
module_exit(pcdev_platform_cleanup);



MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ankur");
MODULE_DESCRIPTION("platform device");