//普通服务端demo
#include"socket_tcp.hpp"

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
    TcpSocket cli_sock;
    if(acp_sock.Accept(cli_sock)==false) {continue;}

    std::string buf;
    cli_sock.Recv(buf);
    std::cout<<"cli say:"<<buf<<std::endl;

    buf.clear();
    std::cout<<"srv say:";
    fflush(stdout);
    getline(std::cin,buf);
    cli_sock.Send(buf);
  }
  return 0;
}
