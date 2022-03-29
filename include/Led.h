#pragma once

#include <Arduino.h>
#include "Constants.h"
#include "Updatable.h"

class Led : public Updatable
{
public:
    Led(uint8_t pin)
    {
        _pin = pin;
        _state = false;
        _turnOnTime = 0;
        _turnOnPeriod = SIZE_MAX;
    }

    void Init()
    {
        Init(false);
    }

    void Init(bool state)
    {
        pinMode(_pin, OUTPUT);
	    Write(state);
    }

    void Write(bool state)
    {
        _state = state;
        digitalWrite(_pin, state ? HIGH : LOW);
    }

    void Pulse(size_t period)
    {
        _turnOnPeriod = period;
        _turnOnTime = GetTime();
        Write(true);
    }

    void Toggle()
    {
        Write(!_state);
    }

    void Update() override
    {
        if(GetTime() - _turnOnTime > _turnOnPeriod)
        {
            _turnOnPeriod = SIZE_MAX;
            Write(false);
        }
    }

private:
    bool _state;
    uint8_t _pin;
    size_t _turnOnTime;
    size_t _turnOnPeriod;
};