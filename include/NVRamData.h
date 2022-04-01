#pragma once

#include "BootModes.h"
#include "Arduino.h"

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
        u8_t offset = BaseOffset;
        u32_t startMark = 0;
        u32_t endMark = 0;
        BootModes bootMode = BootModes::NORMAL;
        system_rtc_mem_read(offset, &startMark, sizeof(u32_t));
        offset += (sizeof(u32_t) + 3) / 4;
        system_rtc_mem_read(offset, &bootMode, sizeof(BootMode));
        offset += (sizeof(BootMode) + 3) / 4;        
        system_rtc_mem_read(offset, &endMark, sizeof(u32_t));
        offset += (sizeof(u32_t) + 3) / 4;

        if(startMark == Mark && endMark == Mark)
        {
            BootMode = bootMode;
        }
        else
        {
            Reset();
        }

        Save();
    }
    void Save()
    {
        uint8_t offset = BaseOffset;
        u32_t mark = Mark;
        BootModes bootMode = BootMode;
        system_rtc_mem_write(offset, &mark, sizeof(u32_t));
        offset += (sizeof(u32_t) + 3) / 4;
        system_rtc_mem_write(offset, &bootMode, sizeof(BootMode));
        offset += (sizeof(BootMode) + 3) / 4;        
        system_rtc_mem_write(offset, &mark, sizeof(u32_t));
        offset += (sizeof(u32_t) + 3) / 4;
    }
    void Reset()
    {
        BootMode = BootModes::NORMAL;
    }

    BootModes BootMode;

private:
    static const u32_t Mark = 0xA0B0C0D0;
    static const u8_t BaseOffset = 256 / 4;
};