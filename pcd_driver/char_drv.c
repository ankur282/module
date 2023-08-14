#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include <linux/uaccess.h>
#include<linux/kdev_t.h>

#define DEVICE_NAME "my_chardrv"
#define BUF_SIZE 1024



dev_t device_number;
struct cdev chardrv;
struct class *chardrv_class;
struct device *chardrv_device;
char device_buffer[BUF_SIZE];








loff_t pcd_lseek(struct file *filp, loff_t offset, int whence)
{
	loff_t temp;

	pr_info("lseek requested \n");
	pr_info("Current value of the file position = %lld\n",filp->f_pos);

	switch(whence)
	{
		case SEEK_SET:
			if((offset > BUF_SIZE) || (offset < 0))
				return -EINVAL;
			filp->f_pos = offset;
			break;
		case SEEK_CUR:
			temp = filp->f_pos + offset;
			if((temp > BUF_SIZE) || (temp < 0))
				return -EINVAL;
			filp->f_pos = temp;
			break;
		case SEEK_END:
			temp = BUF_SIZE + offset;
			if((temp > BUF_SIZE) || (temp < 0))
				return -EINVAL;
			filp->f_pos = temp;
			break;
		default:
			return -EINVAL;
	}
	
	pr_info("New value of the file position = %lld\n",filp->f_pos);

	return filp->f_pos;

}


ssize_t chardrv_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
	pr_info("Write requested for %zu bytes\n",count);
	pr_info("Current file position = %lld\n",*f_pos);

	
	/* Adjust the 'count' */
	if((*f_pos + count) > BUF_SIZE)
		count = BUF_SIZE - *f_pos;

	if(!count){
		pr_err("No space left on the device \n");
		return -ENOMEM;
	}

	/*copy from user */
	if(copy_from_user(&device_buffer[*f_pos],buff,count)){
		return -EFAULT;
	}

	/*update the current file postion */
	*f_pos += count;

	pr_info("Number of bytes successfully written = %zu\n",count);
	pr_info("Updated file position = %lld\n",*f_pos);

	/*Return number of bytes which have been successfully written */
	return count;
}



ssize_t chardrv_read(struct file *filp,char __user *buff, size_t count, loff_t *f_pos)
{
	pr_info("Write requested for %zu bytes\n",count);
	pr_info("Current file position = %lld\n",*f_pos);

	
	/* Adjust the 'count' */
	if((*f_pos + count) > BUF_SIZE)
		count = BUF_SIZE - *f_pos;

	if(!count){
		pr_err("No space left on the device \n");
		return -ENOMEM;
	}

	/*copy from user */
	if(copy_from_user(&device_buffer[*f_pos],buff,count)){
		return -EFAULT;
	}

	/*update the current file postion */
	*f_pos += count;

	pr_info("Number of bytes successfully written = %zu\n",count);
	pr_info("Updated file position = %lld\n",*f_pos);

	/*Return number of bytes which have been successfully written */
	return count;
}


int chardrv_open(struct inode *inode, struct file *filp)
{
	pr_info("open was successful..\n");

	return 0;
}

int chardrv_release(struct inode *inode, struct file *flip)
{
	pr_info("release was successful..\n");

	return 0;
}



struct file_operations chardrv_fops=
{
	.open = chardrv_open,
	.release = chardrv_release,
	.read = chardrv_read,
	.write = chardrv_write,
	.owner = THIS_MODULE
};




static int __init chardrv_init(void)
{
	
	alloc_chrdev_region(&device_number,0,1,DEVICE_NAME);
	
	pr_info("Device Major and Minor number = %d:%d\n",MAJOR(device_number),MINOR(device_number));
	
	cdev_init(&chardev,&pcd_fops);
	chardrv.owner=THIS_MODULE;
	
	class_create(THIS_MODULE,DEVICE_NAME);
	device_create(chardrv_class,NULL,device_number,NULL,"chardrv");
	
	pr_info("Module init was successful..\n");
	
	
	
	
	
	
}












static void __exit chardrv_cleanup(void)
{
	
	unregister_chrdev_region(device_number,1);
	cdev_del(&chardrv);
	device_destroy(chardrv_class,device_number);
	class_destroy(chardrv_class);
	
	pr_info("module cleanup sucessful.. \n");
	
	
	
	
}








module_init(chardrv_init);
module_exit(chardrv_cleanup);



MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ankur");
MODULE_DESCRIPTION("character driver");
