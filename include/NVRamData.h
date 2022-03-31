#pragma once

#include "BootModes.h"

#include "user_interface.h"

class NVRamData final
{
public:
    NVRamData()
    {
        Reset();
        Load();
    }
    void Load()
    {
        uint8_t offset = BaseOffset;
        system_rtc_mem_read(offset, &BootMode, sizeof(BootMode));
        offset += (sizeof(BootMode) + 3) / 4;
    }
    void Save()
    {
        uint8_t offset = BaseOffset;
        system_rtc_mem_write(offset, &BootMode, sizeof(BootMode));
        offset += (sizeof(BootMode) + 3) / 4;
    }
    void Reset()
    {
        BootMode = BootModes::NORMAL;
    }

    BootModes BootMode;

private:
    static const uint8_t BaseOffset = 64;
};