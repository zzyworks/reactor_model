#include <cstdio>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "inet_address.h"

int main(int argc, char const* argv[]) {
  if (argc != 3) {
    printf("Using: ./client ip port\n");
    printf("Example: ./client 172.30.116.113 5005\n\n");
    return -1;
  }
  int sock_fd = 0;
  char buffer[1024];
  if ((sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    printf("socket() failed.\n");
    return -1;
  }
  // memset(&serv_addr, 0, sizeof(serv_addr));
  InetAddress addr(AF_INET, argv[1], atoi(argv[2]));
  if(connect(sock_fd, addr.addr(), sizeof(addr)) != 0) {
    printf("connect(%s:%s) failed.\n", argv[1], argv[2]);
    close(sock_fd);
    return -1;
  }
  for(int i = 0; i != 10000000; ++i) {
    memset(buffer, 0, sizeof(buffer));
    printf("please input:");
    scanf("%s", buffer);
    if(send(sock_fd, buffer, strlen(buffer), 0) <= 0) {
      printf("write() failed.\n");
      close(sock_fd);
      return -1;
    }
    memset(buffer, 0, sizeof(buffer));
    if(recv(sock_fd, buffer, sizeof(buffer), 0) <= 0) {
      printf("read() failed.\n");
      close(sock_fd);
      return -1;
    }
    printf("recv:%s\n", buffer);
  }
  return 0;
}
