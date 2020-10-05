//实现非阻塞IO,轮询方式读取标准输入
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>


void SetNoBlock(int fd)
{
  //int fcntl(int fd,int cmd,.../*args*/);
  //获取文件状态标记
  int flag=fcntl(fd,F_GETFL);
  if(flag<0)
  {
    perror("fcntl");
    return;
  }
  //设置文件描述符标记
  fcntl(fd,F_SETFL,flag|O_NONBLOCK);
}

int main()
{
  SetNoBlock(0);
  while(1)
  {
    char buf[1024]={0};
    int read_size=read(0,buf,1023);
    if(read_size<0)
    {
      perror("read");
      sleep(1);
      continue;
    }
    printf("stdin say : %s\n",buf);
  }

  return 0;
}
