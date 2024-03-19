#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const* argv[]) {
  if (argc != 3) {
    printf("Using: ./client ip port\n");
    printf("Example: ./client 172.30.116.113 5005\n\n");
    return -1;
  }
  int sock_fd = 0;
  sockaddr_in serv_addr;
  char buffer[1024];
  if ((sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    printf("socket() failed.\n");
    return -1;
  }
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(atoi(argv[2]));
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  if(connect(sock_fd, (sockaddr*)&serv_addr, sizeof(serv_addr)) != 0) {
    printf("connect(%s:%s) failed.\n", argv[1], argv[2]);
    close(sock_fd);
    return -1;
  }
  for(int i = 0; i != 1000; ++i) {
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "这是第%03d个人", i);
    if(send(sock_fd, buffer, strlen(buffer), 0) <= 0) {
      printf("write() failed.\n");
      close(sock_fd);
      return -1;
    }
    // usleep(100);
  }
  return 0;
}
