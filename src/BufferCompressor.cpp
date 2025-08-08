//
// Created by Jacob Andersen on 7/22/25.
//

#include "BufferCompressor.h"

#include <zlib.h>

namespace celerity {
BufferCompressionType BufferCompressor::detect_compression_type(ByteBuffer& buffer) {
  if (buffer.get_data_length() >= 2) {
    const auto first = buffer.peek_ubyte(0);
    const auto second = buffer.peek_ubyte(1);

    if (first == 0x1f && second == 0x8b) {
      return GZIP;
    }

    if ((first & 0x0f) == 0x08 && (static_cast<uint16_t>(first) << 8 | second) % 31 == 0) {
      return ZLIB;
    }
  }

  return NONE;
}

ByteBuffer& BufferCompressor::compress(ByteBuffer& buffer, const BufferCompressionType type) {
  if (type == NONE) return buffer;

  auto raw_data = buffer.get_bytes();

  buffer.reset();

  z_stream stream{};
  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;

  int window_bits = 15;  // default for zlib
  if (type == GZIP) {
    window_bits += 16;  // add 16 for gzip
  }

  if (const int deflate_init_result =
          deflateInit2(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED, window_bits, 8, Z_DEFAULT_STRATEGY);
      deflate_init_result != Z_OK) {
    throw std::runtime_error("deflateInit2 failed: " + std::to_string(deflate_init_result));
  }

  stream.avail_in = raw_data.size();
  stream.next_in = raw_data.data();

  std::vector<uint8_t> output;
  constexpr int CHUNK_SIZE = 4096;
  uint8_t chunk[CHUNK_SIZE];

  int deflate_result;
  do {
    stream.avail_out = CHUNK_SIZE;
    stream.next_out = chunk;

    deflate_result = deflate(&stream, Z_FINISH);
    if (deflate_result == Z_STREAM_ERROR) {
      deflateEnd(&stream);
      throw std::runtime_error("deflate failed: " + std::to_string(deflate_result));
    }

    output.insert(output.end(), chunk, chunk + (CHUNK_SIZE - stream.avail_out));
  } while (deflate_result != Z_STREAM_END);

  deflateEnd(&stream);

  buffer.set_bytes(output);

  return buffer;
}

ByteBuffer& BufferCompressor::decompress(ByteBuffer& buffer) {
  const auto compression_type = detect_compression_type(buffer);
  if (compression_type == NONE) {
    return buffer;
  }

  auto compressed_data = buffer.get_bytes();

  buffer.reset();

  z_stream stream{};
  stream.zalloc = Z_NULL;
  stream.zfree = Z_NULL;
  stream.opaque = Z_NULL;

  int window_bits = 15;  // default for zlib
  if (compression_type == GZIP) {
    window_bits += 16;  // add 16 for gzip
  }

  if (const int inflate_init_result = inflateInit2(&stream, window_bits); inflate_init_result != Z_OK) {
    throw std::runtime_error("inflateInit2 failed: " + std::to_string(inflate_init_result));
  }

  stream.avail_in = compressed_data.size();
  stream.next_in = compressed_data.data();

  std::vector<uint8_t> output;
  constexpr int CHUNK_SIZE = 4096;
  uint8_t chunk[CHUNK_SIZE];

  int inflate_result;
  do {
    stream.avail_out = CHUNK_SIZE;
    stream.next_out = chunk;

    inflate_result = inflate(&stream, Z_NO_FLUSH);
    if (inflate_result != Z_OK && inflate_result != Z_STREAM_END) {
      inflateEnd(&stream);
      throw std::runtime_error("inflate failed: " + std::to_string(inflate_result));
    }

    output.insert(output.end(), chunk, chunk + (CHUNK_SIZE - stream.avail_out));
  } while (inflate_result != Z_STREAM_END);

  inflateEnd(&stream);

  buffer.set_bytes(output);

  return buffer;
}
}  // namespace celerity