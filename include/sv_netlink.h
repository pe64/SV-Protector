#ifndef SV_NETLINK_H
#define SV_NETLINK_H

#include <sv_file.h>
#include <sv_process.h>

#define NETLINK_SV_PROTECT 19

typedef struct {
	union{
		int cid;
		int rid;
	};
	union {
		char pos;
		svfile_u2k_args_st file_args;
		svfile_k2u_ret_st file_ret;
		svproc_u2k_args_st proc_args;
		svproc_k2u_ret_st proc_ret;
	};
}svnetlink_nlmsg_st;

#endif
