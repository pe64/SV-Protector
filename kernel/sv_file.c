#include <asm/unistd.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/gfp.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/spinlock_types.h>
#include <linux/file.h>

#include "sv_base.h"
#include "sv_frame.h"
#include "priv.h"

#define GET_FILE_INODE(filp) \
	(filp->f_path.dentry->d_inode->i_ino)

static svfile_list_st s_svfile_head;
static struct file_operations *s_fops;
static long (*system_unlock_ioctl)(struct file *, unsigned int, unsigned long);
static long (*system_compat_ioctl)(struct file *, unsigned int, unsigned long);

rwlock_t lock;

static void init_svfile_list_node(svfile_list_st *node)
{
	memset(node, 0, sizeof(svfile_list_st));
	INIT_LIST_HEAD(&node->head);
}

static int check_file_inlist(ino_t inode, svfile_list_st **nodeptr)
{
	struct list_head* list;
	svfile_list_st *node;
	read_lock(&lock);
	list_for_each(list, &s_svfile_head.head){
		node = list_entry(list, svfile_list_st, head);
		if(node->inode == inode){
			if(nodeptr){
				*nodeptr = node;
			}
			read_unlock(&lock);
			return SV_ERROR;
		}
	}
	read_unlock(&lock);
	return SV_OK;
}

static long 
sv_file_protect_unlock_ioctl(struct file *filp, 
		unsigned int cmd, unsigned long arg)
{
	int ret;
	ret = check_file_inlist(GET_FILE_INODE(filp), NULL);
	if(ret == SV_OK && cmd ){
		
	}
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
	svfile_list_st *node;
	svfile_set_st *req;
	struct file *filp;
	int cmd = 0;
	int arg = 0;
	int ret;
	
	req = args;

	if(!s_fops){
		s_fops = get_file_operations(req->file_name);
		if(!s_fops){
			return SV_ERROR;
		}
		replace_fops_ioctl(s_fops);
	}

	ret = check_file_inlist(req->inode, NULL);
	if(ret != SV_OK){
		return SV_ERROR;
	}

	node = (svfile_list_st *)kmalloc(sizeof(svfile_list_st), GFP_KERNEL);
	if(node){
		printk("kmalloc error!!!!");
		return SV_ERROR;
	}
	
	init_svfile_list_node(node);
	//TODO:modify d_path() function to get file name.
	strcpy(node->name, req->file_name);
	node->inode = req->inode;
	strcpy(node->password, req->password);
	filp = fget(req->fd);
	if(filp){
		//TODO:lock file args.
		system_unlock_ioctl(filp, cmd, (unsigned long)&arg);
	}
	fput(filp);
	write_lock(&lock);
	list_add(&node->head, &s_svfile_head.head);
	write_unlock(&lock);

	return SV_OK;
}

int svfile_del_protect_file(void *args)
{
	svfile_set_st *req;
	svfile_list_st *node;
	int ret;

	req = args;
	node = NULL;

	ret = check_file_inlist(req->inode, &node);
	if(ret == SV_OK || !node){
		return SV_ERROR;
	}

	write_lock(&lock);
	if(!strcmp(node->password, req->password)){
		list_del(&node->head);
		kfree(node);
		write_unlock(&lock);
		return SV_OK;
	}
	write_unlock(&lock);

	return SV_ERROR;
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

int svfile_dealwith_entry(void __user *args)
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
	"svfile_opts", SV_FRAME_PROTECT_FILE, svfile_dealwith_entry,
};

int file_init(void)
{
	svframe_register_kernel_syscall(&sv_file_ops);
	INIT_LIST_HEAD(&s_svfile_head.head);
	rwlock_init(&lock);
	return 0;
}

void file_fini(void)
{
	printk("file fini!!!!");
	return ;
}
