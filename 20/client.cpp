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
  for (int i = 0; i != 100; ++i) {
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "这是第%d个超级女生。", i);

    char tmp_buf[1024]; // 临时的buffer，报文头部+报文内容。
    memset(tmp_buf, 0, sizeof(tmp_buf));
    int len = strlen(buffer); // 计算报文的大小。
    memcpy(tmp_buf, &len, 4); // 拼接报文头部。
    memcpy(tmp_buf + 4, buffer, len); // 拼接报文内容。

    send(sock_fd, tmp_buf, len + 4, 0); // 把请求报文发送给服务端。
  }
  for(int i = 0; i != 100; ++i) {
    int len;
    recv(sock_fd, &len, 4, 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock_fd, buffer, len, 0);
    printf("recv:%s\n", buffer);
  }
  return 0;
}
