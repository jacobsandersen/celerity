#ifndef CELERITY_NBT_TAG_TAGTYPE_H
#define CELERITY_NBT_TAG_TAGTYPE_H

#include <string>
#include <utility>

namespace celerity::nbt::tag {
class TagType {
 public:
  static const TagType End;
  static const TagType Byte;
  static const TagType Short;
  static const TagType Int;
  static const TagType Long;
  static const TagType Float;
  static const TagType Double;
  static const TagType ByteArray;
  static const TagType String;
  static const TagType List;
  static const TagType Compound;
  static const TagType IntArray;
  static const TagType LongArray;
  static const TagType* Types[13];

  static TagType type_id_to_type(const uint8_t type_id) {
    if (type_id > 12) {
      throw std::invalid_argument("Tried to get Tag Type with Tag Type ID > 12.");
    }

    return *Types[type_id];
  }

  [[nodiscard]] uint8_t get_type_id() const { return type_id_; }

  [[nodiscard]] std::string get_type_name() const { return type_name_; }

  bool operator==(const TagType& rhs) const { return type_id_ == rhs.type_id_; }

  bool operator!=(const TagType& rhs) const { return !operator==(rhs); }

 private:
  TagType(const uint8_t type_id, std::string type_name) : type_id_(type_id), type_name_(std::move(type_name)) {}

  uint8_t type_id_;
  std::string type_name_;
};

inline const TagType TagType::End{0, "TAG_End"};
inline const TagType TagType::Byte{1, "TAG_Byte"};
inline const TagType TagType::Short{2, "TAG_Short"};
inline const TagType TagType::Int{3, "TAG_Int"};
inline const TagType TagType::Long{4, "TAG_Long"};
inline const TagType TagType::Float{5, "TAG_Float"};
inline const TagType TagType::Double{6, "TAG_Double"};
inline const TagType TagType::ByteArray{7, "TAG_ByteArray"};
inline const TagType TagType::String{8, "TAG_String"};
inline const TagType TagType::List{9, "TAG_List"};
inline const TagType TagType::Compound{10, "TAG_Compound"};
inline const TagType TagType::IntArray{11, "TAG_IntArray"};
inline const TagType TagType::LongArray{12, "TAG_LongArray"};
inline const TagType* TagType::Types[13] = {
    &TagType::End,      &TagType::Byte,     &TagType::Short,     &TagType::Int,    &TagType::Long,
    &TagType::Float,    &TagType::Double,   &TagType::ByteArray, &TagType::String, &TagType::List,
    &TagType::Compound, &TagType::IntArray, &TagType::LongArray};
}  // namespace celerity::nbt::tag

#endif
