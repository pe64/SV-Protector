#include <linux/fs.h>
#include <linux/ftrace.h>
#include <linux/kprobes.h>
#include <asm/unistd.h>

#include "priv.h"

MODULE_DESCRIPTION("defense and modify file to protect");
MODULE_AUTHOR("wang_xuefei <wxf_happylife@163.com>");
MODULE_LICENSE("GPL");

static int __init sv_protect_init(void)
{
	printk("syscall intercept: Hi, poor linux!\n");
	chdev_init();
	file_init();
	process_init();
	//sv_netlink_init();

	return 0;
}

static void __exit sv_protect_fini(void)
{
	chdev_fini();
	file_fini();
	process_fini();
	//sv_netlink_fini();
	printk("syscall intercept: bye, poor linux!\n");

}

module_init(sv_protect_init);
module_exit(sv_protect_fini);
