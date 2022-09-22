#include "kernel/types.h"//uint
#include "user/user.h"//write,sleep(int)
#include "kernel/stat.h"//stat
#include "kernel/fs.h"//dirent

char*
fmtname(char *path)//用处是提取一条路径上最后的那个文件
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), 0, DIRSIZ-strlen(p));//之前为什么填充' '   嗷，是从ls抄过来的，ls补空格是为了对齐
  return buf;
}

void find(char* path,char *filename){
    char *p,buf[512];
    int fd;
    struct stat st;
    struct dirent de;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if(fstat(fd, &st) < 0){//看看这条路径本身是什么“状态”（主要看是不是文件夹）
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch(st.type){
        case T_FILE:
            strcpy(buf, path);
            if(strcmp(fmtname(buf),filename)==0){
                printf("%s\n",buf);
            }
            break;
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof (buf)){
                printf("find: path too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf+strlen(buf);
            *p++ = '/';
             read(fd, &de, sizeof(de));//貌似是把. 和..读掉了
             read(fd, &de, sizeof(de));
            while(read(fd, &de, sizeof(de)) == sizeof(de)){
                if(de.inum == 0)
                    continue;//???什么意思,和隐藏文件夹有关吗
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                
                if(strcmp(fmtname(buf),filename)==0){
                    printf("%s\n",buf);
                }
                int temp_fd;
                if((temp_fd = open(buf, 0)) < 0){//错了，检查才能打开
                    fprintf(2, "find: cannot open %s\n", path);
                    return;
                }
                if(fstat(temp_fd, &st) < 0){//看看这条路径本身是什么“状态”（主要看是不是文件夹）
                    fprintf(2, "find: cannot stat %s\n", path);
                    close(temp_fd);
                    return;
                }
                if(st.type==T_DIR ){
                    //printf("going to find %s\n",fmtname(buf));
                    find(buf,filename);
                }
                close(temp_fd);
            }
    }
    return;
}
int
main(int argc, char *argv[])
{
    if(argc!=3)
        write(2,"find wrong\n",11);
    else
    	find(argv[1],argv[2]);
    exit(0);
}
