/**
 * @file   event_loop.h
 * @author Aklzzz(csdn)
 * @date   2024/03/04 18:57:15
 * @note   
 *  事件循环类
*/

#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <sys/syscall.h>
#include <unistd.h>
#include <functional>
#include <queue>
#include <mutex>
#include <sys/eventfd.h>

#include "my_epoll.h"
#include "noncopyable.h"

class Channel;
class Epoll;

class EventLoop : Noncopyable {
public:
  using EpollTimeoutCb = std::function<void(EventLoop*)>;
  using Tasks = std::function<void()>;

  EventLoop();

  ~EventLoop() = default;

  void run();

  // 把任务添加到队列中
  void push(Tasks fn);

  /// @brief 把channel添加/更新到红黑树上
  void undateChannel(Channel* cha);

  // 从红黑树上删除channel
  void removeChannel(Channel* cha);

  void setEpollTimeOutCb(EpollTimeoutCb cb);

  // 判断当前线程是否为事件循环线程
  bool isInLoopThread();

  // 用eventfd唤醒事件循环线程
  void wakeUp();

  // 事件循环线程被唤醒后执行的函数
  void handleWakeUp();

private:
  std::unique_ptr<Epoll> epo_;
  pid_t thread_id_;  // 事件循环所在线程的id
  std::queue<Tasks> tasks_que_; // 事件循环线程被eventfd唤醒后执行的任务队列
  std::mutex mutex_; // 任务队列同步的互斥锁
  int wake_up_fd_; // 用于唤醒事件循环线程的eventfd
  std::unique_ptr<Channel> wake_channel_; // eventfd的channel
  EpollTimeoutCb epo_timeout_cb_;  // epoll_wait()超时的回调函数
};

#endif  // EVENT_LOOP_H
