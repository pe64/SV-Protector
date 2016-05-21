#ifndef SV_NETLINK_H
#define SV_NETLINK_H

#define NETLINK_SV_PROTECT 21

typedef struct {
	int cid;
	void *args;
}svnetlink_nlmsg_st;

#endif
