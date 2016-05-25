#include <asm/unistd.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/gfp.h>
#include <linux/slob_def.h>

#include "sv_base.h"
#include "sv_file.h"
#include "sv_frame.h"
#include "priv.h"

static svfile_list_st s_svfile_head;
static struct file_operations *s_fops;

static long (*system_unlock_ioctl)(struct file *, unsigned int, unsigned long);
static long (*system_compat_ioctl)(struct file *, unsigned int, unsigned long);

static void init_svfile_list_node(svfile_list_st *node)
{
	memset(node, 0, sizeof(svfile_list_st));
	INIT_LIST_HEAD(&node->list);
}

static long sv_file_protect_unlock_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	return system_unlock_ioctl(filp, cmd, arg);
}

static long sv_file_protect_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	return system_compat_ioctl(filp, cmd, arg);
}

static void replace_fops_ioctl(struct file_operations *f_op)
{
	if(f_op){
		f_op->unlocked_ioctl = sv_file_protect_unlock_ioctl;
#ifdef CONFIG_COMPAT
		f_op->compat_ioctl = sv_file_protect_compat_ioctl;
#endif
	}
}

static struct file_operations *get_file_operations(char *path)
{
	struct file *filp;
	struct file_operations *f_op;
	filp = filp_open(path, O_RDWR, 0644);
	if(IS_ERR(filp)){
		return NULL;
	}
	
	f_op = (struct file_operations *)filp->f_op;

	filp_close(filp, 0);

	return f_op;
}

int svfile_add_protect_file(svfile_u2k_args_st *args)
{
	//printk("[%s][%d] file[%s]\n",__FILE__,__LINE__, args->file_name);
	svfile_list_st *node;

	if(!s_fops){
		s_fops = get_file_operations(args->file_name);
		if(!s_fops){
			return SV_ERROR;
		}
		replace_fops_ioctl(s_fops);
	}
	
	node = (svfile_list_st *)kmalloc(sizeof(svfile_list_st), GFP_KERNEL);
	if(node){
		printk("kmalloc error!!!!");
		return SV_ERROR;
	}

	init_svfile_list_node(node);

	return SV_OK;
}

sv_syscall_ops_st file_ops = {
	"add_file", SV_CID_ADD_PROTECT_FILE, svfile_add_protect_file
};

int file_init(void)
{

	printk("file init!!!");
	svframe_register_kernel_syscall(&file_ops);
	INIT_LIST_HEAD(&s_svfile_head.list);

	return 0;
}

void file_fini(void)
{
	printk("file fini!!!!");
	return ;
}
