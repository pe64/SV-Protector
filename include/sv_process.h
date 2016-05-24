#ifndef SV_PROCESS_H
#define SV_PROCESS_H

typedef struct {
	char proc_name[256];
	pid_t pid;
	int flags;
}svproc_u2k_args_st;

typedef struct {
	int err;
	char proc_name[256];
	int index;
}svproc_k2u_ret_st;

#endif 
