//服务端--线程版,分离线程,自动回收
#include"socket_tcp.hpp"
#include<pthread.h>

void* thr_start(void* arg)
{
  TcpSocket* cli=(TcpSocket*)arg;
  while(1){
    std::string buf;
    cli->Recv(buf);
    std::cout<<"cli say:"<<buf<<std::endl;

    buf.clear(); 
    std::cout<<"srv say:";
    fflush(stdout);
    getline(std::cin,buf);
    cli->Send(buf);
  }
  delete cli;
  return NULL;
}



int main(int argc,char* argv[])
{
  if(argc!=3)
  {
    std::cout<<"./srv ip port";
    return -1;
  }
  std::string ip= argv[1];
  uint16_t port=atoi(argv[2]);

  TcpSocket acp_sock;
  acp_sock.Socket();
  acp_sock.Bind(ip,port);
  acp_sock.Listen(); 

  while(1)
  {
    //TcpSocket cli_sock; 因为分配的对象是在栈上的,循环结束就会自动回收资源,
    //所以需要手动在堆上分配空间
    TcpSocket* cli_sock=new TcpSocket;
    if(acp_sock.Accept(*cli_sock)==false) {continue;}
    pthread_t tid; 
    pthread_create(&tid,NULL,thr_start,(void*)cli_sock);
    pthread_detach(tid);
  }
  acp_sock.~TcpSocket();
  return 0;
}
