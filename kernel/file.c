#include <asm/unistd.h>
#include <linux/printk.h>

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
