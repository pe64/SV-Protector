#ifndef PRIV_H
#define PRIV_H

typedef int (*KERNEL_SYSCALL_PROC)(void *args);

typedef struct {
	char *name;
	int cid;
	KERNEL_SYSCALL_PROC proc;
}sv_syscall_ops_st;

typedef struct {
	struct list_head list;
	char name[256];
	ino_t inode;
	int index;
	int flags;
	pid_t write_pid;
	time_t timestamps;
}svfile_list_st;

extern int file_init(void);
extern void file_fini(void);
extern int process_init(void);
extern void process_fini(void);
extern int sv_netlink_init(void);
extern int svframe_invoke_syscall(int cid, void *args);
extern void svframe_register_kernel_syscall(sv_syscall_ops_st *node);
extern void sv_netlink_fini(void);

#endif
