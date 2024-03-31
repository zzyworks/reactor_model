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
#include <cstring>

#include "copyable.h"

enum class SeparatorType : uint8_t { NO_SEPARATOR, FOUR_BYTE, HTTP_SEPARATOR };

class Buffer : Copyable {
public:
  Buffer(SeparatorType sep = SeparatorType::FOUR_BYTE);

  ~Buffer() = default;

  /// @brief 把数据追加到buf_中
  void append(const char* data, size_t n);

  // 把数据最加到buf_中，附带报文头部
  void appendWithSep(const char* data, size_t n);

  void erase(size_t pos, size_t n);

  /// @brief 返回buf_的大小
  size_t size() const;

  /// @brief 返回buf_的首地址
  const char* data() const;

  /// @brief 清空buf_
  void clear();

  // 从buf_中拆分出一个报文，存放在str中
  bool pickMessage(std::string& str);

private:
  std::string buf_;
  SeparatorType sep_;
};

#endif  // BUFFER_H
