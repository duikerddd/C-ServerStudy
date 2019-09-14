//基于udp简单网络通信
#include<iostream>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>   //atoi
#include<string>
#include<string.h>
#include<arpa/inet.h>  //htons inet_addr
#include<netinet/in.h> //sockaddr_in
#define CHECK_RET(RET) if(RET==false){return false;}
using namespace std;

class UdpSocket{
  public:
    UdpSocket():_sockfd(-1){}
    //1.创建套接字
    bool Socket(){
      _sockfd=socket(AF_INET,SOCK_DGRAM,17); 
      if(_sockfd<0)
      {
        perror("socket error");
        return false;
      }
    }

    //2.绑定地址信息
    bool Bind(string &ip,uint16_t port){
      sockaddr_in addr;
      addr.sin_family=AF_INET;
      addr.sin_port=htons(port);
      addr.sin_addr.s_addr=inet_addr(ip.c_str()); 
      socklen_t len=sizeof(sockaddr_in);
      int ret=bind(_sockfd,(sockaddr*)&addr,len);
      if(ret!=0)
      {
        perror("bind error");
        return false;
      }
      return true;
    }
    //3.接收消息
    bool Recv(string &buff,sockaddr_in* save_addr=NULL){
      char tmp[1024]={0};
      sockaddr_in raddr;
      socklen_t rlen=sizeof(sockaddr_in);
      int len=recvfrom(_sockfd,tmp,1023,0,(sockaddr*)&raddr,&rlen);
      if(len<0)
      {
        cout<<"recv error";
        return false;
      }
      buff.assign(tmp,len);
      if(save_addr!=NULL)
      {
        memcpy(save_addr,&raddr,rlen);
      }
      return true;
    }
    //4.发送消息
    bool Send(string &buff,sockaddr_in &des_addr){
      size_t len = buff.size();
      socklen_t slen=sizeof(sockaddr_in);
      int ret=sendto(_sockfd,buff.c_str(),len,0,(sockaddr*)&des_addr,slen);
      if(ret<0)
      {
        cout<<"send error";
        return false;
      }
      return true;
    }
    //5.关闭套接字
    bool Close(){
      if(_sockfd>0)
      {
        close(_sockfd);
        _sockfd=-1;
      }
      return true;
    }
  private:
    int _sockfd;
};
