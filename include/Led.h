#pragma once

#include <Arduino.h>
#include "Constants.h"

class Led final
{
public:
    Led(uint8_t pin)
    {
        _pin = pin;
        _state = false;
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

    void Toggle()
    {
        Write(!_state);
    }

private:
    bool _state;
    uint8_t _pin;
};