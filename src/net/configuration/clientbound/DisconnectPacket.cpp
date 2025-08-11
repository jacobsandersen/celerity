#include "DisconnectPacket.h"

#include <json/value.h>

#include "src/nbt/NBTWriter.h"
#include "src/nbt/TagCompoundBuilder.h"
#include "src/nbt/TagListBuilder.h"
#include "src/nbt/tag/TagByte.h"
#include "src/nbt/tag/TagString.h"

namespace celerity::net::configuration::client {
ByteBuffer DisconnectPacket::encode() const {
  ByteBuffer buf;
  const nbt::NBTWriter writer(buf);

  const auto component = nbt::TagCompoundBuilder::create()
                             ->add("type", nbt::tag::TagString("text"))
                             ->add("text", nbt::tag::TagString("Disconnected during configuration: "))
                             ->add("extra", nbt::TagListBuilder<nbt::tag::TagString>::create(
                                                nbt::tag::TagString(icu::UnicodeString(reason_.data())))
                                                ->build_list())
                             ->add("color", nbt::tag::TagString("red"))
                             ->add("bold", nbt::tag::TagByte(1))
                             ->build();

  writer.write_tag(component);
  return buf;
}
}  // namespace celerity::net::configuration::client
