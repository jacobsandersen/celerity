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
  void write_tag(const tag::NamedTag& tag) const;
  void write_tag(const tag::Tag& tag) const;
  void write_tag(const icu::UnicodeString& name, const tag::Tag& tag) const;

 private:
  void write_payload(const tag::Tag& tag) const;
};
}  // namespace celerity::nbt
#endif  // NBTWRITER_H
