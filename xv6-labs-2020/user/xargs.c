#include "kernel/types.h"//uint
#include "user/user.h"//write,sleep(int)
#include"kernel/param.h"//MAXARG

int main(int argc, char *argv[])
{
    char buf[512],buf_char,*exec_argv[MAXARG+1];
    int exec_argc=0;
    buf_char=0;
    for(int i=0;i<512;i++)
        buf[i]=0;
    for(int i=0;i<=MAXARG;i++)
        exec_argv[i]=0;
    for(;exec_argc<argc-1;exec_argc++){
        exec_argv[exec_argc]=argv[exec_argc+1];
    }
    // read(0,buf,sizeof(buf));
    int offset_buf=0;
    while(read(0,&buf_char,1)==1){
        if(buf_char=='\n'){
            buf[offset_buf]=0;
            if(fork()==0){
                exec_argv[exec_argc]=buf;
                exec(argv[1],exec_argv);
            }
            else
                wait(0);
            offset_buf=0;
        }
        else if(buf_char==0){
            buf[offset_buf]=0;
            if(fork()==0){
                exec_argv[exec_argc]=buf;
                exec(argv[1],exec_argv);
            }
            else
                wait(0);
            offset_buf=0;
        }
        else{
            buf[offset_buf++]=buf_char;
        }
    }
    exit(0);
}
