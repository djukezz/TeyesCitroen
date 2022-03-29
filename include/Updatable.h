#pragma once

#include "Arduino.h"

class Updatable
{
public:
    Updatable() {}
    virtual ~Updatable() {}
    virtual void Update() {}
    virtual void Init() {}
protected:
    size_t GetTime()
    {
        return static_cast<size_t>(millis());
    }
};