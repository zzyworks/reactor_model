#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

int
main(int argc, char* argv[]) {
  if (argc != 3) {
    printf("Usings: ./client 172.30.116.113 5005\n\n");
    return -1;
  }
  char buf[1024];
  int sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock_fd < 0) {
    perror("socket failed");
    return -1;
  }
  sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));

  if (connect(sock_fd, reinterpret_cast<sockaddr*>(&serv_addr),
              sizeof(serv_addr))
      != 0) {
    printf("connect(%s:%s) failed.\n", argv[1], argv[2]);
    close(sock_fd);
    return -1;
  }

  for(int i = 0; i != 2; ++i) {
    memset(buf, 0, sizeof(buf));
    printf("please input:");
    scanf("%s", buf);
    if(send(sock_fd, buf, strlen(buf), 0) <= 0) {
      printf("write failed.\n");
      close(sock_fd);
      return -1;
    }
    memset(buf, 0, sizeof(buf));
    if(recv(sock_fd, buf, sizeof(buf), 0) <= 0) {
      printf("read failed.\n");
      close(sock_fd);
      return -1;
    }
    printf("recv:%s\n", buf);
  }
  return 0;
}
