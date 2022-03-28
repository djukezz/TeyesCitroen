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
    void WriteDebug(const char *format, ...);
    void WriteDebug(const char *prefix, const u8_t* data, size_t dataSize);

private:
    void WriteImpl(size_t size);
    size_t WriteTime(char* buffer);
    char _buffer[256];
    WiFiUDP _udp;
    HardwareSerial* _serial;
    IPAddress _local;
    IPAddress _remote;
    unsigned long int _startTime;
    static const u16_t _debugPort = 6666;
    bool _disabled;
};