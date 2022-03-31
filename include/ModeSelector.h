#pragma once

#include "Esp.h"
#include "Buttons.h"
#include <Arduino.h>
#include "Log.h"
#include "Updatable.h"

// You have to press <DebugButton> button <Repeats> times with max delay <Time>ms between presses
class ModeSelector final : public Updatable
{
public:
    ModeSelector(uint8_t pin)
        : _pin(pin)
    {
        pinMode(_pin, INPUT);
    }

    bool IsDebugMode()
    {
        return digitalRead(_pin) == LOW ||
               ESP.getResetInfoPtr()->reason == REASON_SOFT_RESTART;
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

        if (_repeats >= Repeats)
            Reset();
        else if (_repeats * 2 >= Repeats)
            Log::GetInstance()->WriteDebug("%d times left");
    }

    void Update() override
    {
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
        Log::GetInstance()->WriteDebug("Resetting...");
        Log::GetInstance()->WriteDebug("Boot in debug mode...");
        delay(500);
        ESP.reset();
    }

    const uint8_t _pin;
    static const Buttons DebugButton = Buttons::SWC_KEY_ESC;
    static const size_t Repeats = 10;
    static const size_t Time = 1000;
    size_t _repeats = 0;
    size_t _pressTime = 0;
};