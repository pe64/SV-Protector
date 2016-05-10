#include <asm/unistd.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#include "priv.h"



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
