//
// Created by Jacob Andersen on 7/29/25.
//

#ifndef NBTWRITER_H
#define NBTWRITER_H
#include "src/ByteBuffer.h"
#include "tag/NamedTag.h"

namespace celerity::nbt {
class NBTWriter {
  ByteBuffer& buffer_;

 public:
  explicit NBTWriter(ByteBuffer& buffer) : buffer_(buffer) {}
  void write_tag(tag::NamedTag& tag);

 private:
  void write_payload(std::unique_ptr<tag::Tag>& tag);
};
}  // namespace celerity::nbt
#endif  // NBTWRITER_H
