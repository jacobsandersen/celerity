//
// Created by Jacob Andersen on 7/29/25.
//

#include "NBTWriter.h"

#include "Concepts.h"
#include "tag/TagByte.h"
#include "tag/TagByteArray.h"
#include "tag/TagCompound.h"
#include "tag/TagDouble.h"
#include "tag/TagFloat.h"
#include "tag/TagInt.h"
#include "tag/TagIntArray.h"
#include "tag/TagList.h"
#include "tag/TagLong.h"
#include "tag/TagLongArray.h"
#include "tag/TagShort.h"
#include "tag/TagString.h"

namespace celerity::nbt {
void NBTWriter::write_tag(const tag::NamedTag& tag) const {  // NOLINT(*-no-recursion)
  write_tag(tag.get_name(), tag.get_tag());
}

void NBTWriter::write_tag(const std::unique_ptr<tag::Tag>& tag) const {
  icu::UnicodeString bogus;
  bogus.setToBogus();

  write_tag(bogus, tag);
}

void NBTWriter::write_tag(const icu::UnicodeString& name,  // NOLINT(*-no-recursion)
                          const std::unique_ptr<tag::Tag>& tag) const {
  if (tag == nullptr) return;

  const auto type = tag->get_type();
  if (type == tag::TagType::End) {
    throw std::runtime_error("Cannot write a top-level Tag_END");
  }

  buffer_.write_ubyte(type.get_type_id());
  if (!name.isBogus()) buffer_.write_string_modified_utf8(name);
  write_payload(tag);
}

template <typename T>
  requires DerivedTag<T>
T* downcast(const std::unique_ptr<tag::Tag>& base_tag) {
  auto* downcasted = dynamic_cast<T*>(base_tag.get());
  if (downcasted == nullptr) {
    throw std::runtime_error(
        "Attempted to downcast tag for writing, but did not get expected type. "
        "Malformed NBT?");
  }

  return downcasted;
}

void NBTWriter::write_payload(const std::unique_ptr<tag::Tag>& tag) const {  // NOLINT(*-no-recursion)
  switch (tag->get_type().get_type_id()) {
    // End
    case 0: {
      buffer_.write_ubyte(0);
      break;
    }
    // Byte
    case 1: {
      buffer_.write_byte(downcast<tag::TagByte>(tag)->get_byte());
      break;
    }
    // Short
    case 2: {
      buffer_.write_be_short(downcast<tag::TagShort>(tag)->get_short());
      break;
    }
    // Int
    case 3: {
      buffer_.write_be_int(downcast<tag::TagInt>(tag)->get_int());
      break;
    }
    // Long
    case 4: {
      buffer_.write_be_long(downcast<tag::TagLong>(tag)->get_long());
      break;
    }
    // Float
    case 5: {
      buffer_.write_be_float(downcast<tag::TagFloat>(tag)->get_float());
      break;
    }
    // Double
    case 6: {
      buffer_.write_be_double(downcast<tag::TagDouble>(tag)->get_double());
      break;
    }
    // ByteArray
    case 7: {
      const auto bytes = downcast<tag::TagByteArray>(tag)->get_bytes();
      buffer_.write_be_int(static_cast<int32_t>(bytes.size()));
      for (const int8_t byte : bytes) {
        buffer_.write_byte(byte);
      }
      break;
    }
    // String
    case 8: {
      buffer_.write_string_modified_utf8(downcast<tag::TagString>(tag)->get_string());
      break;
    }
    // List
    case 9: {
      const auto list_tag = downcast<tag::TagList>(tag);
      const auto& items = list_tag->get_items();
      buffer_.write_ubyte(list_tag->get_child_type().get_type_id());
      buffer_.write_be_int(static_cast<int32_t>(items.size()));
      for (auto& item : items) {
        write_payload(item);
      }
      break;
    }
    // Compound
    case 10: {
      for (const auto& tags = downcast<tag::TagCompound>(tag)->get_tags(); auto& named_tag : tags) {
        write_tag(named_tag);
      }
      buffer_.write_ubyte(0);  // Tag End
      break;
    }
    // IntArray
    case 11: {
      const auto ints = downcast<tag::TagIntArray>(tag)->get_ints();
      buffer_.write_be_int(static_cast<int32_t>(ints.size()));
      for (const int32_t int_ : ints) {
        buffer_.write_be_int(int_);
      }
      break;
    }
    // LongArray
    case 12: {
      const auto longs = downcast<tag::TagLongArray>(tag)->get_longs();
      buffer_.write_be_int(static_cast<int32_t>(longs.size()));
      for (const int64_t long_ : longs) {
        buffer_.write_be_long(long_);
      }
      break;
    }
    default: {
      throw std::domain_error("Attempted to write unknown tag type");
    }
  }
}

}  // namespace celerity::nbt