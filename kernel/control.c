#include <linux/ftrace.h>
#include <linux/kprobes.h>
#include <asm/unistd.h>

MODULE_DESCRIPTION("Delete and modify the control file, and send a message when the process exits.");
MODULE_AUTHOR("wang_xuefei <wxf_happylife@163.com>");
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


