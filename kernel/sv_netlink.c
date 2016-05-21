#include <linux/ftrace.h>
#include <linux/kprobes.h>
#include <asm/unistd.h>
#include <net/netlink.h>
#include <net/inet_sock.h>
#include "sv_netlink.h"

#define RCV_SKB_FAIL(err) do { netlink_ack(skb, nlh, (err)); return; } while (0)

static struct sock *sk;
static DEFINE_MUTEX(sv_mutex);

static void __input(struct sk_buff *skb)
{
	int nlmsglen;
	struct nlmsghdr *nlh;

	nlh = nlmsg_hdr(skb);
	nlmsglen = nlh->nlmsg_len;
	if(nlmsglen < sizeof(*nlh) || skb->len < nlmsglen){
		RCV_SKB_FAIL(-EINVAL);
	}
	printk("file:%s,line:%d\n",__FILE__,__LINE__);

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

void netlink_fini(void)
{
	if(sk){
		netlink_kernel_release(sk);
	}
	printk("bye netlink!!!");
}
