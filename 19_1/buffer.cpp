#include "buffer.h"

Buffer::Buffer() {

}

Buffer::~Buffer() {

}

void 
Buffer::append(const char* data, size_t size) {
  buf_.append(data, size);
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