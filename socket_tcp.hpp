//基于TCP的简单网络通信demo
//使用c++对tcp_socket进行封装
#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>   //atoi
#include<string>
#include<string.h>
#include<arpa/inet.h>  //htons inet_addr

class TcpSocket{
  public:
    TcpSocket():_sockfd(-1){}
    //1.创建套接字
    bool Socket()
    {
      _sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
      if(_sockfd<0)
      {
        std::cout<<"sock error";
        return false;
      }
      return true;
    }
    //2.绑定地址信息
    bool Bind(std::string& ip,uint16_t port)
    {
      sockaddr_in addr;
      addr.sin_family=AF_INET;
      addr.sin_port=htons(port);
      addr.sin_addr.s_addr=inet_addr(ip.c_str());

      int ret=bind(_sockfd,(sockaddr*)&addr,sizeof(sockaddr_in));
      if(ret!=0)
      {
        std::cout<<"bind error";
        return false;
      }
      return true;
    }
    //3.服务器监听--并设置同一时间最大连接数
    bool Listen(int backlog=5)
    {
      //int listen(int sockfd, int backlog);
      //backlog:最大连接数
      int ret=listen(_sockfd,backlog);
      if(ret<0)
      {
        std::cout<<"listen error";
        return false;
      }
      return true;
    }
    //3.客户端连接---向服务端发起连接请求
    bool Connect(std::string& ip,uint16_t port)
    {
      //int connect(int sockfd, const struct sockaddr *addr,
      //                   socklen_t addrlen);
      sockaddr_in des_addr;
      des_addr.sin_family=AF_INET;
      des_addr.sin_port=htons(port);
      des_addr.sin_addr.s_addr=inet_addr(ip.c_str());

      int ret=connect(_sockfd,(sockaddr*)&des_addr,sizeof(sockaddr_in));
      if(ret!=0)
      {
        std::cout<<"connect error";
        return false;
      }
      return true;
    }
    //4.服务器获取连接---获取已经连接成功的客户端,返回新的socket并保存客户端信息
    bool Accept(TcpSocket& new_sock,sockaddr_in *save_addr=NULL)
    {
      //int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
      sockaddr_in peer;
      socklen_t len=sizeof(sockaddr_in);
      int newfd=accept(_sockfd,(sockaddr*)&peer,&len);
      if(newfd<0)
      {
        std::cout<<"accept error";
        return false;
      }

      new_sock.SetFd(newfd);
      if(save_addr)
      {
        memcpy(save_addr,&peer,len);
      }
      return true;
    }
    //设置新sockfd
    void SetFd(int newfd)
    {
      _sockfd=newfd;
    }
    //收发数据
    bool Recv(std::string& buf)
    {
      char tmp[1024]={0};
      //size_t recv(int sockfd, void *buf, size_t len, int flags);
      int len=recv(_sockfd,tmp,1023,0);
      if(len<0)
      {
        std::cout<<"recv error";
        return false;
      }
      else if(len==0)
      {
        std::cout<<"peer shutdown";
        return false;
      }else{
        buf.assign(tmp,len);
        return true;
      }
    }
    bool Send(std::string& buf)
    {
      //int send(int sockfd, const void *msg, size_t len, int flags);
      int len=send(_sockfd,buf.c_str(),buf.size(),0);
      if(len<0)
      {
        std::cout<<"send error";
        return false;
      }
      return true;
    }
  private:
    int _sockfd;
};
