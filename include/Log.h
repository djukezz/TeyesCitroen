#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "Esp.h"
#include "WiFiUdp.h"

class Log final
{
public:
    Log(IPAddress *local, HardwareSerial *serial)
    {
        _serial = serial;

        _startTime = millis();

        if (local)
        {
            u32_t broadcast = local->v4() | 0xFF000000;
            _local = *local;
            _remote = IPAddress(broadcast);
            _udp = new WiFiUDP();
        }
        else
        {
            _udp = nullptr;
        }

        _count = 0;
    }

    ~Log() {}

    template <typename... Args>
    void WriteDebug(const char *format, Args... args)
    {
        AppendDebug(format, args...);
        FlushDebug();
    }

    template <typename... Args>
    void AppendDebug(const char *format, Args... args)
    {
        if (_count == 0)
            WriteTime();

        int size = snprintf(_buffer + _count, sizeof(_buffer) - _count, format, args...);
        if (size < 0)
            return;
        _count += size;
    }

    void FlushDebug()
    {
        WriteImpl();
        _count = 0;
    }

    static Log * const GetInstance()
    {
        if (_instance)
            return _instance;
        return _empty;
    }

    static void Init(IPAddress *local, HardwareSerial *serial)
    {
        _instance = new Log(local, serial);
    }

private:
    void WriteImpl()
    {
        if (_count == 0)
            return;

        if (_udp)
        {
            if (_udp->beginPacketMulticast(_remote, _debugPort, _local, 2))
            {
                _udp->write(_buffer, _count);
                _udp->endPacket();
            }
            else
            {
                if (_serial)
                    _serial->println("udp error");
            }
        }

        if (_serial)
        {
            _buffer[_count++] = '\r';
            _buffer[_count++] = '\n';
            _serial->write(_buffer, _count);
        }
    }
    size_t WriteTime()
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
    char _buffer[512];
    WiFiUDP *_udp;
    HardwareSerial *_serial;
    IPAddress _local;
    IPAddress _remote;
    unsigned long int _startTime;
    static const u16_t _debugPort = 6666;
    size_t _count;

    static Log * const _empty;
    static Log *_instance;
};

inline Log* const Log::_empty = new Log(nullptr, nullptr);
inline Log* Log::_instance = nullptr;