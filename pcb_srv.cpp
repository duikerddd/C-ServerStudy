//用子进程和信号实现不阻塞和资源回收demo
#include"socket_tcp.hpp"
#include<signal.h>
#include<sys/wait.h>

void sigcb(int nu)
{
  while(waitpid(-1,NULL,WNOHANG)>0);
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

  signal(SIGCHLD,sigcb);
  TcpSocket acp_sock;
  acp_sock.Socket();
  acp_sock.Bind(ip,port);
  acp_sock.Listen(); 

  while(1)
  {
    TcpSocket cli_sock;
    //先接受数据,再在子进程里收发数据
    if(acp_sock.Accept(cli_sock)==false) {continue;}
    int pid=fork();
    if(pid==0)
    {
      while(1)
      {
        std::string buf;
        cli_sock.Recv(buf);
        std::cout<<"cli say:"<<buf<<std::endl;

        buf.clear();
        std::cout<<"srv say:";
        fflush(stdout);
        getline(std::cin,buf);
        cli_sock.Send(buf);
      }
      //父进程关闭,对子进程无影响,节省父进程空间
      cli_sock.~TcpSocket();
    }

  }
  return 0;
}
