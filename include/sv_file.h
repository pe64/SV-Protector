#ifndef SV_FILE_H
#define SV_FILE_H

//#include <linux/list.h>
#include <linux/types.h>
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


#endif
