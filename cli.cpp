//客户端demo
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
  
  TcpSocket cli_sock;
  cli_sock.Socket();
  cli_sock.Connect(ip,port);

  while(1)
  {
    std::string buf;
    std::cout<<"cli say:";
    fflush(stdout);
    getline(std::cin,buf);
    cli_sock.Send(buf);

    buf.clear();
    cli_sock.Recv(buf);
    std::cout<<"srv say:"<<buf<<std::endl;

  }
  return 0;
}
