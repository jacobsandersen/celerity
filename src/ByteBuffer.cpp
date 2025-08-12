#include "ByteBuffer.h"

#include <absl/log/log.h>
#include <unicode/schriter.h>
#include <zlib.h>

#include <boost/endian/conversion.hpp>
#include <cstring>
#include <queue>
#include <stdexcept>
#include <utility>

#include "VarInt.h"
#include "nbt/tag/TagType.h"

namespace celerity {
template <typename T>
typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value, void>::type
ByteBuffer::write_buffer(std::deque<uint8_t>& buffer, T value) {
  buffer.insert(buffer.end(), reinterpret_cast<uint8_t*>(&value), reinterpret_cast<uint8_t*>(&value) + sizeof(T));
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value, void>::type
ByteBuffer::write_buffer(std::deque<uint8_t>& buffer, T value, uint32_t offset) {
  size_t size = sizeof(T);
  offset = offset * size;

  if (buffer.size() < offset + size) {
    buffer.resize(offset + size);
  }

  std::memcpy(&buffer[offset], reinterpret_cast<uint8_t*>(&value), size);
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value,
                        T>::type static read_or_peek_buffer(std::deque<uint8_t>& buffer, uint32_t offset, bool erase) {
  size_t size = sizeof(T);
  offset = offset * size;

  if (buffer.empty() || buffer.size() < offset + size) {
    throw std::runtime_error("Buffer too small");
  }

  T value;
  std::memcpy(&value, &buffer[offset], size);

  if (erase) buffer.erase(buffer.begin() + offset, buffer.begin() + offset + static_cast<std::ptrdiff_t>(size));

  return value;
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value, T>::type
ByteBuffer::read_buffer(std::deque<uint8_t>& buffer, uint32_t offset) {
  return read_or_peek_buffer<T>(buffer, offset, true);
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value, T>::type
ByteBuffer::read_buffer(std::deque<uint8_t>& buffer) {
  return read_buffer<T>(buffer, 0);
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value, T>::type
ByteBuffer::peek_buffer(std::deque<uint8_t>& buffer, uint32_t offset) {
  return read_or_peek_buffer<T>(buffer, offset, false);
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value || std::is_floating_point<T>::value, T>::type
ByteBuffer::peek_buffer(std::deque<uint8_t>& buffer) {
  return peek_buffer<T>(buffer, 0);
}

void ByteBuffer::write_boolean(bool value) { write_byte(value ? 0x1 : 0x0); }

bool ByteBuffer::read_boolean() { return read_byte() == 0x1; }

void ByteBuffer::write_byte(int8_t value) { write_buffer(m_data, value); }

int8_t ByteBuffer::read_byte() { return read_buffer<int8_t>(m_data); }

int8_t ByteBuffer::peek_byte() { return peek_byte(0); }

int8_t ByteBuffer::peek_byte(uint32_t offset) { return peek_buffer<int8_t>(m_data, offset); }

void ByteBuffer::write_bytes(const std::vector<int8_t>& bytes) {
  for (int8_t byte : bytes) {
    write_byte(byte);
  }
}

void ByteBuffer::write_bytes(const int8_t* bytes, size_t num_bytes) {
  for (int i = 0; i < num_bytes; i++) {
    write_byte(bytes[i]);
  }
}

std::vector<int8_t> ByteBuffer::read_bytes(size_t num_bytes) {
  std::vector<int8_t> vec;

  for (int i = 0; i < num_bytes; i++) {
    vec.push_back(read_byte());
  }

  return vec;
}

std::vector<int8_t> ByteBuffer::peek_bytes(const size_t num_bytes) {
  std::vector<int8_t> vec;
  for (size_t i = 0; i < num_bytes; i++) {
    vec.push_back(peek_byte(i));
  }
  return vec;
}

void ByteBuffer::write_ubyte(uint8_t value) { write_buffer(m_data, value); }

void ByteBuffer::write_ubyte(uint8_t value, uint32_t offset) { write_buffer(m_data, value, offset); }

uint8_t ByteBuffer::read_ubyte() { return read_buffer<uint8_t>(m_data); }

uint8_t ByteBuffer::peek_ubyte() { return peek_ubyte(0); }

uint8_t ByteBuffer::peek_ubyte(uint32_t offset) { return peek_buffer<uint8_t>(m_data, offset); }

void ByteBuffer::write_ubytes(const std::vector<uint8_t>& ubytes) {
  for (uint8_t ubyte : ubytes) {
    write_ubyte(ubyte);
  }
}

void ByteBuffer::write_ubytes(const uint8_t* bytes, size_t num_bytes) {
  for (int i = 0; i < num_bytes; i++) {
    write_ubyte(bytes[i]);
  }
}

std::vector<uint8_t> ByteBuffer::read_ubytes(size_t num_ubytes) {
  std::vector<uint8_t> vec;

  for (int i = 0; i < num_ubytes; i++) {
    vec.push_back(read_ubyte());
  }

  return vec;
}

std::vector<uint8_t> ByteBuffer::peek_ubytes(const size_t num_ubytes) {
  const auto bytes_to_read = std::min(static_cast<uint32_t>(num_ubytes), get_data_length());

  std::vector<uint8_t> vec;
  vec.reserve(bytes_to_read);
  for (size_t i = 0; i < bytes_to_read; i++) {
    vec.push_back(peek_ubyte(i));
  }

  return vec;
}

void ByteBuffer::write_short(int16_t value) { write_buffer(m_data, value); }

int16_t ByteBuffer::read_short() { return read_buffer<int16_t>(m_data); }

void ByteBuffer::write_be_short(int16_t value) { write_short(boost::endian::native_to_big(value)); }

int16_t ByteBuffer::read_be_short() { return boost::endian::big_to_native(read_short()); }

void ByteBuffer::write_ushort(uint16_t value) { write_buffer(m_data, value); }

uint16_t ByteBuffer::read_ushort() { return read_buffer<uint16_t>(m_data); }

void ByteBuffer::write_be_ushort(uint16_t value) { write_ushort(boost::endian::native_to_big(value)); }

uint16_t ByteBuffer::read_be_ushort() { return boost::endian::big_to_native(read_short()); }

void ByteBuffer::write_int(int32_t value) { write_buffer(m_data, value); }

int32_t ByteBuffer::read_int() { return read_buffer<int32_t>(m_data); }

void ByteBuffer::write_be_int(int32_t value) { write_int(boost::endian::native_to_big(value)); }

int32_t ByteBuffer::read_be_int() { return boost::endian::big_to_native(read_int()); }

void ByteBuffer::write_uint(uint32_t value) { write_buffer(m_data, value); }

uint32_t ByteBuffer::read_uint() { return read_buffer<uint32_t>(m_data); }

void ByteBuffer::write_be_uint(uint32_t value) { write_uint(boost::endian::native_to_big(value)); }

uint32_t ByteBuffer::read_be_uint() { return boost::endian::big_to_native(read_int()); }

void ByteBuffer::write_long(int64_t value) { write_buffer(m_data, value); }

int64_t ByteBuffer::read_long() { return read_buffer<int64_t>(m_data); }

void ByteBuffer::write_be_long(int64_t value) { write_long(boost::endian::native_to_big(value)); }

int64_t ByteBuffer::read_be_long() { return boost::endian::big_to_native(read_long()); }

void ByteBuffer::write_ulong(uint64_t value) { write_buffer(m_data, value); }

uint64_t ByteBuffer::read_ulong() { return read_buffer<uint64_t>(m_data); }

void ByteBuffer::write_be_ulong(uint64_t value) { write_ulong(boost::endian::native_to_big(value)); }

uint64_t ByteBuffer::read_be_ulong() { return boost::endian::big_to_native(read_long()); }

void ByteBuffer::write_float(float value) { write_buffer(m_data, value); }

float ByteBuffer::read_float() { return read_buffer<float>(m_data); }

void ByteBuffer::write_be_float(float value) {
  uint32_t encoded;
  std::memcpy(&encoded, &value, sizeof(encoded));
  write_be_uint(encoded);
}

float ByteBuffer::read_be_float() {
  uint32_t encoded = read_be_uint();
  float decoded;
  std::memcpy(&decoded, &encoded, sizeof(decoded));
  return decoded;
}

void ByteBuffer::write_double(double value) { write_buffer(m_data, value); }

double ByteBuffer::read_double() { return read_buffer<double>(m_data); }

void ByteBuffer::write_be_double(double value) {
  uint64_t encoded;
  std::memcpy(&encoded, &value, sizeof(encoded));
  write_be_ulong(encoded);
}

double ByteBuffer::read_be_double() {
  uint64_t encoded = read_be_ulong();
  double decoded;
  std::memcpy(&decoded, &encoded, sizeof(decoded));
  return decoded;
}

void ByteBuffer::write_string(const std::string& str) {
  write_varint(static_cast<int32_t>(str.length()));
  for (const int8_t byte : str) {
    write_byte(byte);
  }
}

std::string ByteBuffer::read_string() {
  int32_t length = read_varint();
  std::string s;

  for (int i = 0; i < length; i++) {
    s.push_back(read_byte());
  }

  return s;
}

void ByteBuffer::write_string_modified_utf8(const icu::UnicodeString& str) {
  write_be_ushort(str.length());

  icu::StringCharacterIterator itr(str);
  itr.setToStart();
  while (itr.hasNext()) {
    UChar32 next = itr.next32PostInc();
    if (next >= u'\u0001' && next <= u'\u007f') {
      write_byte(static_cast<int8_t>(next));
    } else if (next == u'\u0000' || (next >= u'\u0080' && next <= u'\u07ff')) {
      write_byte(static_cast<int8_t>(0xc0 | (0x1f & (next >> 6))));
      write_byte(static_cast<int8_t>(0x80 | (0x3f & next)));
    } else if (next >= u'\u0800' && next <= u'\uffff') {
      write_byte(static_cast<int8_t>(0xe0 | (0x0f & (next >> 12))));
      write_byte(static_cast<int8_t>(0x80 | (0x3f & (next >> 6))));
      write_byte(static_cast<int8_t>(0x80 | (0x3f & next)));
    }
  }
}

icu::UnicodeString ByteBuffer::read_string_modified_utf8() {
  const uint16_t length = read_be_ushort();

  std::queue<int8_t> bytes;
  for (int i = 0; i < length; i++) {
    bytes.push(read_byte());
  }

  icu::UnicodeString str;

  while (!bytes.empty()) {
    const int8_t first_byte = bytes.front();
    bytes.pop();

    if ((first_byte >> 4) == 0b1111 || (first_byte >> 6) == 0b10) {
      throw std::invalid_argument("First byte in modified UTF-8 group did not match expected pattern.");
    }

    if ((first_byte >> 4) == 0b1110) {
      if (bytes.empty()) {
        throw std::invalid_argument("Expected 2nd byte in 3-byte modified UTF-8 group, found only 1.");
      }

      int8_t second_byte = bytes.front();
      bytes.pop();

      if (bytes.empty()) {
        throw std::invalid_argument("Expected 3rd byte in 3-byte modified UTF-8 group, found only 2.");
      }

      int8_t third_byte = bytes.front();
      bytes.pop();

      if ((second_byte >> 6) != 0b10 || (third_byte >> 6) != 0b10) {
        throw std::invalid_argument(
            "2nd or 3rd byte in 3-byte modified UTF-8 group did not match "
            "expected pattern.");
      }

      str.append(((first_byte & 0x0F) << 12) | ((second_byte & 0x3F) << 6) | (third_byte & 0x3F));
    } else if ((first_byte >> 5) == 0b110) {
      if (bytes.empty()) {
        throw std::invalid_argument("Expected 2nd byte in 2-byte modified UTF-8 group, found only 1.");
      }

      int8_t second_byte = bytes.front();
      bytes.pop();

      if ((second_byte >> 6) != 0b10) {
        throw std::invalid_argument(
            "2nd byte in 2-byte modified UTF-8 group did not match expected "
            "pattern.");
      }

      str.append(((first_byte & 0x1F) << 6) | (second_byte & 0x3F));
    } else if ((first_byte >> 7) == 0) {
      str.append(first_byte);
    }
  }

  return str;
}

void ByteBuffer::write_varint(int32_t value) {
  auto* encoded = new int8_t[5];
  uint8_t bytes_written;

  VarInt::encode_varint(value, encoded, &bytes_written);

  for (int i = 0; i < bytes_written; i++) {
    write_byte(encoded[i]);
  }
}

int32_t ByteBuffer::read_varint() { return read_varint(nullptr); }

int32_t ByteBuffer::read_varint(uint8_t* bytes_read) { return VarInt::decode_varint(this, bytes_read); }

std::optional<std::pair<int32_t, uint8_t>> ByteBuffer::peek_varint() {
  try {
    ByteBuffer tmp(peek_ubytes(5));  // max size of var int is 5 bytes
    uint8_t num_bytes = 0;
    const int32_t varint = tmp.read_varint(&num_bytes);
    return std::pair(varint, num_bytes);
  } catch (std::exception&) {
    return std::nullopt;
  }
}

void ByteBuffer::write_varlong(int64_t value) {
  int8_t* encoded;
  uint8_t bytes_written;

  VarInt::encode_varlong(value, encoded, &bytes_written);

  for (int i = 0; i < bytes_written; i++) {
    write_byte(encoded[i]);
  }
}

int64_t ByteBuffer::read_varlong() { return VarInt::decode_varlong(this, nullptr); }

uuids::uuid ByteBuffer::read_uuid() {
  uint64_t most_significant = read_ulong();
  uint64_t least_significant = read_ulong();
  std::array<uuids::uuid::value_type, 16> uuid_bytes{};
  std::memcpy(uuid_bytes.data(), &most_significant, sizeof(uint64_t));
  std::memcpy(uuid_bytes.data() + 8, &least_significant, sizeof(uint64_t));
  return {uuid_bytes};
}

void ByteBuffer::write_uuid(uuids::uuid unique_id) {
  auto bytes = unique_id.as_bytes();
  uint64_t most_significant, least_significant;
  std::memcpy(&most_significant, bytes.data(), sizeof(uint64_t));
  std::memcpy(&least_significant, bytes.data() + 8, sizeof(uint64_t));
  write_ulong(most_significant);
  write_ulong(least_significant);
}

void ByteBuffer::append(const ByteBuffer& buffer) { m_data.append_range(buffer.get_bytes()); }

std::deque<uint8_t> ByteBuffer::get_data() const { return m_data; }

std::vector<uint8_t> ByteBuffer::get_bytes() const { return {m_data.begin(), m_data.end()}; }

void ByteBuffer::set_data(std::deque<uint8_t> data) { m_data = std::move(data); }

void ByteBuffer::set_bytes(std::vector<uint8_t> bytes) {
  m_data.clear();
  m_data.insert(m_data.end(), bytes.begin(), bytes.end());
}

uint32_t ByteBuffer::get_data_length() const { return m_data.size(); }

nbt::tag::TagType ByteBuffer::read_nbt_tag_type() { return nbt::tag::TagType::type_id_to_type(read_ubyte()); }

void ByteBuffer::reset() { m_data.clear(); }

std::string ByteBuffer::to_hex_string() const {
  std::ostringstream oss;
  oss << std::hex << std::uppercase << std::setfill('0');
  for (const uint8_t& byte : get_bytes()) {
    oss << std::setw(2) << static_cast<int>(byte) << ' ';
  }
  return oss.str();
}
void ByteBuffer::truncate_front(const size_t num_bytes) { m_data.erase(m_data.begin(), m_data.begin() + num_bytes); }
}  // namespace celerity