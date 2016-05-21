#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

char *srcroot;
char *dstroot;
extern void link_dir(char *sdir, char* ddir);
static int is_dir(char *dir)
{
	struct stat fs;

	if (stat(dir, &fs) != 0) {
		perror("stat");
		fprintf(stderr, "stat %s failed\n", dir);
		exit(1);
	}
	return S_ISDIR(fs.st_mode);
}

static int do_link(char *from, char *name, char *to)
{
	char source[1024];
	char dest[1024];
	char link_sdir[1024];
	char link_ddir[1024];
	char pwd[1024];

	getcwd(pwd,sizeof(pwd));

	snprintf(source, sizeof(source), "%s/%s", from, name);
	snprintf(dest, sizeof(dest), "%s/%s", to, name);
	if (is_dir(source)) {
		mkdir(dest, 0777);
		link_dir(source, dest);
	} else {
		snprintf(link_sdir, sizeof(link_sdir), "%s/%s",pwd,source);
		snprintf(link_ddir, sizeof(link_ddir), "%s/%s",pwd,dest);
		if (symlink(link_sdir, link_ddir) != 0) {
			if (errno != EEXIST) {
				fprintf(stderr, "softlink %s --> %s failed!\n", source, dest);
				exit(1);
			}
		} else {
			printf("softlinked %s\n", dest);
		}
	}
	return 0;
}

void clean_dir(char *ddir, int flags)
{
	char *p;
	DIR *dir;
	struct dirent *dirattr;
	char file[1024];
	struct stat fs;
	int count;

	p = strrchr(ddir, '/');
	if(p && *(p+1) == '.'){
		return ;
	}
	
	dir = opendir(ddir);
	if(!dir)
		return;

	count = 0;
	while((dirattr = readdir(dir)) != NULL){
		if(!strcmp(dirattr->d_name, ".")||
				!strcmp(dirattr->d_name, ".."))
			continue;
		count ++;
		snprintf(file, sizeof(file), "%s/%s", ddir, dirattr->d_name);
		if(stat(file,&fs) != 0){
			printf("remove %s, not exit\n",file);
			unlink(file);
			count--;
			continue;
		}
		if(S_ISDIR(fs.st_mode))
			clean_dir(file, 1);
	}
	closedir(dir);

	if(count == 1){
		snprintf(file, sizeof(file), "%s/Makefile", ddir);
		if(stat(file, &fs) == 0 && S_ISREG(fs.st_mode)){
			printf("remove %s, not used\n", file);
			unlink(file);
			printf("remove %s, empty\n",ddir);
			rmdir(ddir);
			return ;
		}
	}
	if(count == 0 && flags){
		printf("remove %s. empty\n",ddir);
		rmdir(ddir);
	}

	return;
}

void link_dir(char *sdir, char* ddir)
{
	DIR* dir;
	struct dirent *dirattr;

	dir = opendir(sdir);
	if(!dir){
		perror("opendir:");
		exit(1);
	}
	while((dirattr = readdir(dir)) != NULL){
		if(!strcmp(dirattr->d_name, ".")||
				!strcmp(dirattr->d_name, ".."))
			continue;

		if(!strcmp(dirattr->d_name, ".git")) {
			continue;
		}

		do_link(sdir, dirattr->d_name, ddir);
	}
	closedir(dir);
}

void start_link_dir(char *sdir, char* ddir)
{
	if(!sdir || !ddir){
		printf("src or dst is NULL\n");
		exit(1);
	}

	clean_dir(ddir, 0);
	link_dir(sdir, ddir);
}

int main(int argc, char **argv)
{

	srcroot = argv[1];
	dstroot = argv[2];

	start_link_dir(srcroot, dstroot);
	return 0;
}
