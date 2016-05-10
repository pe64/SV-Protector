#include <sys/socket.h>
#include <asm/types.h>
#include <linux/netlink.h>

#define BIND_NLADDR_FLAGS 0
#define SEND_NLADDR_FLAGS 1

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
	
	if(!addr){
		return 0;
	}

	sk_netlink_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_SV_PROTECT);
	if(sk_netlink_fd <= 0){
		return sk_netlink_fd;
	}

	if(bind(sk_netlink_fd, (struct sockaddr *)nl_addr, sizeof(struct sockaddr_nl))){
		close(sk_netlink_fd);
		sk_netlink_fd = 0;
	}

	return sk_netlink_fd;
}

struct nlmsghdr *build_netlink_msg(struct msghdr *msg, struct iovec *iov)
{
	struct nlmsghdr *nlh;
	struct sockaddr_nl *send_addr;
	
	nlh = NULL;

	send_addr = init_sockaddr_nl_object(SEND_NLADDR_FLAGS);
	if(!send_addr){
		goto out;
	}

	nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(sqnl_msg_st)));
	if(!nlh){
		goto out;
	}

	bzero(nlh, NLMSG_SPACE(sizeof(sqnl_msg_st)));

	nlh->nlmsg_len = NLMSG_SPACE(sizeof(sqnl_msg_st));
	nlh->nlmsg_pid = getpid();

	iov->iov_base = nlh;
	iov->iov_len = nlh->nlmsg_len;

	msg->msg_name = nl_addr;
	msg->msg_namelen = sizeof(struct sockaddr_nl);
	msg->msg_iov = iov;
	msg->msg_iovlen = 1;
out:
	if(send_addr)
		free(send_addr);
	return nlh;
}


int deal_with(Comm::ConnectionPointer client, int server_fd)
{
	socklen_t addr_len;
	struct nlmsghdr *nlh;
	struct iovec iov;
	struct msghdr msg;
	sqnl_msg_st *sqnl;
	int ret;

	if(!sk_netlink_fd){
		ret = init_netlink_socket();
		if(ret == SQUID_ERROR){
			return ret;
		}
	}

	bzero(&msg, sizeof(struct msghdr));
	bzero(&iov, sizeof(struct iovec));

	addr_len = sizeof(struct sockaddr_in);

	nlh = build_netlink_msg(&msg, &iov);
	if(!nlh){
		return SQUID_ERROR;
	}

	sqnl = (sqnl_msg_st *)NLMSG_DATA(nlh);

	getsockname(server_fd, (struct sockaddr *)&sqnl->server_local_addr, &addr_len);
	getpeername(server_fd, (struct sockaddr *)&sqnl->server_peer_addr, &addr_len);
	client->remote.getSockAddr(sqnl->client_peer_addr);
	client->local.getSockAddr(sqnl->client_local_addr);

	printf("[%s][%d]-[%s]\n",__FILE__,__LINE__,__func__);
	printf("s_local_addr = %s:%d\n",inet_ntoa(sqnl->server_local_addr.sin_addr),ntohs(sqnl->server_local_addr.sin_port));
	printf("s_peer_addr = %s:%d\n",inet_ntoa(sqnl->server_peer_addr.sin_addr),  ntohs(sqnl->server_peer_addr.sin_port));
	printf("c_peer_addr = %s:%d\n",inet_ntoa(sqnl->client_peer_addr.sin_addr),  ntohs(sqnl->client_peer_addr.sin_port));
	printf("c_local_addr = %s:%d\n",inet_ntoa(sqnl->client_local_addr.sin_addr),ntohs(sqnl->client_local_addr.sin_port));
	printf("[%s][%d]-[%s]\n",__FILE__,__LINE__,__func__);

	sendmsg(sk_netlink_fd, &msg, 0);
	printf("[%s][%d]-[%s]\n",__FILE__,__LINE__,__func__);
	return SQUID_OK;
}

