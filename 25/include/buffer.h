/**
 * @file   buffer.h
 * @author Aklzzz(csdn)
 * @date   2024/03/06 11:33:12
 * @note   
 *  作为服务端的缓冲区使用
*/

#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <string.h>

#include "copyable.h"

class Buffer : Copyable {
public:
  Buffer();

  ~Buffer();

  /// @brief 把数据追加到buf_中
  std::string& append(const char* data, size_t n);

	// 把数据追加到buf_中,附带报文头部
	std::string& appendWithHead(const char* data, size_t n);
 
	std::string& erase(size_t pos, size_t n);
  
	/// @brief 返回buf_的大小
  size_t size() const;
  
	/// @brief 返回buf_的首地址
  const char* data() const;
  
	/// @brief 清空buf_
  void clear();

private:
  std::string buf_;
};

#endif // BUFFER_H
