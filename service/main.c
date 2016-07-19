#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fs.h>

#include "sv_file.h"
#include "sv_base.h"
#include "sv_frame.h"

int main(int argc, char **argv)
{
	int ret;
	sv_kernel_req_st req;
	struct stat st;
	int fd;
	int arg = FS_IMMUTABLE_FL;

	bzero(&st, sizeof(st));
	bzero(&req, sizeof(sv_kernel_req_st));
	req.module_cmd = SV_FILE_REQ_ADD;
	fd = open("a",O_RDWR);
	req.set.fd = fd;
	fstat(req.set.fd, &st);
	req.set.inode = st.st_ino;
	strcpy(req.set.password, "wxfqwer");

	sv_syscall_init();
	sv_syscall_invoke(SV_FRAME_PROTECT_FILE, &req);
	ioctl(fd, FS_IOC_SETFLAGS, &arg);

	return 0;
}
