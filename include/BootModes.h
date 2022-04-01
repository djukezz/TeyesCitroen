#pragma once

#include <c_types.h>

enum class BootModes : u32
{
    NORMAL = 0,
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

    static bool IsValid(BootModes b)
    {
        switch (b)
        {
        case BootModes::NORMAL:
        case BootModes::DEBUG:
        case BootModes::FOTA:
            return true;
        default:
            return false;
        }
    }
};