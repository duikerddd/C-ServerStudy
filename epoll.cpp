//基于epoll的TCP回显服务器
#include<vector>
#include<stdio.h>
#include<string>
#include<iostream>
#include<sys/socket.h>
#include<sys/select.h>
#include<netinet/in.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include"tcp.hpp"

class Epoll{
  public:
    //初始化
    Epoll():_epoll_fd(-1){}
    //1.初始化
    int EpollCreate()
    {
      _epoll_fd=epoll_create(10);
      return _epoll_fd;
    }
    //输入
    int EpollAdd(TcpSocket& sock)
    {
      int fd=sock.GetFd();
      epoll_event event;
      event.events=EPOLLIN;
      event.data.fd=fd;
      int ret=epoll_ctl(_epoll_fd,EPOLL_CTL_ADD,fd,&event);
      if(ret<0)
      {
        perror("epoll_ctl");
        return -1;
      }
      return 0;
    }
    int EpollDel(TcpSocket& sock)
    {
      int fd=sock.GetFd();
      int ret=epoll_ctl(_epoll_fd,EPOLL_CTL_DEL,fd,NULL);
      if(ret<0)
      {
        perror("epoll_ctl");
        return -1;
      }
      return 0;
    }
    //输出
    int EpollWait(std::vector<TcpSocket>& output)
    {
      output.clear();
      epoll_event output_event[100];
      //int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
      int ret=epoll_wait(_epoll_fd,output_event ,100,-1);
      if(ret<0){
        perror("epoll_wait");
        return -1;
      }
      for(int i=0;i<ret;++i)
      {
        TcpSocket sock(output_event[i].data.fd);
        output.push_back(sock);
      }
      return ret;
    }
  private:
    int _epoll_fd;
};

int main(int argc,char* argv[]){
  if(argc!=3)
  {
    perror("./epoll ip port");
    return 1;
  }
  //1.服务器初始化
  TcpSocket listen_sock; 
  listen_sock.Socket();
  std::string ip=argv[1];
  uint16_t port=atoi(argv[2]);
  listen_sock.Bind(ip,port);
  listen_sock.Listen();

  //2.epoll初始化,监听listen_sock
  Epoll epoll;
  epoll.EpollCreate();
  epoll.EpollAdd(listen_sock);

  printf("Server Ok\n");

  //3.进入循环
  while(1)
  {
    //等待,输出
    std::vector<TcpSocket> output;
    output.reserve(100);
    int ret=epoll.EpollWait(output);
    if(ret<0) continue;
    for(int i=0;i<output.size();++i)
    {
      //如果是listen_sock,调用accept,添加监视new_sock
      if(output[i].GetFd()==listen_sock.GetFd())
      {
        TcpSocket new_sock;
        listen_sock.Accept(new_sock);
        epoll.EpollAdd(new_sock);
        printf("cli %d is connected\n",new_sock.GetFd());
      }
      //如果是读就绪,读写一次
      else {
        char buf[1023]={0};
        int read_size=read(output[i].GetFd(),buf,1023);
        if(read_size<0)
        {
          perror("read");
          return -1;
        }
        else if(read_size==0)
        {
          printf("cli %d disconnected!\n",output[i].GetFd());
          epoll.EpollDel(output[i]);
          close(i);
          continue;
        }
        printf("cli %d say:%s\n",output[i].GetFd(),buf);
        write(output[i].GetFd(),buf,1023);
      }
    }

  }
  return 0;
}
