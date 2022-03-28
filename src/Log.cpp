#include "Log.h"

Log::Log(IPAddress local, HardwareSerial *serial)
{
    _disabled = false;

    _serial = serial;

    _startTime = millis();
    u32_t broadcast = local.v4() | 0xFF000000;
    _local = local;
    _remote = IPAddress(broadcast);

    _udp = WiFiUDP();
}

Log::Log()
{
    _disabled = true;
}

void Log::WriteDebug(const char *format, ...)
{
    if (_disabled)
        return;

    auto ptr = WriteTime(_buffer);

    va_list args;
    va_start(args, format);
    ptr += vsprintf(_buffer + ptr, format, args);
    va_end(args);

    WriteImpl(ptr);
}

void Log::WriteDebug(const char *prefix, const u8_t *data, size_t dataSize)
{
    if (_disabled)
        return;

    auto ptr = WriteTime(_buffer);
    size_t prefixLength = std::strlen(prefix);
    std::memcpy(_buffer + ptr, prefix, std::strlen(prefix));
    ptr += prefixLength;
    _buffer[ptr++] = ':';
    for (size_t i = 0; i < dataSize; i++)
    {
        auto size = sprintf(_buffer + ptr, " %02X", data[i]);
        ptr += size;
    }

    WriteImpl(ptr);
}

void Log::WriteImpl(size_t size)
{
    if (_udp.beginPacketMulticast(_remote, _debugPort, _local, 2))
    {
        _udp.write(_buffer, size);
        _udp.endPacket();
    }
    else
    {
        if(_serial != nullptr)
            _serial->println("udp error");
    }

    if (_serial != nullptr)
    {
        _buffer[size++] = '\r';
        _buffer[size++] = '\n';
        _serial->write(_buffer, size);
    }
}

size_t Log::WriteTime(char *buffer)
{
    auto time = millis() - _startTime;
    uint32_t ss = time / 1000;
    uint32_t ms = time % 1000;
    uint32_t mm = ss / 60;
    uint32_t hh = mm / 60;
    mm = mm % 60;
    ss = ss % 60;

    return sprintf(_buffer, "%02d:%02d:%02d.%03d ", hh, mm, ss, ms);
}

Log::~Log()
{
}