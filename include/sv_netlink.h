#ifndef SV_NETLINK_H
#define SV_NETLINK_H

#include <sv_file.h>
#include <sv_process.h>

#define NETLINK_SV_PROTECT 19

typedef struct {
	int cid;
	union {
		char pos;
		svfile_u2k_args_st file_args;
		svproc_u2k_args_st proc_args;
	};
}svnetlink_nlmsg_st;

#endif
