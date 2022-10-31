#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
main() {
	printf("UID : %d\n",getuid());
	printf("EUID : %d\n",geteuid());
	printf("GID : %d\n",getgid());
	printf("EGID : %d\n",getegid());
}
