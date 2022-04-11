#pragma once

#include "Updatable.h"
#include "Log.h"
#include "ButtonCanMessage.h"
#include "UnknownCanMessage.h"

class UdpCommandReceiver final : public Updatable
{
public:
    UdpCommandReceiver(std::function<void(CanMessageBase *)> callback, uint16_t port = 6666)
    {
        _udp.begin(port);
    }
    void Update() override
    {
        int packetSize = _udp.parsePacket();
        if (packetSize <= 0 || packetSize > 32)
            return;

        char buff[packetSize + 1];
        _udp.read(buff, packetSize);
        buff[packetSize] = 0;

        if (packetSize == 3 && buff[0] == 'b')
        {
            char *end;
            auto b = static_cast<Buttons>(strtoul(buff + 1, &end, 16));
            Log::GetInstance()->WriteDebug("REMOTE BUTTON %s", ButtonEx::ToString(b));
            ButtonCanMessage msg(b);
            _callback(&msg);
        }
        else if (packetSize == 5 && buff[0] == 't')
        {
            auto param = strtoul(buff, nullptr, 16);
            u8 t = param & 0xFF;
            u8 msgType = (param >> 8);
            Log::GetInstance()->WriteDebug("REMOTE T %02X", msgType);
            u8 msgData[5]{Constants::Prefix, 0x04, msgType, t};
            UnknownCanMessage::CalcCheckSum(msgData, sizeof(msgData));
            UnknownCanMessage msg(msgData, sizeof(msgData));
            _callback(&msg);
        }
    }

private:
    static const int _port;
    std::function<void(CanMessageBase *)> _callback;
    WiFiUDP _udp;
};