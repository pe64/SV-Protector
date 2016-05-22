#include <linux/kernel.h>
#include <sv_frame.h>

#include "priv.h"

sv_syscall_ops_st *s_syscall_table[SV_CID_MAX];

void svframe_register_kernel_syscall(sv_syscall_ops_st *node)
{
	if(node->cid >= 0 && node->cid < SV_CID_MAX)
		s_syscall_table[node->cid] = node;
	
	return;
}

int svframe_invoke_syscall(int cid, void *args)
{
	if(cid >= 0 && cid < SV_CID_MAX)
		return s_syscall_table[cid]->proc(args);
	
	printk("[%s][%d],cid error cid[%d]",__FILE__,__LINE__,cid);
	return SV_ERR_SYSCALL_ID_ERROR;
}
