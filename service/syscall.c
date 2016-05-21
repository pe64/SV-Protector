#include <unistd.h>

#include <sv_netlink.h>
#include <sv_base.h>

#include "priv.h"

static int syscall_handle;

int sv_syscall_init(void)
{
	struct sockaddr_nl *addr;
	
	addr = init_sockaddr_nl_object(SEND_NLADDR_FLAGS);
	if(!addr){
		return SV_ERROR;
	}

	syscall_handle = init_netlink_socket(addr);
	if(syscall_handle <= 0){
		return SV_ERROR;
	}
	
	return SV_OK;
}
