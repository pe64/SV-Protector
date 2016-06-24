#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include "sv_netlink.h"

#include "priv.h"

struct sockaddr_nl *
init_sockaddr_nl_object(int flags)
{
	struct sockaddr_nl *addr;

	addr = (struct sockaddr_nl *)malloc(sizeof(struct sockaddr_nl));
	if(!addr){
		return addr;
	}

	addr->nl_family = AF_NETLINK;
	if(flags == BIND_NLADDR_FLAGS){
		addr->nl_pid = getpid();
	}else{
		addr->nl_pid = 0;	
	}
	return addr;
}

int init_netlink_socket(struct sockaddr_nl *nl_addr)
{
	int sk_netlink_fd;
	
	if(!nl_addr){
		printf("file:%s,line:%d\n",__FILE__,__LINE__);
		return 0;
	}

	sk_netlink_fd = socket(AF_NETLINK, SOCK_RAW, 19);//NETLINK_SV_PROTECT);
	if(sk_netlink_fd <= 0){
		perror("scoket");
		return sk_netlink_fd;
	}

	if(bind(sk_netlink_fd, (struct sockaddr *)nl_addr, sizeof(struct sockaddr_nl))){
		close(sk_netlink_fd);
		sk_netlink_fd = 0;
	}

	printf("file:%s,line:%d\n",__FILE__,__LINE__);
	return sk_netlink_fd;
}

struct nlmsghdr *
build_netlink_msg(struct msghdr *msg, struct iovec *iov)
{
	struct nlmsghdr *nlh;
	struct sockaddr_nl *send_addr;
	
	nlh = NULL;

	send_addr = init_sockaddr_nl_object(SEND_NLADDR_FLAGS);
	if(!send_addr){
		goto out;
	}

	nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(svnetlink_nlmsg_st)));
	if(!nlh){
		goto out;
	}

	bzero(nlh, NLMSG_SPACE(sizeof(svnetlink_nlmsg_st)));

	nlh->nlmsg_len = NLMSG_SPACE(sizeof(svnetlink_nlmsg_st));
	nlh->nlmsg_pid = getpid();

	iov->iov_base = nlh;
	iov->iov_len = nlh->nlmsg_len;

	msg->msg_name = send_addr;
	msg->msg_namelen = sizeof(struct sockaddr_nl);
	msg->msg_iov = iov;
	msg->msg_iovlen = 1;
	return nlh;
out:
	if(nlh)
		free(nlh);
	if(send_addr)
		free(send_addr);
	return NULL;
}

