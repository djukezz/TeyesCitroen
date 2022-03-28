#pragma once

#include <cstring>

#include "CanMessageBase.h"
#include "MessageTypes.h"
#include "Buttons.h"

class CondinionerCanMessage : public CanMessageBase
{
// FD 0C 21    C8 00 34 20 00 00 40 00 00    89
//                   t1 t2
// t = v/2 - 2
// In Celsius        24 14                  

public:
    CondinionerCanMessage(u8 *ptr)
    {
        std::memcpy(_data, ptr + Constants::DataOffset, PayloadSize);
    }

    static const u8 PayloadSize = 9;
    static const u8 TotalSize = 1 + 1 + 1 + PayloadSize + 1;

    static const MessageTypes MessageType = MessageTypes::AIR_CONDITIONING_INFO;
    Buttons Button;
    size_t CopyTo(u8 *dst) override
    {
        u8 *ptr = dst;
        *ptr++ = Constants::Prefix;
        const size_t size = 1 + 1 + PayloadSize + 1;
        *ptr++ = static_cast<u8>(size);
        *ptr++ = (u8)MessageType;
        std::memcpy(ptr, _data, PayloadSize);
        ptr += sizeof(_data);
        u8 checkSum = CalcCheckSum(dst, size);
        *ptr = checkSum;

        return static_cast<u8>(size) + 1;
    }

    void FixTemperatures()
    {
        // магнитола завышает температуру на 2 градуса, т.е. на 4 отсчета
        const u8 rawDelta = 4;
        u8 t1 = _data[2];
        u8 t2 = _data[3];
        t1 = t1 > rawDelta ? t1 - rawDelta : 0;
        t2 = t2 > rawDelta ? t2 - rawDelta : 0;
        _data[2] = t1;
        _data[3] = t2;
    }

    float GetT1()
    {
        return _data[2] * 0.5f - 2;
    }

    float GetT2()
    {
        return _data[3] * 0.5f - 2;
    }

private:
    u8 _data[PayloadSize];
};