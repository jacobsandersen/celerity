#include <absl/log/globals.h>
#include <absl/log/initialize.h>

#include <iostream>

#include "MinecraftServer.h"
#include "nbt/NBTWriter.h"
#include "nbt/TagCompoundBuilder.h"
#include "nbt/tag/TagString.h"

int main(int argc, char* argv[]) {
  absl::SetStderrThreshold(absl::LogSeverity::kInfo);
  absl::InitializeLog();

  celerity::ByteBuffer my_buffer;
  celerity::nbt::NBTWriter writer(my_buffer);

  auto test = celerity::nbt::TagCompoundBuilder::create("hello world")
                  ->add("name", celerity::nbt::tag::TagString("Bananrama"))
                  ->build();

  writer.write_tag(test);

  LOG(INFO) << "Test NBT write: " << my_buffer.to_hex_string();

  celerity::MinecraftServer::get_server()->start();
}
