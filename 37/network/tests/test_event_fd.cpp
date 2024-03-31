// 写操作可以多次写入，而读操作只能读一次，多读则阻塞

#include <stdint.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <cstdio>

void
test0() {
  int efd = eventfd(0, 0);
  uint64_t buf = 3;
  ssize_t ret = 0;
  ret = write(efd, &buf, sizeof(uint64_t));
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf); // 读取成功返回8，失败返回-1
  close(efd);
}

void
test1() {
  int efd = eventfd(2, 0);
  uint64_t buf = 3;
  ssize_t ret = 0;
  ret = write(efd, &buf, sizeof(uint64_t));
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf); // 8 5
  close(efd);
}

void
test2() {
  int efd = eventfd(2, 0);
  uint64_t buf = 3;
  ssize_t ret = 0;
  ret = write(efd, &buf, sizeof(uint64_t));
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf);
  ret = read(efd, &buf, sizeof(uint64_t)); // 阻塞
  printf("ret = %ld, buf = %ld\n", ret, buf);
  close(efd);
}

void
test3() {
  int efd = eventfd(0, 0);
  uint64_t buf = 3;
  ssize_t ret = 0;
  ret = read(efd, &buf, sizeof(uint64_t)); // 阻塞
  printf("ret = %ld, buf = %ld\n", ret, buf);
  close(efd);
}

void
test4() {
  int efd = eventfd(2, 0);
  uint64_t buf = 3;
  ssize_t ret = 0;
  ret = read(efd, &buf, sizeof(uint64_t)); // 没有写入,直接读初始值为2
  printf("ret = %ld, buf = %ld\n", ret, buf); // 8 2
  close(efd);
}

void
test5() {
  int efd = eventfd(2, 0);
  uint64_t buf = 3;
  ssize_t ret = 0;
  ret = write(efd, &buf, sizeof(buf));
  ret = write(efd, &buf, sizeof(buf));
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf); // 8 8(3 + 3 + 2)
  close(efd);
}

void
test6() {
  int efd = eventfd(2, 0);
  uint64_t buf = 3;
  ssize_t ret = 0;
  ret = write(efd, &buf, sizeof(buf));
  ret = write(efd, &buf, sizeof(buf));
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf); // 8 8(3 + 3 + 2)
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf); // 阻塞
  close(efd);
}

void
test7() {
  int efd = eventfd(2, EFD_NONBLOCK);
  uint64_t buf = 3;
  ssize_t ret = 0;
  ret = write(efd, &buf, sizeof(buf));
  ret = write(efd, &buf, sizeof(buf));
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf); // 8 8(3 + 3 + 2)
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf);
  close(efd);
}

void
test8() {
  int efd = eventfd(2, EFD_NONBLOCK|EFD_SEMAPHORE);
  uint64_t buf = 3;
  ssize_t ret = 0;
  ret = write(efd, &buf, sizeof(buf));
  ret = write(efd, &buf, sizeof(buf));
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf); // 8 8(3 + 3 + 2)
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf);
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf);
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf);
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf);
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf);
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf);
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf);
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf);
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf);
  ret = read(efd, &buf, sizeof(uint64_t));
  printf("ret = %ld, buf = %ld\n", ret, buf);
  close(efd);
}

int
main() {
  test8();
  return 0;
}
