#include "VarInt.h"

#include <iostream>

constexpr int32_t SEGMENT_BITS = 0x7F;
constexpr int32_t CONTINUE_BIT = 0x80;

namespace celerity {
int32_t VarInt::decode_varint(ByteBuffer* buf, uint8_t* bytes_read) {
  int32_t value = 0;
  int8_t position = 0;

  if (bytes_read) *bytes_read = 0;

  while (true) {
    const int8_t currentByte = buf->read_byte();
    if (bytes_read) (*bytes_read)++;

    const auto byteValue = currentByte & SEGMENT_BITS;
    value |= byteValue << position;

    if ((currentByte & CONTINUE_BIT) == 0) {
      break;
    }

    position += 7;

    if (position >= 32) {
      throw std::runtime_error("VarInt is too big");
    }
  }

  return value;
}

void VarInt::encode_varint(int32_t value, int8_t* buffer, uint8_t* bytes_written) {
  int8_t position = 0;

  if (bytes_written) *bytes_written = 0;

  while (true) {
    buffer[position] = static_cast<int8_t>(value & SEGMENT_BITS);
    value >>= 7;

    if (value == 0) {
      if (bytes_written) (*bytes_written)++;
      return;
    }

    buffer[position] |= static_cast<int8_t>(CONTINUE_BIT);
    ++position;
    if (bytes_written) (*bytes_written)++;
  }
}

int64_t VarInt::decode_varlong(ByteBuffer* buf, uint8_t* bytes_read) {
  int64_t value = 0;
  int8_t position = 0;

  if (bytes_read) *bytes_read = 0;

  while (true) {
    const int8_t currentByte = buf->read_byte();
    if (bytes_read) (*bytes_read)++;

    const auto byteValue = static_cast<int64_t>(currentByte & SEGMENT_BITS);
    value |= byteValue << position;

    if ((currentByte & CONTINUE_BIT) == 0) {
      break;
    }

    position += 7;

    if (position >= 64) {
      throw std::runtime_error("VarLong is too big");
    }
  }

  return value;
}

void VarInt::encode_varlong(int64_t value, int8_t* buffer, uint8_t* bytes_written) {
  int position = 0;

  if (bytes_written) *bytes_written = 0;

  while (true) {
    buffer[position] = static_cast<int8_t>(value & SEGMENT_BITS);
    value >>= 7;

    if (value == 0) {
      if (bytes_written) (*bytes_written)++;
      return;
    }

    buffer[position] |= static_cast<int8_t>(CONTINUE_BIT);
    ++position;
    if (bytes_written) (*bytes_written)++;
  }
}

uint8_t VarInt::encoding_length(int64_t value) {
  int length = 1;

  while ((value & ~SEGMENT_BITS) != 0) {
    value >>= 7;
    ++length;
  }

  return length;
}
}  // namespace celerity