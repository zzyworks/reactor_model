#include "buffer.h"

Buffer::Buffer(SeparatorType sep) : sep_(sep) {}

void
Buffer::append(const char* data, size_t n) {
  buf_.append(data, n);
}

void
Buffer::appendWithSep(const char* data, size_t n) {
  if (sep_ == SeparatorType::NO_SEPARATOR)
    buf_.append(data, n);
  else if (sep_ == SeparatorType::FOUR_BYTE) {
    buf_.append(reinterpret_cast<char*>(&n), 4);  // 处理报文长度（头部）
    buf_.append(data, n);                         // 处理报文内容
  }
}

void
Buffer::erase(size_t pos, size_t n) {
  buf_.erase(pos, n);
}

size_t
Buffer::size() const {
  return buf_.size();
}

const char*
Buffer::data() const {
  return buf_.data();
}

void
Buffer::clear() {
  buf_.clear();
}

bool
Buffer::pickMessage(std::string& str) {
  if (buf_.size() == 0)
    return false;
  if (sep_ == SeparatorType::NO_SEPARATOR) {
    str = buf_;
    buf_.clear();
  } else if (sep_ == SeparatorType::FOUR_BYTE) {
    std::size_t len = 0;
    memcpy(&len, buf_.data(), 4);  // 从buf_中获取报文头部
    // 如果buf_中的数据量小于报文头部，说明buf_中的报文不完整
    if (buf_.size() < len + 4)
      return false;
    // 从buf_中获取一个报文
    str = buf_.substr(4, len);
    buf_.erase(0, len + 4);  // 从buf_中删除刚才已经获取的报文
  }
  return true;
}
