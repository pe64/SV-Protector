#ifndef SV_FILE_H
#define SV_FILE_H

#include <linux/list.h>
#include <linux/coda.h>

typedef struct {
	char file_name[256];
	ino_t inode;
	pid_t pid;
	int flags;
}svfile_u2k_args_st;

typedef struct {
	int err;
	char file_name[256];
	ino_t inode;
	int index;
}svfile_k2u_ret_st;

typedef struct {
	struct list_head list;
	char name[256];
	ino_t inode;
	int index;
	int flags;
	pid_t write_pid;
	time_t timestamps;
}svfile_list_st;

#endif
