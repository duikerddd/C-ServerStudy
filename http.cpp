//实现一个简单的http服务端程序
//功能:不管请求是什么,都返回 kono dio da!
//http协议传输层用的是tcp协议
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<pthread.h>
#include<errno.h>
typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;


class Server{
  public:
    Server(std::string ip,int64_t port):_ip(ip),_port(port) 
    {}
    //初始化服务器 创建socket,绑定地址信息并开始监听
    int Init()
    {
      int fd=socket(AF_INET,SOCK_STREAM,6);
      if(fd<0)
      {
        perror("socket");
        return -1;
      }
      sockaddr_in addr;
      addr.sin_family=AF_INET;
      addr.sin_port=htons(_port);
      addr.sin_addr.s_addr=inet_addr(_ip.c_str());
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
    
  private:
    std::string _ip;
    int64_t _port;
};


void* ThreadEntry(void* arg)
{
  long new_sock=(long)arg;
  //1.接收客户端信息并解析,这里不解析
  char buf[1024*10]={0};
  read(new_sock,buf,sizeof(buf)-1);
  printf("[Request] %s\n",buf);

  //2.构造一个helloworld响应字符串,并写回客户端
  const char* first_line="HTTP/1.1 200 OK\n";
  const char* blank_line="\n";
  const char* body="<html>kono dio da !</html>";
  char header[1024*10]={0};
  sprintf(header,"Content-Type: text/html;\nContent-Length: %lu\n",strlen(body));
  write(new_sock,first_line,strlen(first_line));
  write(new_sock,header,strlen(header));
  write(new_sock,blank_line,strlen(blank_line));
  write(new_sock,body,strlen(body));
  return NULL;
}

int main(int argc,char* argv[])
{
  if(argc!=3)
  {
    printf("./http [ip] [port]");
    return 1;
  }

  Server server(argv[1],atoi(argv[2]));
  int listen_sock = server.Init();
  printf("ServerInit Ok\n");

  while(1)
  {
    sockaddr_in peer;
    socklen_t len=sizeof(peer);
    long new_sock=accept(listen_sock,(sockaddr*)&peer,&len);
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
