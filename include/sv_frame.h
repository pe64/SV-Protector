#ifndef SV_FRAME_H
#define SV_FRAME_H

enum {
	SV_CID_ADD_PROTECT_FILE,
	SV_CID_MAX,
};

enum {
	SV_RID_ADD_PROTECT_FILE,
	SV_RID_MAX,
};

enum {
	SV_ERR_SYSCALL_ID_ERROR,
};

static char *s_err_table[] = {
	"syscall id error!!",
};

#endif

