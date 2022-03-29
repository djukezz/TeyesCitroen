#pragma once

#include <stdio.h>
#include <stdarg.h>
#include "Esp.h"
#include "WiFiUdp.h"

class Log final
{
public:
    Log(IPAddress local, HardwareSerial* serial);
    Log();
    ~Log();
    template <typename ...Args>
    void WriteDebug(const char *format, Args ...args);
    template <typename ...Args>
    void AppendDebug(const char *format, Args ...args);
    void AppendDebug();
    void FlushDebug();

private:
    void WriteImpl();
    size_t WriteTime();
    char _buffer[512];
    WiFiUDP _udp;
    HardwareSerial* _serial;
    IPAddress _local;
    IPAddress _remote;
    unsigned long int _startTime;
    static const u16_t _debugPort = 6666;
    bool _disabled;
    size_t _count;
};

template<typename ...Args>
inline void Log::AppendDebug(const char* format, Args ...args)
{
    if(_disabled)
        return;

    if(_count == 0)
        WriteTime();

    int size = snprintf(_buffer + _count, sizeof(_buffer) - _count, format, args...);
    if(size < 0)
        return;
    _count += size;
}

template<typename ...Args>
inline void Log::WriteDebug(const char* format, Args ...args)
{
    AppendDebug(format, args...);
    FlushDebug();
}