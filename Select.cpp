//用select TCP 服务端
#include"select_tcp.hpp"

int main(int argc,char *argv[])
{
  if(argc!=3)
  {
    printf("./Server ip port");
    return 1;
  }
  std::string ip=argv[1];
  uint16_t port=atoi(argv[2]);

  //1.初始化服务器
  TcpSocket listen_sock;
  listen_sock.Socket();
  listen_sock.Bind(ip,port);
  listen_sock.Listen();
  std::cout<<"Server Ok"<<std::endl;

  //2.初始化源Selector,把lisent_sock加入到监听事件中
  Selector read_fds;
  read_fds.FdSetAdd(listen_sock);

  //3.进入事件循环
  while(1)
  {
    //使用select完成等待,并创建一个输出参数
    fd_set out_fds=read_fds._fds;
    int ret=select(read_fds._max_fd+1,&out_fds,NULL,NULL,NULL);
    if(ret<0)
    {
      perror("select");
      return -1;
    }

    //select返回后,有两种情况
    //  a)listen_sock就绪 --- 有新的连接请求,调用accept
    //  b)new_sock就绪 --- 可以从缓冲区读数据了,调用read
    if(FD_ISSET(listen_sock.GetFd(),&out_fds)) {
      TcpSocket new_sock;
      listen_sock.Accept(new_sock);
      read_fds.FdSetAdd(new_sock);
      printf("client %d connected \n",new_sock.GetFd());
    }
    else{
      for(int i=0;i<=read_fds._max_fd;++i)
      {
        if(!FD_ISSET(i,&out_fds))
        {
          continue;
        }
        char buf[1024]={0};
        int read_size=read(i,buf,1023);
        if(read_size<0)
        {
          perror("read");
          return -1;
        }
        else if(read_size ==0)
        {
          //断开,善后工作
          printf("client %d shutdown\n",i);
          FD_CLR(i,&read_fds._fds);
          close(i);
        }
        else {
          printf("client say:%s\n",buf);
          write(i,buf,1023);
        }
      }
    }
  }
  

  return 0;
}
