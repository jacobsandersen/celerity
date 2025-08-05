cmake_minimum_required(VERSION 3.31)
project(celerity)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)

find_package(boost_asio CONFIG REQUIRED)
find_package(boost_endian CONFIG REQUIRED)
find_package(boost_any CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(cryptopp CONFIG REQUIRED)
find_package(stduuid CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(toml11 CONFIG REQUIRED)
find_package(absl CONFIG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(ICU REQUIRED IMPORTED_TARGET icu-i18n icu-io icu-uc)

add_executable(celerity src/main.cpp)

target_sources(celerity PRIVATE
        src/MinecraftServer.cpp
        src/ConfigManager.cpp
        src/ServerConfig.cpp
        src/VarInt.cpp
        src/RSAKeypair.cpp
        src/ByteBuffer.cpp
        src/BasicTimer.cpp
        src/KnownPack.cpp
        src/BufferCompressor.cpp
        src/BufferCrypter.cpp
        src/util/UUIDUtil.cpp
        src/player/Player.cpp
        src/player/MojangProfile.cpp
        src/player/ClientInformation.cpp
        src/net/Connection.cpp
        src/net/NetworkManager.cpp
        src/net/HttpClient.cpp
        src/net/handshaking/HandshakingPacketRegistry.cpp
        src/net/handshaking/IntentionPacket.cpp
        src/net/status/StatusPacketRegistry.cpp
        src/net/status/StatusRequestPacket.cpp
        src/net/status/StatusResponsePacket.cpp
        src/net/status/PingRequestPacket.cpp
        src/net/status/PongResponsePacket.cpp
        src/net/login/in/PacketLoginInLoginStart.cpp
        src/net/login/in/PacketLoginInEncryptionResponse.cpp
        src/net/login/in/PacketLoginInLoginAcknowledged.cpp
        src/net/login/in/PacketLoginInLoginPluginResponse.cpp
        src/net/login/out/PacketLoginOutEncryptionRequest.cpp
        src/net/login/out/PacketLoginOutDisconnect.cpp
        src/net/login/out/PacketLoginOutLoginSuccess.cpp
        src/net/login/out/PacketLoginOutLoginPluginRequest.cpp
        src/net/login/out/PacketLoginOutSetCompression.cpp
        src/net/configuration/in/PacketConfigurationInClientInformation.cpp
        src/net/configuration/in/PacketConfigurationInPluginMessage.cpp
        src/net/configuration/in/PacketConfigurationInKeepAlive.cpp
        src/net/configuration/out/PacketConfigurationOutKeepAlive.cpp
        src/net/configuration/out/PacketConfigurationOutDisconnect.cpp
        src/net/configuration/out/PacketConfigurationOutRegistryData.cpp
        src/net/configuration/out/PacketConfigurationOutFeatureFlags.cpp
        src/net/configuration/out/PacketConfigurationOutKnownPacks.cpp
        src/net/configuration/in/PacketConfigurationInKnownPacks.cpp
        src/nbt/NBTReader.cpp
        src/nbt/NBTWriter.cpp
)

target_sources(celerity PRIVATE FILE_SET nbt_headers TYPE HEADERS FILES
        src/nbt/Concepts.h
        src/nbt/TagListBuilder.h
        src/nbt/TagCompoundBuilder.h
        src/nbt/tag/TagType.h
        src/nbt/tag/NamedTag.h
        src/nbt/tag/Tag.h
        src/nbt/tag/TagEnd.h
        src/nbt/tag/TagByte.h
        src/nbt/tag/TagShort.h
        src/nbt/tag/TagInt.h
        src/nbt/tag/TagLong.h
        src/nbt/tag/TagFloat.h
        src/nbt/tag/TagDouble.h
        src/nbt/tag/TagByteArray.h
        src/nbt/tag/TagString.h
        src/nbt/tag/TagList.h
        src/nbt/tag/TagCompound.h
        src/nbt/tag/TagIntArray.h
        src/nbt/tag/TagLongArray.h
)

target_link_libraries(celerity PRIVATE
        Boost::asio Boost::endian Boost::any JsonCpp::JsonCpp cryptopp::cryptopp stduuid
        CURL::libcurl toml11::toml11 absl::log ZLIB::ZLIB
        PkgConfig::ICU
)