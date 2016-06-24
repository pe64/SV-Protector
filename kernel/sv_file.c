#include <asm/unistd.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/gfp.h>
#include <linux/slob_def.h>
#include <linux/list.h>
#include <asm/uaccess.h>

#include "sv_base.h"
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

static long 
sv_file_protect_unlock_ioctl(struct file *filp, 
		unsigned int cmd, unsigned long arg)
{
	return system_unlock_ioctl(filp, cmd, arg);
}

static long 
sv_file_protect_compat_ioctl(struct file *filp, 
		unsigned int cmd, unsigned long arg)
{
	return system_compat_ioctl(filp, cmd, arg);
}

static void 
replace_fops_ioctl(struct file_operations *f_op)
{
	if(f_op){
		f_op->unlocked_ioctl = sv_file_protect_unlock_ioctl;
#ifdef CONFIG_COMPAT
		f_op->compat_ioctl = sv_file_protect_compat_ioctl;
#endif
	}
}

static struct file_operations *
get_file_operations(char *path)
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

int svfile_add_protect_file(void *args)
{
	//printk("[%s][%d] file[%s]\n",__FILE__,__LINE__, args->file_name);
	svfile_list_st *node;
	svfile_set_st req;
	
	memcpy(&req, args, sizeof(req));

	if(!s_fops){
		s_fops = get_file_operations(req.file_name);
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

int svfile_del_protect_file(void *args)
{
	svfile_set_st req;
	
	memcpy(&req, args, sizeof(req));
	
	return SV_OK;
}

int svfile_list_protect_file(void *args)
{
	svfile_get_st req;
	memcpy(&req, args, sizeof(req));
	return SV_OK;
}

static int (*sv_file_invoke_table[])(void *args) = {
	svfile_add_protect_file,
	svfile_del_protect_file,
	svfile_list_protect_file,
};

int svfile_dealwith_entry(void *args)
{
	sv_kernel_req_st req;
	if(copy_from_user(&req, args, sizeof(req))){
		printk("file:%s,line:%d,func:%s,get req error!!!!\n",__FILE__,__LINE__,__func__); 
		return SV_ERROR;
	}

	if(req.module_cmd > SV_FILE_REQ_START && req.module_cmd < SV_FILE_REQ_MAX){
		return sv_file_invoke_table[req.module_cmd](&req.pos);
	}

	return SV_ERROR;
}

sv_syscall_ops_st sv_file_ops = {
	"svfile_opts",SV_FRAME_PROTECT_FILE,svfile_dealwith_entry,
};

int file_init(void)
{
	svframe_register_kernel_syscall(&sv_file_ops);
	INIT_LIST_HEAD(&s_svfile_head.list);
	return 0;
}

void file_fini(void)
{
	printk("file fini!!!!");
	return ;
}
