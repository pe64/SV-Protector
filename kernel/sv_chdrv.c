#include <linux/init.h>
#include <linux/fs.h>     
#include <linux/types.h>
#include <linux/fcntl.h> 
#include <asm/page.h>
#include <linux/file.h>
#include <linux/mm.h>   
#include <linux/highmem.h>
#include <asm/pgtable.h>
#include <linux/security.h>
#include <linux/audit.h>
#include <linux/fdtable.h>
#include <linux/device.h>

#include "priv.h"

#define PXDEV_MAJOR	207

static struct class *chdev_class;

static int 
chdev_mmap(struct file * filp, struct vm_area_struct *vma)
{
	return -1;
}

static ssize_t 
chdev_write(struct file * file, const char __user * buf, 
		size_t count, loff_t *pos) 
{
	*pos = *pos + count;
	return count;
}

static ssize_t 
chdev_read(struct file * file, char __user * buf, 
		size_t count, loff_t *pos) 
{
	*pos = *pos + count;
	return count;
}

static loff_t 
chdev_lseek(struct file * file, loff_t offset, int origin)
{
	switch (origin) {
	case 0: file->f_pos = offset; return file->f_pos;
	case 1: file->f_pos += offset; return file->f_pos;
	default: return -EINVAL;
	}
}

static int 
chdev_open(struct inode * inode, struct file * file)
{
	printk("[%s][%d]-[%s]\n",__FILE__,__LINE__,__func__); 
	return 0;
}

static int 
chdev_release(struct inode * inode, struct file * file)
{
	return 0;
}

static long 
chdev_ioctl(struct file *file, 
		unsigned int cmd, unsigned long arg)
{
	return svframe_invoke_syscall(cmd, (void __user *)arg);
}

static struct file_operations chdev_fops = {
	owner:		THIS_MODULE,
	mmap:		chdev_mmap,
	llseek:		chdev_lseek,
	read:		chdev_read,
	unlocked_ioctl:	chdev_ioctl,
	write:		chdev_write,
	open:		chdev_open,
	release:	chdev_release,
};

int chdev_init(void)
{
	printk("[%s][%d]-[%s]\n",__FILE__,__LINE__,__func__); 
	register_chrdev(PXDEV_MAJOR, "sv_chdev", &chdev_fops);
	chdev_class = class_create(THIS_MODULE, "chdev_class");
	device_create(chdev_class, NULL, MKDEV(PXDEV_MAJOR, 0), NULL, "sv_chdev");
	return 0;
}

void chdev_fini(void)
{
	unregister_chrdev(PXDEV_MAJOR, "sv_chdev");
	device_destroy(chdev_class, MKDEV(PXDEV_MAJOR, 0));
	class_destroy(chdev_class);
}
