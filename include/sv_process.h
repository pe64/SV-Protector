#ifndef SV_PROCESS_H
#define SV_PROCESS_H

typedef struct {
	char proc_name[256];
	pid_t pid;
	int flags;
}svproc_u2k_args_st;

#endif 
