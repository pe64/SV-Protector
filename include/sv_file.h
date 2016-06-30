#ifndef SV_FILE_H
#define SV_FILE_H

//#include <linux/list.h>
#ifdef SVFILE_KERNEL
#include <linux/types.h>
#include <linux/coda.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

enum {
	SV_FILE_REQ_START,
	SV_FILE_REQ_ADD,
	SV_FILE_REQ_DEL,
	SV_FILE_REQ_LIST,
	SV_FILE_REQ_MAX,
};

typedef struct {
	int fd;
	char file_name[256];
	ino_t inode;
	char password[256];
	int flags;
}svfile_set_st;

typedef struct {
	char file_name[256];
	ino_t inode;
	void *next;
}svfile_get_st;

#endif
