#include "kernel/types.h"//uint
#include "user/user.h"//write,sleep(int)
void prime(int read_file_descriptor){
    int p[2];
    int num,first;
    pipe(p);
    read(read_file_descriptor,&first,4);
    printf("prime %d\n",first);
    while(read(read_file_descriptor,&num,4)!=0)
        if(num%first!=0)
            write(p[1],&num,4);
    close(p[1]);
    close(read_file_descriptor);//?
    if(fork()==0){
        int p_child[2];
        read(p[0],&first,4);
        printf("prime %d\n",first);
        if(first<31){
            pipe(p_child);
            // prime(p_child[0]);//要不要出于防止继承太多pipe考虑放后面
            while(read(p[0],&num,4)!=0)
                if(num%first!=0)
                    write(p_child[1],&num,4);
            close(p_child[1]);
            prime(p_child[0]);//要不要出于防止继承太多pipe考虑放后面（好像是要的，不然这里面的p_child[1]就没被关
            close(p_child[0]);//?
        }
        close(p[0]);//?
        exit(0);
    }
    close(p[0]);//?
    while(wait(0)!=-1){
        continue;
    }
    exit(0);
    //return;
}
int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    printf("prime 2\n");
	// prime(p[0]);
    for(int i=3;i<=35;i++){
        int temp=i;
        if(temp%2!=0)
            write(p[1],&temp,4);
    }
    close(p[1]);
	prime(p[0]);
	close(p[0]);//?
	while(wait(0)!=-1){
        continue;
    }
    exit(0);
}

