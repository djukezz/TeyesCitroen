#include "Log.h"
#include <stdlib.h>
#include <stdio.h>

Log::Log(IPAddress local, HardwareSerial *serial)
{
    _disabled = false;

    _serial = serial;

    _startTime = millis();
    u32_t broadcast = local.v4() | 0xFF000000;
    _local = local;
    _remote = IPAddress(broadcast);
    _udp = WiFiUDP();

    _count = 0;
}

Log::Log()
{
    _disabled = true;
}

void Log::FlushDebug()
{
    WriteImpl();
    _count = 0;
}

void Log::AppendDebug()
{
}

void Log::WriteImpl()
{
    if(_count == 0)
        return;

    if (_udp.beginPacketMulticast(_remote, _debugPort, _local, 2))
    {
        _udp.write(_buffer, _count);
        _udp.endPacket();
    }
    else
    {
        if(_serial != nullptr)
            _serial->println("udp error");
    }

    if (_serial != nullptr)
    {
        _buffer[_count++] = '\r';
        _buffer[_count++] = '\n';
        _serial->write(_buffer, _count);
    }
}

size_t Log::WriteTime()
{
    auto time = millis() - _startTime;
    uint32_t ss = time / 1000;
    uint32_t ms = time % 1000;
    uint32_t mm = ss / 60;
    uint32_t hh = mm / 60;
    mm = mm % 60;
    ss = ss % 60;

    auto size = sprintf(_buffer + _count, "%02d:%02d:%02d.%03d ", hh, mm, ss, ms);
    _count += size;
    return size;
}

Log::~Log()
{
}