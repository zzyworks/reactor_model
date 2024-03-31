/**
 * @file   event_loop.h
 * @author Aklzzz(csdn)
 * @date   2024/03/04 18:57:15
 * @note   
 *  事件循环类
*/

#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <string.h>
#include <sys/eventfd.h>
#include <sys/syscall.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <functional>
#include <mutex>
#include <queue>

#include "my_epoll.h"
#include "noncopyable.h"

class Channel;
class Epoll;

int initTimerFd(int sec = 5);

enum class LoopType : uint8_t { MAIN_LOOP, OTHER_LOOP };

class EventLoop : Noncopyable {
public:
  using EpollTimeoutCb = std::function<void(EventLoop*)>;
  using Tasks = std::function<void()>;

  EventLoop(const LoopType& loop_type);

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

  // 闹钟响时执行的函数
  void handleTimer();

private:
  LoopType loop_type_;  // 区分主从事件循环
  std::unique_ptr<Epoll> epo_;
  pid_t thread_id_;  // 事件循环所在线程的id
  std::queue<Tasks> tasks_que_;  // 事件循环线程被eventfd唤醒后执行的任务队列
  std::mutex mutex_;  // 任务队列同步的互斥锁
  int wake_up_fd_;    // 用于唤醒事件循环线程的eventfd
  std::unique_ptr<Channel> wake_channel_;  // eventfd的channel
  int timer_fd_;
  std::unique_ptr<Channel> timer_channel_;
  EpollTimeoutCb epo_timeout_cb_;  // epoll_wait()超时的回调函数
};

#endif  // EVENT_LOOP_H
