#include <linux/ftrace.h>
#include <linux/kprobes.h>
#include <asm/unistd.h>

MODULE_DESCRIPTION("Intercept the system call table in Linux");
MODULE_AUTHOR("alert7 (alert7@xfocus.org) \n\t\talbcamus <albcamus@gmail.com>");
MODULE_LICENSE("GPL");
static int __init this_init(void)
{
	printk("syscall intercept: Hi, poor linux!\n");
	return 0;
}

static void __exit this_fini(void)
{
	printk("syscall intercept: bye, poor linux!\n");
}

module_init(this_init);
module_exit(this_fini);
