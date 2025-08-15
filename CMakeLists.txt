cmake_minimum_required(VERSION 3.31)
project(celerity)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)

find_package(boost_asio CONFIG REQUIRED)
find_package(boost_endian CONFIG REQUIRED)
find_package(boost_any CONFIG REQUIRED)
find_package(boost_container CONFIG REQUIRED)
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
        src/VarInt.cpp
        src/ServerConfig.cpp
        src/RSAKeypair.cpp
        src/MinecraftServer.cpp
        src/KnownPack.cpp
        src/ConfigManager.cpp
        src/ByteBuffer.cpp
        src/BufferCrypter.cpp
        src/BufferCompressor.cpp
        src/Scheduler.cpp
        src/Identifier.cpp
        src/registry/Registry.cpp
        src/util/RandUtil.cpp
        src/util/UUIDUtil.cpp
        src/player/MojangProfile.cpp
        src/player/Player.cpp
        src/task/configuration/ConfigurationTask.cpp
        src/task/configuration/KeepAliveKillerTask.cpp
        src/task/configuration/KeepAliveTask.cpp
        src/task/configuration/RegistryDataTask.cpp
        src/net/NetworkManager.cpp
        src/net/HttpClient.cpp
        src/net/Connection.cpp
        src/net/handshaking/HandshakingPacketRegistry.cpp
        src/net/handshaking/serverbound/IntentionPacket.cpp
        src/net/status/StatusPacketRegistry.cpp
        src/net/status/clientbound/StatusResponsePacket.cpp
        src/net/status/clientbound/PongResponsePacket.cpp
        src/net/status/serverbound/StatusRequestPacket.cpp
        src/net/status/serverbound/PingRequestPacket.cpp
        src/net/login/LoginPacketRegistry.cpp
        src/net/login/clientbound/CookieRequestPacket.cpp
        src/net/login/clientbound/CustomQueryPacket.cpp
        src/net/login/clientbound/HelloPacket.cpp
        src/net/login/clientbound/LoginCompressionPacket.cpp
        src/net/login/clientbound/LoginDisconnectPacket.cpp
        src/net/login/clientbound/LoginFinishedPacket.cpp
        src/net/login/serverbound/CookieResponsePacket.cpp
        src/net/login/serverbound/CustomQueryAnswerPacket.cpp
        src/net/login/serverbound/HelloPacket.cpp
        src/net/login/serverbound/KeyPacket.cpp
        src/net/login/serverbound/LoginAcknowledgedPacket.cpp
        src/net/configuration/ConfigurationPacketRegistry.cpp
        src/net/configuration/clientbound/ClearDialogPacket.cpp
        src/net/configuration/clientbound/CookieRequestPacket.cpp
        src/net/configuration/clientbound/CustomPayloadPacket.cpp
        src/net/configuration/clientbound/CustomReportDetailsPacket.cpp
        src/net/configuration/clientbound/DisconnectPacket.cpp
        src/net/configuration/clientbound/FinishConfigurationPacket.cpp
        src/net/configuration/clientbound/KeepAlivePacket.cpp
        src/net/configuration/clientbound/PingPacket.cpp
        src/net/configuration/clientbound/RegistryDataPacket.cpp
        src/net/configuration/clientbound/ResetChatPacket.cpp
        src/net/configuration/clientbound/ResourcePackPopPacket.cpp
        src/net/configuration/clientbound/ResourcePackPushPacket.cpp
        src/net/configuration/clientbound/SelectKnownPacksPacket.cpp
        src/net/configuration/clientbound/ServerLinksPacket.cpp
        src/net/configuration/clientbound/ShowDialogPacket.cpp
        src/net/configuration/clientbound/StoreCookiePacket.cpp
        src/net/configuration/clientbound/TransferPacket.cpp
        src/net/configuration/clientbound/UpdateEnabledFeaturesPacket.cpp
        src/net/configuration/clientbound/UpdateTagsPacket.cpp
        src/net/configuration/serverbound/ClientInformationPacket.cpp
        src/net/configuration/serverbound/CookieResponsePacket.cpp
        src/net/configuration/serverbound/CustomClickActionPacket.cpp
        src/net/configuration/serverbound/CustomPayloadPacket.cpp
        src/net/configuration/serverbound/FinishConfigurationPacket.cpp
        src/net/configuration/serverbound/KeepAlivePacket.cpp
        src/net/configuration/serverbound/PongPacket.cpp
        src/net/configuration/serverbound/ResourcePackPacket.cpp
        src/net/configuration/serverbound/SelectKnownPacksPacket.cpp
        src/net/play/PlayPacketRegistry.cpp
        src/nbt/NBTWriter.cpp
        src/nbt/NBTReader.cpp
        src/nbt/Json2NBTReader.cpp
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
        Boost::asio Boost::endian Boost::any Boost::container JsonCpp::JsonCpp
        cryptopp::cryptopp stduuid CURL::libcurl toml11::toml11 absl::log absl::log_initialize
        ZLIB::ZLIB PkgConfig::ICU
)