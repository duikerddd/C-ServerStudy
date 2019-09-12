//socket基本使用--服务端
#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include <arpa/inet.h>

using namespace std;

int main()
{
   //int socket(int domain, int type, int protocol);
  int sockfd = socket(AF_INET,SOCK_DGRAM,17);
  if(sockfd<0)
  {
    perror("sock error");
    return -1;
  }
  
  struct sockaddr_in local;
  local.sin_family=AF_INET;
  local.sin_port=htons(9000);
  local.sin_addr.s_addr=inet_addr("192.168.241.128");
  socklen_t len = sizeof(struct sockaddr_in);
  //用户端绑定地址信息
  //int bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
  bind(sockfd,(struct sockaddr*)&local,len);

  struct sockaddr_in cli_addr;
  socklen_t rlen=sizeof(struct sockaddr_in);
  while(1)
  {
    //size_t recvfrom(int sockfd, void *buf, size_t len, int flags,
    //              struct sockaddr *src_addr, socklen_t *addrlen);
    char buff[1024]={0};
    recvfrom(sockfd,buff,1023,0,(struct sockaddr*)&cli_addr,&rlen);
    cout<<"cli say:"<<buff<<endl;

    memset(buff,0x00,1023);
    //size_t sendto(int sockfd, const void *buf, size_t len, int flags,
    //           const struct sockaddr *dest_addr, socklen_t addrlen);
    cout<<"plase say:";
    fflush(stdout);
    scanf("%s",buff);
    sendto(sockfd,buff,1023,0,(struct sockaddr*)&cli_addr,rlen);

  }
  
  close(sockfd);
  return 0;
}
