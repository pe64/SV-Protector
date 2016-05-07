#include <linux/ftrace.h>
#include <linux/kprobes.h>
#include <asm/unistd.h>

int netlink_init(void)
{
	printk("hello netlink");
	return 0;
}

void netlink_fini(void)
{
	printk("bye netlink!!!");
}
