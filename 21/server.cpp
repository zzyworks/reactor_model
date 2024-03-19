#include "tcp_server.h"

int main(int argc, char const* argv[]) {
  if (argc != 3) {
    printf("Using: ./server ip port\n");
    printf("Example: ./server 192.168.0.78 5005\n\n");
    return -1;
  }
  
  TcpServer tcp_server(AF_INET, argv[1], atoi(argv[2]));
  tcp_server.start();
  
  return 0;
}
