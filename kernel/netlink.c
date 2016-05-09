#include <linux/ftrace.h>
#include <linux/kprobes.h>
#include <asm/unistd.h>
#include <net/netlink.h>



void input(struct sk_buff *skb)
{
	mutex_lock(&sv_mutex);
	__input(skb);
	mutex_unlock(&sv_mutex);
}

struct sock *sk;

int netlink_init(void)
{
	printk("hello netlink");
	
	sk = netlink_kernel_create(&init_net, NETLINK_SV_PROTECT, 1, input, NULL, THIS_MODULE);
	if(!sk){
		printk("netlink create error !!!!");
	}

	return 0;
}

void netlink_fini(void)
{
	printk("bye netlink!!!");
}
