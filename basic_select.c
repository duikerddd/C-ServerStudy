//循环从标准输入读取数据,并写到标准输出上
//使用selcet完成等待过程
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/select.h>

int main()
{
  //1.先把文件描述符集准备好
  while(1)
  {
    fd_set read_set; 
    FD_ZERO(&read_set);
    FD_SET(0,&read_set);
    //select(nfds,fd_set *read,fd_set *write,fd_set *exceptfds);
    int ret=select(1,&read_set,NULL,NULL,NULL);
    if(ret<0)
    {
      perror("select");
      return -1;
    }
    char buf[1024]={0};
    read(0,buf,1023);
    printf("stdin: %s\n",buf);
    
  }
  return 0;
}
