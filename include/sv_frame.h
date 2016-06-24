#ifndef SV_FRAME_H
#define SV_FRAME_H
#include "sv_file.h"
typedef struct {
	int module_cmd;
	union{
		char pos;
		svfile_get_st get;
		svfile_set_st set;
	};
}sv_kernel_req_st;

enum {
	SV_ERR_SYSCALL_ID_ERROR,
};

enum {
	SV_FRAME_SYSCALL_START,
	SV_FRAME_PROTECT_FILE,
	SV_FRAME_PROTECT_PROCESS,
	SV_FRAME_SYSCALL_MAX,
};

#endif

