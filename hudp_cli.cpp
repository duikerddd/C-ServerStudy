
//udp--客户端
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

   sockaddr_in srv_addr;
   srv_addr.sin_family=AF_INET;
   srv_addr.sin_port=htons(port);
   srv_addr.sin_addr.s_addr=inet_addr(ip.c_str());
  while(1)
  {
   string buf;
   std::cout<<"cli say:";
   fflush(stdout);
   getline(cin,buf);
   sock.Send(buf,srv_addr);
 
   buf.clear();
   sock.Recv(buf);
   std::cout<<"ser say:"<<buf<<endl;
  }
  
  sock.Close();
  return 0;
}
