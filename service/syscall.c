#include <unistd.h>
#include <sys/socket.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sv_base.h>

#include "priv.h"
#define CHDEV_PATH "/dev/sv_chdev"

static int syscall_handle;

int sv_syscall_init(void)
{
	if(syscall_handle <= 0){
		syscall_handle = open(CHDEV_PATH, O_RDWR);
		if(syscall_handle <=0 ){
			return SV_ERROR;
		}
	}
	return SV_OK;
}

int sv_syscall_invoke(int syscall_id, void *args)
{
	int ret;
	
	if(syscall_handle <= 0){
		return SV_ERROR;
	}
	
	return ioctl(syscall_handle, syscall_id, args);
}

#if 0
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
	len = recvmsg(syscall_handle, &msg, 0);
	while(NLMSG_OK(nlh, len)){
		if (nlh->nlmsg_type == NLMSG_DONE) {
			puts("nh->nlmsg_type == NLMSG_DONE");
			return;
		}
		if (nlh->nlmsg_type == NLMSG_ERROR) {
			// Do some error handling.
			puts("nh->nlmsg_type == NLMSG_ERROR");
			return;
		}
		nlh = NLMSG_NEXT(nlh, len);
	}
}
static void recv_from_nl()
{

	char buf[1000];
	int len;
	struct iovec iov = {buf, sizeof(buf)};
	struct sockaddr_nl sa;
	struct msghdr msg;
	struct nlmsghdr *nh;

	memset(&msg, 0, sizeof(msg));
	msg.msg_name = (void *)&sa;
	msg.msg_namelen = sizeof(sa);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	//len = recvmsg(nl_sock, &msg, 0);

	len = recvmsg(nl_sock, &msg, 0);

	for (nh = (struct nlmsghdr *)buf; NLMSG_OK(nh, len);
			nh = NLMSG_NEXT(nh, len)) {
		// The end of multipart message.
		if (nh->nlmsg_type == NLMSG_DONE) {
			puts("nh->nlmsg_type == NLMSG_DONE");
			return;
		}
		if (nh->nlmsg_type == NLMSG_ERROR) {
			// Do some error handling.
			puts("nh->nlmsg_type == NLMSG_ERROR");
			return;
		}
#if 1
		puts("Data received from kernel:");
		hex_dump((u8*)NLMSG_DATA(nh), NLMSG_PAYLOAD(nh, 0));

#endif
	}

}
#endif
