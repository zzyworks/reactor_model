#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <string.h>
#include <errno.h>

class Channel {
public:
  Channel(int fd, bool is_listen = true) : fd_(fd),
                                           is_listen_(is_listen) {}

  int fd() { return fd_; }
  
  bool isListen() { return is_listen_; }
  // ...

private:
  int fd_;
  bool is_listen_; // true-监听的fd，false-客户端的fd
  // ...
};

void setNonblocking(int fd) {
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | (O_NONBLOCK));
}

int main(int argc, char const *argv[])
{
  if (argc != 3) {
    printf("Using: ./server ip port\n");
    printf("Example: ./server 172.30.116.113 5005\n\n");
    return -1;
  }
  int listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(listen_fd < 0) {
    perror("socket() failed.\n");
    return -1;
  }
  int opt = 1;
  setsockopt(listen_fd, SOL_SOCKET, 
             SO_REUSEADDR, &opt, 
             static_cast<socklen_t>(sizeof(opt)));
  setsockopt(listen_fd, SOL_SOCKET, 
             TCP_NODELAY, &opt, 
             static_cast<socklen_t>(sizeof(opt)));
  setsockopt(listen_fd, SOL_SOCKET, 
             SO_REUSEPORT, &opt, 
             static_cast<socklen_t>(sizeof(opt)));
  setsockopt(listen_fd, SOL_SOCKET, 
             SO_KEEPALIVE, &opt, 
             static_cast<socklen_t>(sizeof(opt)));
  setNonblocking(listen_fd);
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(argv[2]));
  addr.sin_addr.s_addr = inet_addr(argv[1]);
  if(bind(listen_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
    perror("bind() faile");
    close(listen_fd);
    return -1;
  }
  if(listen(listen_fd, 128) != 0) {
    perror("listen() failed");
    close(listen_fd);
    return -1;
  }
  int epoll_fd = epoll_create(1);
  Channel* serv_channel = new Channel(listen_fd);
  epoll_event ev;
  ev.data.ptr = serv_channel;
  ev.events = EPOLLIN; // 水平触发
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev); // 红黑树索引
  epoll_event evs[10];
  Channel* cha = nullptr;
  while(true) {
    int fds = epoll_wait(epoll_fd, evs, 10, -1);
    if(fds < 0) {
      perror("epoll_wait() failed");
      break;
    }
    if(fds == 0) {
      printf("epoll_wait() timeout.\n");
      continue;
    }
    // 如果fds > 0，表示有事件发生的fd的数量
    for(int i = 0; i != fds; ++i) { // 遍历epoll返回的数组evs
      cha = reinterpret_cast<Channel*>(evs[i].data.ptr);
      if(cha->isListen()) { // 如果是listen_fd有事件，表示有新的客户端连上来
        //////////////////////////////////////////////////////
        sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int client_fd = accept(listen_fd, 
                               reinterpret_cast<sockaddr*>(&client_addr),
                               &len);
        setNonblocking(client_fd);
        printf("accept client(fd=%d, ip=%s, port=%d) ok.\n",
               client_fd,
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));
        // 为新客户端连接准备读事件，并添加到epoll中
        Channel* client_channel = new Channel(client_fd, false);
        ev.data.ptr = client_channel;
        ev.events = EPOLLIN|EPOLLET;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
        //////////////////////////////////////////////////////
      } else { // 如果客户端连接的fd有事件
        //////////////////////////////////////////////////////
          if(evs[i].events & EPOLLRDHUP) {
            printf("client(event_fd=%d) disconnected.\n", cha->fd());
            close(cha->fd());
          } else if(evs[i].events & (EPOLLIN | EPOLLPRI)) {
            char buffer[1024];
            while(true) { // 由于使用非阻塞IO，一次读取buffer大小数据，直到全部的数据读取完毕
              memset(buffer, 0, sizeof(buffer));
              ssize_t read_n = read(cha->fd(), buffer, sizeof(buffer));
              if(read_n > 0) {
                printf("%s\n", buffer);
              } else if(read_n == -1 && errno == EINTR) {
                continue;
              } else if(read_n == -1 
                        && ((errno == EAGAIN) 
                             || (errno == EWOULDBLOCK))) {
                break;
              } else if(read_n == 0) {
                printf("client(event_fd=%d) disconnected.\n", cha->fd());
                close(cha->fd());
                break;
              }
            }
          } else if(evs[i].events & EPOLLOUT) {

          } else {
            printf("client(event_fd=%d) error.\n", cha->fd());
            close(cha->fd());
          }
        //////////////////////////////////////////////////////
      }
    }
  }
  return 0;
}
