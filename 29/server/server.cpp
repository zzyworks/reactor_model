#include "echo_server.h"

int main(int argc, char const* argv[]) {
  if (argc != 3) {
    printf("Using: ./server ip port\n");
    printf("Example: ./server 172.30.116.113 5005\n\n");
    return -1;
  }
  
	EchoServer echo_serv(AF_INET, argv[1], atoi(argv[2]));
  echo_serv.start();
  
  return 0;
}
