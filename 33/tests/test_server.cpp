#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

void
setNonblocking(int fd) {
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int
main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usings: ./server 172.30.116.113 5005\n\n");
    return -1;
  }
  int listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (listen_fd < 0) {
    perror("socket failed");
    return -1;
  }
  int opt = 1;
  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt,
             static_cast<socklen_t>(sizeof(opt)));
  setsockopt(listen_fd, SOL_SOCKET, TCP_NODELAY, &opt,
             static_cast<socklen_t>(sizeof(opt)));
  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &opt,
             static_cast<socklen_t>(sizeof(opt)));
  setsockopt(listen_fd, SOL_SOCKET, SO_KEEPALIVE, &opt,
             static_cast<socklen_t>(sizeof(opt)));
  setNonblocking(listen_fd);
  sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));

  if (bind(listen_fd, reinterpret_cast<sockaddr*>(&serv_addr),
           sizeof(serv_addr))
      < 0) {
    perror("bind failed");
    close(listen_fd);
    return -1;
  }

  if (listen(listen_fd, 128) != 0) {
    perror("listen failed");
    close(listen_fd);
    return -1;
  }

  int epoll_fd = epoll_create(1);

  epoll_event ev;
  ev.data.fd = listen_fd;
  ev.events = EPOLLIN;

  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);

  epoll_event evs[10];
  
  /*
  //////////////////////////////////////////////////////
  // 定时器
  int tfd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC | TFD_NONBLOCK);
  itimerspec timeout;
  bzero(&timeout, sizeof(timeout));
  timeout.it_value.tv_sec = 5;
  timeout.it_value.tv_nsec = 0;
  timerfd_settime(tfd, 0, &timeout, 0);
  ev.data.fd = tfd;
  ev.events = EPOLLIN;
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tfd, &ev);
  //////////////////////////////////////////////////////
  */
  //////////////////////////////////////////////////////
  // 信号
  //////////////////////////////////////////////////////
  for (;;) {
    int fds = epoll_wait(epoll_fd, evs, 10, -1);
    if (fds < 0) {
      perror("epoll_wait failed");
      break;
    } else if (fds == 0) {
      printf("epoll wait timeout.\n");
      continue;
    }
    for (int i = 0; i != fds; ++i) {
      /*
      //////////////////////////////////////////////////////
      if(evs[i].data.fd == tfd) {
        printf("timeout\n");
        timerfd_settime(tfd, 0, &timeout, 0);
        continue;
      }
      //////////////////////////////////////////////////////
      */
      if (evs[i].data.fd == listen_fd) {
        sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int client_fd =
          accept(listen_fd, reinterpret_cast<sockaddr*>(&client_addr), &len);
        setNonblocking(client_fd);
        printf("accept client(fd=%d,ip=%s,port=%d) ok.\n", client_fd,
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        ev.data.fd = client_fd;
        ev.events = EPOLLIN | EPOLLET;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
      } else {
        if (evs[i].events & EPOLLRDHUP) {
          printf("client(event_fd=%d) disconnection.\n", evs[i].data.fd);
          close(evs[i].data.fd);
        } else if (evs[i].events & (EPOLLIN | EPOLLPRI)) {
          char buf[1024];
          for (;;) {
            bzero(&buf, sizeof(buf));
            ssize_t n_read = read(evs[i].data.fd, buf, sizeof(buf));
            if (n_read > 0) {
              printf("recv(event_fd=%d):%s\n", evs[i].data.fd, buf);
              send(evs[i].data.fd, buf, strlen(buf), 0);
            } else if (n_read == -1 && errno == EINTR)
              continue;
            else if (n_read == -1
                     && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
              break;
            else if (n_read == 0) {
              printf("client(event_fd=%d) disconnection.\n", evs[i].data.fd);
              close(evs[i].data.fd);
              break;
            }
          }
        } else if (evs[i].events & EPOLLOUT) {
        } else {
          printf("client(event_fd=%d) error.\n", evs[i].data.fd);
          close(evs[i].data.fd);
        }
      }
    }
  }
  return 0;
}
