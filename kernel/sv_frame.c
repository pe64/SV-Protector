#include <linux/kernel.h>
#include <sv_frame.h>

#include "priv.h"

sv_syscall_ops_st *s_syscall_table[SV_FRAME_SYSCALL_MAX];

void svframe_register_kernel_syscall(sv_syscall_ops_st *node)
{
	if(node->syscall_id >= 0 && node->syscall_id < SV_FRAME_SYSCALL_MAX)
		s_syscall_table[node->syscall_id] = node;
	
	return;
}

int svframe_invoke_syscall(int syscall_id, void *args)
{
	if(syscall_id >= 0 && syscall_id < SV_FRAME_SYSCALL_MAX){
		return s_syscall_table[syscall_id]->proc(args);
	}

	return SV_ERR_SYSCALL_ID_ERROR;
}
