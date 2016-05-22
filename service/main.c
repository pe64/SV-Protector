#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "sv_file.h"
#include "sv_base.h"
#include "sv_frame.h"

int main(int argc, char **argv)
{
	int ret;
	svfile_u2k_args_st file;

	if(sv_syscall_init()){
		printf("init syscall error!!!");
	}
	
	bzero(&file, sizeof(file));
	strcpy(file.file_name, "test_add_file");
	file.pid = getpid();
	file.flags = 1;

	sv_syscall_invoke(SV_CID_ADD_PROTECT_FILE, &file, sizeof(file));
	
	return 0;
}
