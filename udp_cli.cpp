
//socket基本使用--客户端
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
  int sockfd = socket(AF_INET,SOCK_DGRAM,17);
  if(sockfd<0)
  {
    perror("sock error");
    return -1;
  }
  //客户端不用绑定 
  struct sockaddr_in server;
  server.sin_family=AF_INET;
  server.sin_port=htons(9000);
  server.sin_addr.s_addr=inet_addr("192.168.241.128");

  struct sockaddr_in source;

  while(1)
  {
    socklen_t len = sizeof(struct sockaddr_in);
    char buff[1024]={0};
    cout<<"please say : ";
    cin>>buff;
    sendto(sockfd,buff,1023,0,(struct sockaddr*)&server, len);
    fflush(stdout);

    memset(buff,0x00,1023);
    recvfrom(sockfd,buff,1023,0,(struct sockaddr*)&source, &len);
    cout<<"server say:"<<buff<<endl;

  }
  //5.关闭套接字
  close(sockfd);
  return 0;
}
