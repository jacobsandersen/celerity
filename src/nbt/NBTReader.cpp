//
// Created by Jacob Andersen on 7/28/25.
//

#include "NBTReader.h"

#include "TagListBuilder.h"
#include "tag/TagByte.h"
#include "tag/TagByteArray.h"
#include "tag/TagDouble.h"
#include "tag/TagFloat.h"
#include "tag/TagInt.h"
#include "tag/TagIntArray.h"
#include "tag/TagLong.h"
#include "tag/TagLongArray.h"
#include "tag/TagShort.h"
#include "tag/TagString.h"

namespace celerity::nbt {
template <typename T, typename TagT>
  requires std::is_integral_v<T> && DerivedTag<TagT>
std::unique_ptr<TagT> read_tag_array(ByteBuffer& buffer, const std::function<T(ByteBuffer&)>& read_fn) {
  const int32_t length = buffer.read_be_int();
  std::vector<T> items(length);
  for (int i = 0; i < length; i++) {
    items.push_back(read_fn(buffer));
  }
  return std::make_unique<TagT>(items);
}

tag::NamedTag NBTReader::read_tag() {  // NOLINT(*-no-recursion)
  const tag::TagType type = buffer_.read_nbt_tag_type();

  icu::UnicodeString name;
  if (type != tag::TagType::End) {
    name = buffer_.read_string_modified_utf8();
  } else {
    name = "";
  }

  return {name, read_payload(type)};
}

std::unique_ptr<tag::Tag> NBTReader::read_network_tag() { return read_payload(buffer_.read_nbt_tag_type()); }

std::unique_ptr<tag::Tag> NBTReader::read_payload(const tag::TagType& type) {  // NOLINT(*-no-recursion)
  switch (type.get_type_id()) {
    // End
    case 0: {
      return std::make_unique<tag::TagEnd>();
    }
    // Byte
    case 1: {
      return std::make_unique<tag::TagByte>(buffer_.read_byte());
    }
    // Short
    case 2: {
      return std::make_unique<tag::TagShort>(buffer_.read_be_short());
    }
    // Int
    case 3: {
      return std::make_unique<tag::TagInt>(buffer_.read_be_int());
    }
    // Long
    case 4: {
      return std::make_unique<tag::TagLong>(buffer_.read_be_long());
    }
    // Float
    case 5: {
      return std::make_unique<tag::TagFloat>(buffer_.read_be_float());
    }
    // Double
    case 6: {
      return std::make_unique<tag::TagDouble>(buffer_.read_be_double());
    }
    // ByteArray
    case 7: {
      return read_tag_array<int8_t, tag::TagByteArray>(buffer_, [](ByteBuffer& b) { return b.read_byte(); });
    }
    // String
    case 8: {
      return std::make_unique<tag::TagString>(buffer_.read_string_modified_utf8());
    }
    // List
    case 9: {
      const tag::TagType inner_type = buffer_.read_nbt_tag_type();
      const int32_t length = buffer_.read_int();
      if (inner_type == tag::TagType::End && length > 0) {
        throw std::domain_error("TAG_List with length > 0 may not contain TAG_End");
      }

      auto list = std::make_unique<tag::TagList>(inner_type);
      for (int i = 0; i < length; i++) {
        list->add(read_payload(inner_type));
      }

      return list;
    }
    // Compound
    case 10: {
      auto compound = std::make_unique<tag::TagCompound>();
      while (true) {
        tag::NamedTag named_tag = read_tag();
        if (named_tag.get_tag()->get_type() == tag::TagType::End) break;
        compound->add(std::move(named_tag));
      }
      return compound;
    }
    // IntArray
    case 11: {
      return read_tag_array<int32_t, tag::TagIntArray>(buffer_, [](ByteBuffer& b) { return b.read_be_int(); });
    }
    // LongArray
    case 12: {
      return read_tag_array<int64_t, tag::TagLongArray>(buffer_, [](ByteBuffer& b) { return b.read_be_long(); });
    }
    default: {
      throw std::domain_error("Requested to read unknown NBT tag");
    }
  }
}
}  // namespace celerity::nbt