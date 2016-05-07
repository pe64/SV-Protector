#include <asm/unistd.h>
#include <linux/printk.h>

#include "priv.h"

int process_init(void)
{
	printk("process init!!!");
	return 0;
}

void process_fini(void)
{
	printk("process fini!!!");
	return ;
}

