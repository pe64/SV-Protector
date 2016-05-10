#include <asm/unistd.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#include "priv.h"

const struct file_operations *get_file_operations(char *path)
{
	struct file *filp;
	filp = filp_open(path, O_RDWR, 0644);
	if(IS_ERR(filp)){
		return NULL;
	}

	return filp->f_op;
}


int file_init(void)
{
	printk("file init!!!");
	return 0;
}

void file_fini(void)
{
	printk("file fini!!!!");
	return ;
}
