#include <unistd.h>
#include <sys/socket.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <string.h>

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

int sv_syscall_invoke(int cid, void *args, size_t size)
{
	struct nlmsghdr *nlh;
	struct msghdr msg;
	struct iovec iov;
	svnetlink_nlmsg_st *svnl_msg;

	bzero(&msg, sizeof(msg));
	bzero(&iov, sizeof(iov));
	nlh = build_netlink_msg(&msg, &iov);
	if(!nlh){
		return SV_ERROR;
	}	
	
	svnl_msg = (svnetlink_nlmsg_st *)NLMSG_DATA(nlh);
	svnl_msg->cid = cid;
	memcpy(&svnl_msg->pos, args, size);
	sendmsg(syscall_handle, &msg, 0);

}
