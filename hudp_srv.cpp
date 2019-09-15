//udp--服务端
#include"socket_udp.hpp"

int main(int argc,char* argv[])
{
  if(argc!=3)
  {
    std::cout<<"./udp_srv  ip   port";
    return -1;
  }
  string ip=argv[1];
  //atoi --- 字符串转化为整形数
  uint16_t port=atoi(argv[2]);
  
  UdpSocket sock;
  sock.Socket(); 
  sock.Bind(ip,port);

  while(1)
  {
   sockaddr_in cli_addr;
   string buf; 
   sock.Recv(buf,&cli_addr);
   std::cout<<"cli say:"<<buf<<std::endl;

   buf.clear();
   std::cout<<"ser say:";
   fflush(stdout);
   getline(cin,buf);
   sock.Send(buf,cli_addr);
  }
  
  sock.Close();
  return 0;
}
