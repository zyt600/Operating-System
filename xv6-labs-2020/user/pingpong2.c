
#include "kernel/types.h"//uint
#include "user/user.h"//write,sleep(int)
int
main(int argc, char *argv[])
{
	int p[2];
	char buf[5];
	pipe(p);
	
	if(fork()==0){
		read(p[0],buf,5);
		int pid=getpid();
		printf("%d: received %s\n",pid,buf);
		write(p[1],"pong",5);
	}
	else{
		write(p[1],"ping",5);
		wait(0);
		read(p[0],buf,5);
		int pid=getpid();
		printf("%d: received %s\n",pid,buf);
	}
	exit(0);
}
