#pragma once

#include "Esp.h"
#include "Buttons.h"
#include <Arduino.h>
#include "Log.h"
#include "Updatable.h"
#include "BootModes.h"
#include "NVRamData.h"

// You have to press <DebugButton> button <Repeats> times with max delay <Time>ms between presses
class ModeSelector final : public Updatable
{
public:
    ModeSelector(uint8_t pin)
        : _pin(pin)
    {
        pinMode(_pin, INPUT);
    }

    void ButtonPressed(Buttons b)
    {
        switch (b)
        {
        case Buttons::SWC_KEY_NONE:
            return;
        case DebugButton:
            _pressTime = GetTime();
            _repeats++;
            break;
        default:
            _pressTime = 0;
            _repeats = 0;
            break;
        }

        if (_repeats >= DebugRepeats)
            Reset();
        else if (_repeats * 2 >= DebugRepeats)
            Log::GetInstance()->WriteDebug("%d times left", DebugRepeats - _repeats);
    }

    void Update() override
    {
        if(digitalRead(_pin) == LOW)
            Reset();

        if (_pressTime == 0)
            return;

        if (GetTime() - _pressTime > Time)
        {
            _pressTime = 0;
            _repeats = 0;
        }
    }

private:
    void Reset()
    {
        NVRamData nvRam;
        Log::GetInstance()->WriteDebug("Current boot mode is %s", BootModeEx::ToString(nvRam.BootMode));
        nvRam.BootMode = GetNextMode(nvRam.BootMode);
        nvRam.Save();

        Log::GetInstance()->WriteDebug("Resetting...");
        Log::GetInstance()->WriteDebug("Boot in %s mode...", BootModeEx::ToString(nvRam.BootMode));
        delay(500);
        ESP.reset();
    }

    static BootModes GetNextMode(BootModes current)
    {
        switch (current)
        {
        case BootModes::NORMAL:
            return BootModes::DEBUG;
        case BootModes::DEBUG:
            return BootModes::FOTA;
        default:
            return BootModes::NORMAL;
        }
    }

    const uint8_t _pin;
    static const Buttons DebugButton = Buttons::SWC_KEY_ESC;
    static const size_t DebugRepeats = 10;
    static const size_t Time = 1000;
    Buttons _button;
    size_t _repeats = 0;
    size_t _pressTime = 0;
};