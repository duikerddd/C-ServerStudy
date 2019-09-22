//实现一个简单的http服务端程序
//功能:不管请求是什么,都跳转到baidu
//http协议传输层用的是tcp协议
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<pthread.h>
#include<errno.h>
typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;


int ServerInit(const char* ip,uint16_t port)
{
  int fd=socket(AF_INET,SOCK_STREAM,6);
  if(fd<0)
  {
    perror("socket");
    return -1;
  }
  sockaddr_in addr;
  addr.sin_family=AF_INET;
  addr.sin_port=htons(port);
  addr.sin_addr.s_addr=inet_addr(ip);
  int ret=bind(fd,(sockaddr*)&addr,sizeof(addr));
  if(ret<0)
  {
    perror("bind");
    return -1;
  }
  ret=listen(fd,5);
  if(ret<0)
  {
    perror("listen");
    return -1;
  }
  return fd;
}

void* ThreadEntry(void* arg)
{
  long new_sock=(long)arg;
  //1.接收客户端信息并解析,这里不解析
  char buf[1024*10]={0};
  read(new_sock,buf,sizeof(buf)-1);
  printf("[Request] %s\n",buf);

  const char* first_line="HTTP/1.1 302 Found\n";
  const char* blank_line="\n";
  char header[1024*10]={0};
  sprintf(header,"Location: http://www.baidu.com\nContent-Length: 0\n");
  write(new_sock,first_line,strlen(first_line));
  write(new_sock,header,strlen(header));
  write(new_sock,blank_line,strlen(blank_line));
  return NULL;
}

int main(int argc,char* argv[])
{
  if(argc!=3)
  {
    printf("./http [ip] [port]");
    return 1;
  }
  
  int listen_sock=ServerInit(argv[1],atoi(argv[2]));
  if(listen_sock<0)
  {
    perror("ServerInit Failed\n");
    return -1;
  }
  printf("ServerInit Ok\n");

  while(1)
  {
    sockaddr_in peer;
    socklen_t len=sizeof(peer);
    int new_sock=accept(listen_sock,(sockaddr*)&peer,&len);
    if(new_sock<0)
    {
      perror("accept"); 
      continue;
    }
    pthread_t tid;
    pthread_create(&tid,NULL,ThreadEntry,(void*)new_sock);
    pthread_detach(tid);
  }
  return 0;
}
