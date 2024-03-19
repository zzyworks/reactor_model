/**
 * @file   my_epoll.h
 * @author Aklzzz(csdn)
 * @date   2024/03/04 10:25:06
 * @note   
 *  封装epoll的创建、索引和放入就绪队列
*/

#ifndef MY_EPOLL_H
#define MY_EPOLL_H

#include <sys/epoll.h>
#include <vector>
#include <errno.h>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "noncopyable.h"
#include "channel.h"

class Channel;

#define MAX_EVENTS 100

class Epoll : Noncopyable {
public:
  Epoll();

  ~Epoll();

public:
  /// @brief 把channel添加/更新到红黑树上
  void updateChannel(Channel* cha);

  /// @brief 运行epoll_wait()等待事件的发生
	std::vector<Channel*> loop(int timeout = -1);

private:
  void init_();

private:
  int epoll_fd_; // epoll句柄
  epoll_event events_[MAX_EVENTS]; // 存放poll_wait()返回事件的数组
};

#endif // MY_EPOLL_H
