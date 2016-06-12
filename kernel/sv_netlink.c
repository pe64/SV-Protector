#include <linux/ftrace.h>
#include <linux/kprobes.h>
#include <asm/unistd.h>
#include <net/netlink.h>
#include <net/inet_sock.h>

#include "sv_netlink.h"
#include "priv.h"

#define RCV_SKB_FAIL(err) do { netlink_ack(skb, nlh, (err)); return; } while (0)

static struct sock *sk;
static DEFINE_MUTEX(sv_mutex);

static int netlink_sendmsg(__u32 pid, const u8 *data, int data_len)
{
	struct nlmsghdr *rep;
	u8 *res;
	struct sk_buff *skb;

	if(pid < 0 || sk == NULL) {
		printk("Invalid parameter, g_pid = %d, g_nl_sk = %p\n", pid, sk);
		return -1;
	}

	skb = nlmsg_new(data_len, GFP_KERNEL);
	if(!skb) {
		printk("nlmsg_new failed!!!\n");
		return -1;
	}
	
	rep = __nlmsg_put(skb, pid, 0, NLMSG_NOOP, data_len, 0);
	res = nlmsg_data(rep);
	memcpy(res, data, data_len);
	netlink_unicast(sk, skb, pid, MSG_DONTWAIT);

	return 0;
}

static void __input(struct sk_buff *skb)
{
	int nlmsglen;
	int ret;
	struct nlmsghdr *nlh;
	svnetlink_nlmsg_st *data;

	nlh = nlmsg_hdr(skb);
	nlmsglen = nlh->nlmsg_len;
	if(nlmsglen < sizeof(*nlh) || skb->len < nlmsglen){
		RCV_SKB_FAIL(-EINVAL);
	}
	printk("file:%s,line:%d\n",__FILE__,__LINE__);

	data = (svnetlink_nlmsg_st *)NLMSG_DATA(nlh);

	ret = svframe_invoke_syscall(data->cid, (void *)&data->pos);
	netlink_sendmsg(NETLINK_CB(skb).pid, &ret, sizeof(ret));

}

static void input(struct sk_buff *skb)
{
	mutex_lock(&sv_mutex);
	__input(skb);
	mutex_unlock(&sv_mutex);
}

int sv_netlink_init(void)
{
	printk("hello netlink\n");

	sk = netlink_kernel_create(&init_net, NETLINK_SV_PROTECT, 1, input, NULL, THIS_MODULE);
	if(!sk){
		printk("netlink create error !!!!\n");
	}

	return 0;
}

void sv_netlink_fini(void)
{
	if(sk){
		netlink_kernel_release(sk);
	}
	printk("bye netlink!!!");
}


