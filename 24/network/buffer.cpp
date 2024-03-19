#include "buffer.h"

Buffer::Buffer() {

}

Buffer::~Buffer() {

}

std::string&
Buffer::append(const char* data, size_t n) {
  return buf_.append(data, n);
}

std::string& 
Buffer::erase(size_t pos, size_t n) {
  return buf_.erase(pos, n);
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
