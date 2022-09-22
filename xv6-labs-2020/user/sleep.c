#include "kernel/types.h"//uint
#include "user/user.h"//write,sleep(int)
int
main(int argc, char *argv[])
{
    int sec=0;
    if(argc<=1){
        write(2,"usage: sleep seconds",20);
        exit(1);
    }
    if(argc==2){
        sec=atoi(argv[1]);
        sleep(sec);
    }
    exit(0);
}
