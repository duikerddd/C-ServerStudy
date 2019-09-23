//基于select的TCP服务器
#include<vector>
#include<string>
#include<iostream>
#include<sys/socket.h>
#include<sys/select.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include"tcp.hpp"

//将select操作封装起来
class Selector{
  public:
    //初始化 --- 位图置0
    Selector():_max_fd(0)
    {
      FD_ZERO(&_fds);
    }
    //位图添加一个fd
    void FdSetAdd(TcpSocket& sock)
    {
      int fd=sock.GetFd();
      FD_SET(fd,&_fds);
      //注意_max_fd要变化
      if(fd>_max_fd)
      {
        _max_fd=fd;
      }
    }
    //从位图删除一个fd
    void FdSetDel(TcpSocket& sock)
    {
      int fd=sock.GetFd();
      FD_CLR(fd,&_fds);
      //因为不知道删的是不是最大的fd,所以需要观察_max_fd是否变化
      int max_fd=-1;
      for(int i=0;i<=_max_fd;++i)
      {
        if(!FD_ISSET(i,&_fds))
        {
          continue;
        }
        if(i>max_fd)
        {
          max_fd=i;
        }
      }//end for
      //最后记得把结果给_max_fd
      _max_fd=max_fd;
    }

    int _max_fd;
    fd_set _fds;
  private:
};

