//
// Created by Jacob Andersen on 8/15/25.
//

#include "Json2NBTReader.h"

#include <json/reader.h>

#include <algorithm>
#include <fstream>
#include <ranges>

#include "TagCompoundBuilder.h"
#include "TagUtil.h"
#include "tag/TagByte.h"
#include "tag/TagByteArray.h"
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
Json2NBTReader::Json2NBTReader(const std::string& filepath) {
  std::ifstream ifs(filepath, std::ifstream::binary);
  if (!ifs.is_open()) {
    throw std::runtime_error("Can't open file " + filepath);
  }

  Json::CharReaderBuilder builder;
  builder["collectComments"] = false;

  JSONCPP_STRING err;
  if (!Json::parseFromStream(builder, ifs, &json_, &err)) {
    throw std::runtime_error("Error parsing JSON file " + filepath + ": " + err);
  }

  if (!json_.isObject()) {
    throw std::runtime_error("JSON object expected");
  }
}

std::unique_ptr<tag::Tag> convertSigned(const Json::Value& value) {
  const auto number = value.asLargestInt();

  if (number >= std::numeric_limits<int8_t>::min() && number <= std::numeric_limits<int8_t>::max()) {
    return std::make_unique<tag::TagByte>(static_cast<int8_t>(number));
  }

  if (number >= std::numeric_limits<int16_t>::min() && number <= std::numeric_limits<int16_t>::max()) {
    return std::make_unique<tag::TagShort>(static_cast<int16_t>(number));
  }

  if (number >= std::numeric_limits<int32_t>::min() && number <= std::numeric_limits<int32_t>::max()) {
    return std::make_unique<tag::TagInt>(static_cast<int32_t>(number));
  }

  return std::make_unique<tag::TagLong>(static_cast<int64_t>(number));
}

std::unique_ptr<tag::Tag> convertUnsigned(const Json::Value& value) {
  const auto number = value.asLargestUInt();

  if (number <= std::numeric_limits<int8_t>::max()) {
    return std::make_unique<tag::TagByte>(static_cast<int8_t>(number));
  }

  if (number <= std::numeric_limits<int16_t>::max()) {
    return std::make_unique<tag::TagShort>(static_cast<int16_t>(number));
  }

  if (number <= std::numeric_limits<int32_t>::max()) {
    return std::make_unique<tag::TagInt>(static_cast<int32_t>(number));
  }

  return std::make_unique<tag::TagLong>(static_cast<int64_t>(number));
}

std::unique_ptr<tag::Tag> convertFloating(const Json::Value& value) {
  const auto number = value.asDouble();
  if (std::isnan(number) || std::isinf(number)) {
    throw std::runtime_error("Can't convert floating point to NBT representation: " + std::to_string(number));
  }

  if (const float f = static_cast<float>(number); static_cast<double>(f) == number) {
    return std::make_unique<tag::TagFloat>(f);
  }

  return std::make_unique<tag::TagDouble>(number);
}

template <typename TagT, typename ValueT, typename ArrayT, typename Getter>
std::unique_ptr<tag::Tag> collect_array(const std::vector<std::unique_ptr<tag::Tag>>& items, Getter getter) {
  std::vector<ValueT> values;
  values.reserve(items.size());

  for (const auto& item : items) {
    values.push_back((TagUtil::downcast<TagT>(item)->*getter)());
  }

  return std::make_unique<ArrayT>(std::move(values));
}

std::unique_ptr<tag::Tag> convert(Json::Value value) {
  switch (value.type()) {
    case Json::nullValue: {
      throw std::runtime_error("Cannot convert JsonNull to NBT");
    }
    case Json::intValue: {
      return convertSigned(value);
    }
    case Json::uintValue: {
      return convertUnsigned(value);
    }
    case Json::realValue: {
      return convertFloating(value);
    }
    case Json::stringValue: {
      return std::make_unique<tag::TagString>(value.asCString());
    }
    case Json::booleanValue: {
      return std::make_unique<tag::TagByte>(value.asBool() ? 1 : 0);
    }
    case Json::arrayValue: {
      std::vector<std::unique_ptr<tag::Tag>> items;
      for (const auto& item : value) {
        items.push_back(convert(item));
      }

      const auto first_type = items[0]->get_type();
      const auto is_homogenous = std::ranges::all_of(
          items, [first_type](const std::unique_ptr<tag::Tag>& item) { return item->get_type() == first_type; });

      if (!is_homogenous) {
        throw std::runtime_error("Cannot convert non-homogenous JSON Array to NBT");
      }

      if (first_type == tag::TagType::Byte) {
        return collect_array<tag::TagByte, int8_t, tag::TagByteArray>(items, &tag::TagByte::get_byte);
      }

      if (first_type == tag::TagType::Int) {
        return collect_array<tag::TagInt, int32_t, tag::TagIntArray>(items, &tag::TagInt::get_int);
      }

      if (first_type == tag::TagType::Long) {
        return collect_array<tag::TagLong, int64_t, tag::TagLongArray>(items, &tag::TagLong::get_long);
      }

      return std::make_unique<tag::TagList>(first_type, std::move(items));
    }
    case Json::objectValue: {
      const auto builder = TagCompoundBuilder::create();

      for (auto& name : value.getMemberNames()) {
        builder->add(name.data(), convert(value[name]));
      }

      return builder->build_compound_ptr();
    }
    [[unlikely]] default: { throw std::runtime_error("Unknown JSON type cannot be converted to NBT"); }
  }
}

tag::TagCompound Json2NBTReader::convertToNbt() const {
  auto bogus = icu::UnicodeString();
  bogus.setToBogus();

  return convertToNbt(bogus);
}

tag::TagCompound Json2NBTReader::convertToNbt(const icu::UnicodeString& name) const {
  const auto builder = TagCompoundBuilder::create(name.isBogus() ? "" : name);

  for (auto& member_name : json_.getMemberNames()) {
    builder->add(member_name.data(), convert(member_name));
  }

  return builder->build_compound();
}
}  // namespace celerity::nbt