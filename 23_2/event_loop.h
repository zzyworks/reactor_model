/**
 * @file   event_loop.h
 * @author Aklzzz(csdn)
 * @date   2024/03/04 18:57:15
 * @note   
 *  事件循环类
*/

#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <functional>

#include "my_epoll.h"
#include "channel.h"

class Channel;
class Epoll;

class EventLoop : Noncopyable {
public:
	typedef std::function<void(EventLoop*)> EpollTimeoutCallback;

  EventLoop();

  ~EventLoop();

  void run();

  Epoll* epo();

  /// @brief 把channel添加/更新到红黑树上
  void undateChannel(Channel* cha);

	void setEpollTimeOutCb(EpollTimeoutCallback cb);

private:
  Epoll* epo_;
	// epoll_wait()超时的回调函数
	EpollTimeoutCallback epo_timeout_cb_;
};

#endif // EVENT_LOOP_H
