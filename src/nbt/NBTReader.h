//
// Created by Jacob Andersen on 7/28/25.
//

#ifndef CELERITY_NBT_NBTREADER_H
#define CELERITY_NBT_NBTREADER_H

#include "../ByteBuffer.h"
#include "tag/NamedTag.h"
#include "tag/TagCompound.h"
#include "tag/TagList.h"

namespace celerity::nbt {
class NBTReader {
  ByteBuffer& buffer_;

 public:
  explicit NBTReader(ByteBuffer& buffer) : buffer_(buffer) {}
  tag::NamedTag read_tag(int depth);

 private:
  std::unique_ptr<tag::Tag> read_payload(const tag::TagType& type, int depth);
};
}  // namespace celerity::nbt

#endif  // CELERITY_NBT_NBTREADER_H
