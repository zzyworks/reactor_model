#include <signal.h>

#include "echo_server.h"

EchoServer* echo_serv = nullptr;

void stop(int sig) {
  printf("sig=%d", sig);
  echo_serv->stop();
  printf("echo_serv is stop\n");
  if(echo_serv) {
    delete echo_serv;
    echo_serv = nullptr;
  }
  exit(0);
}

int main(int argc, char const* argv[]) {
  if (argc != 3) {
    printf("Using: ./server ip port\n");
    printf("Example: ./server 172.30.116.113 5005\n\n");
    return -1;
  }
 
  signal(SIGTERM, stop);
  signal(SIGINT, stop);

	echo_serv = new EchoServer(AF_INET, argv[1], atoi(argv[2]), 3, 2);
  echo_serv->start();
  
  return 0;
}
