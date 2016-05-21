#ifndef PRIV_H
#define PRIV_H

extern int file_init(void);
extern void file_fini(void);
extern int process_init(void);
extern void process_fini(void);

typedef int (*KERNEL_SYSCALL_PROC)(void *args);

typedef struct {
	char *name;
	int cid;
	KERNEL_SYSCALL_PROC proc;
}sv_syscall_ops_st;


#endif
