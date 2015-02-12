#ifndef _BM_PACKET_BUFFER_H_
#define _BM_PACKET_BUFFER_H_

#include <cassert>

using std::unique_ptr;

class PacketBuffer {
public:
  PacketBuffer() {}

  PacketBuffer(size_t size)
    : size(size),
      data_size(0),
      buffer(unique_ptr<char []>(new char[size])),
      head(buffer.get() + size) {}

  PacketBuffer(size_t size, const char *data, size_t data_size)
    : size(size),
      data_size(0),
      buffer(unique_ptr<char []>(new char[size])),
      head(buffer.get() + size) {
    std::copy(data, data + data_size, push(data_size));
  }

  char *start() const { return head; }

  char *end() const { return buffer.get() + size; }
  
  char *push(size_t bytes) {
    assert(data_size + bytes <= size);
    data_size += bytes;
    head -= bytes;
    return head;
  }

  char *pop(size_t bytes) {
    assert(bytes <= data_size);
    data_size -= bytes;
    head += bytes;
    return head;
  }
  
  size_t get_data_size() const { return data_size; }

  PacketBuffer clone(size_t end_bytes) const {
    assert(end_bytes <= data_size);
    PacketBuffer pb(size);
    std::copy(end() - end_bytes, end(), pb.push(end_bytes));
    return pb;
  }

private:
  size_t size;
  size_t data_size;
  unique_ptr<char []> buffer;
  char *head;
};

#endif
