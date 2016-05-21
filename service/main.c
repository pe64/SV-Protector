#include <stdio.h>

int main(int argc, char **argv)
{
	int ret;

	if(sv_syscall_init()){
		printf("init syscall error!!!");
	}

	return 0;
}
