#pragma once

#include <c_types.h>

enum class BootModes : u32
{
    NORMAL,
    DEBUG,
    FOTA,
};

class BootModeEx final
{
public:
    static const char *ToString(BootModes b)
    {
        switch (b)
        {
        case BootModes::NORMAL:
            return "NORMAL";
        case BootModes::DEBUG:
            return "DEBUG";
        case BootModes::FOTA:
            return "OTA";
        default:
            return "[UNKNOWN]";
        }
    }
};