#ifndef MCCPP_PACKETLOGINOUTLOGINPLUGINREQUEST_H
#define MCCPP_PACKETLOGINOUTLOGINPLUGINREQUEST_H

#include <cstdint>
#include <memory>
#include <utility>
#include "../../Connection.h"
#include "../../OutboundPacket.h"

class PacketLoginOutLoginPluginRequest : public OutboundPacket {
public:
    PacketLoginOutLoginPluginRequest(int32_t message_id, bool successful, std::vector<uint8_t> bytes) :
            OutboundPacket(0x04),
            m_message_id(message_id),
            m_successful(successful),
            m_bytes(std::move(bytes)) {}

    void write_data(const std::shared_ptr<Connection> &conn, ByteBuffer &buffer) override;
private:
    int32_t m_message_id;
    bool m_successful;
    std::vector<uint8_t> m_bytes;
};


#endif
