#include <asm/unistd.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#include "sv_base.h"
#include "sv_file.h"
#include "sv_frame.h"
#include "priv.h"

const struct file_operations *get_file_operations(char *path)
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
	printk("[%s][%d] file[%s]\n",__FILE__,__LINE__, args->file_name);
	return SV_OK;
}

sv_syscall_ops_st file_ops = {
	"add_file", SV_CID_ADD_PROTECT_FILE, svfile_add_protect_file
};

int file_init(void)
{

	printk("file init!!!");
	svframe_register_kernel_syscall(&file_ops);

	return 0;
}

void file_fini(void)
{
	printk("file fini!!!!");
	return ;
}
