#include <cstdio>
#include <unistd.h>

void test0() {
  int pipe_fd[2];
  if(pipe2(pipe_fd, 0) == -1) {
    printf("create pipe failed.\n");
    return;
  }

  write(pipe_fd[1], "Hello world", 11);

  ssize_t ret = 0;
  char buf[128] = {0};

  ret = read(pipe_fd[0], &buf, sizeof(buf));
  printf("ret = %ld, buf = %s\n", ret, buf);
  ret = read(pipe_fd[0], &buf, sizeof(buf));
  printf("ret = %ld, buf = %s\n", ret, buf);
  close(pipe_fd[0]);
  close(pipe_fd[1]);
}

int main() {
  test0();
  return 0;
}
