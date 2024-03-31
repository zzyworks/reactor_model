/**
 * @file   channel.h
 * @author Aklzzz(csdn)
 * @date   2024/03/04 13:16:20
 * @note   
 *  管道类，用于让epoll_event携带更多的数据，以便于区分不同服务的fd
*/

#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>

#include "my_epoll.h"
#include "my_socket.h"
#include "event_loop.h"
#include "connection.h"

class Epoll;

class Channel : Noncopyable {
public:
	typedef std::function<void()> ReadCallback;
	typedef std::function<void()> CloseCallback;
	typedef std::function<void()> ErrorCallback;
	typedef std::function<void()> WriteCallback;

	Channel();

	Channel(EventLoop* loop, int fd);

  ~Channel() = default;

  int fd();

  /// @brief 采用边缘触发
  void useET();

  /// @brief 注册读事件
  void enableReading();
  
  /// @brief 注销读事件
  void disableReading();
  
  /// @brief 注册写事件
  void enableWriting();
  
  /// @brief 注销写事件
  void disableWrite();
  
  void setInEpoll(bool on = true);

  void setRevents(uint32_t ev);

  bool inPoll();

  uint32_t events();

  uint32_t revents();

  /// @brief 事件处理函数，epoll_wait()返回时，运行
  void handleEvent();

  /// @brief 设置fd读事件的回调函数
  void setReadCallback(ReadCallback cb);

  /// @brief 设置关闭fd的回调函数
  void setCloseCallback(CloseCallback cb);

  /// @brief 设置fd发生了错误的回调函数
  void setErrorCallback(ErrorCallback cb);

	// 设置fd_写事件的回调函数
	void setWriteCallback(WriteCallback cb);

private:
  EventLoop* loop_; // Channel对应的事件循环
  int fd_; // Channel拥有的fd，Channel与fd是一对一关系
  uint32_t events_; // fd_需要监视的事件
  uint32_t revents_; // fd_已发生的事件
  bool in_epoll_; // Channel是否已添加到epoll树上
  // fd读事件的回调函数，如果是accept_channel，将回调Acceptor::newConnection()；如果是client_channel，将回调Channel::onMessage()
	ReadCallback read_cb_; 
	// 关闭fd的回调函数，将回调Connection::closeCallback()
  CloseCallback close_cb_; 
	// fd发生错误的回调函数，将回调Connection::errorCallback()
  ErrorCallback error_cb_;
	// 设置fd_写事件的回调函数，将回调Connection::writeCallback()
	WriteCallback write_cb_;
};

#endif // CHANNEL_H
