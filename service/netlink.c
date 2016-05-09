#include <sys/socket.h>
#include <asm/types.h>
#include <linux/netlink.h>

int sk_netlink_fd;
struct sockaddr_nl nl_addr;

int init_netlink_socket(void)
{
	sk_netlink_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_PROTECT);
	if(sk_netlink_fd <= 0){
		sk_netlink_fd = 0;
		return -1;
	}

	nl_addr.nl_family = AF_NETLINK;
	nl_addr.nl_pid = getpid();

	if(bind(sk_netlink_fd, (struct sockaddr *)&nl_addr, sizeof(struct sockaddr_nl))){
		close(sk_netlink_fd);
		sk_netlink_fd = 0;
		return SQUID_ERROR;
	}
	nl_addr.nl_pid = 0;
	return 0;
}

struct nlmsghdr *build_netlink_msg(struct msghdr *msg, struct iovec *iov)
{
	struct nlmsghdr *nlh;

	nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(sizeof(sqnl_msg_st)));
	if(!nlh){
		return nlh;
	}

	bzero(nlh, NLMSG_SPACE(sizeof(sqnl_msg_st)));

	nlh->nlmsg_len = NLMSG_SPACE(sizeof(sqnl_msg_st));
	nlh->nlmsg_pid = getpid();

	iov->iov_base = nlh;
	iov->iov_len = nlh->nlmsg_len;

	msg->msg_name = &nl_addr;
	msg->msg_namelen = sizeof(struct sockaddr_nl);
	msg->msg_iov = iov;
	msg->msg_iovlen = 1;

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

