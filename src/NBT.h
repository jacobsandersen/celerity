//
// Created by Jacob Andersen on 8/12/25.
//

#ifndef NBT_H
#define NBT_H

#include <cstddef>
#include <vector>
#include <boost/container/map.hpp>

namespace celerity::nbt {
enum class TagType {
  End,
  Byte,
  Short,
  Int,
  Long,
  Float,
  Double,
  ByteArray,
  String,
  List,
  Compound,
  IntArray,
  LongArray
};

struct Tag;
typedef nullptr_t TagEnd;
typedef int8_t TagByte;
typedef int16_t TagShort;
typedef int32_t TagInt;
typedef int64_t TagLong;
typedef float TagFloat;
typedef double TagDouble;
typedef std::vector<TagByte> TagByteArray;
typedef std::string TagString;
struct TagList;
typedef boost::container::map<TagString, Tag> TagCompound;
typedef std::vector<TagInt> TagIntArray;
typedef std::vector<TagLong> TagLongArray;



}  // namespace celerity::nbt

#endif //NBT_H
