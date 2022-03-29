#pragma once

#include "Updatable.h"

class Periodic : public Updatable
{
public:
    Periodic(size_t period)
    {
        _period = period;
        _lastUpdate = 0;
    }
    virtual ~Periodic() {}
    void Update() override
    {
        size_t time = GetTime();
        if(time - _lastUpdate >= _period)
        {
            _lastUpdate = time;
            Process();
        }
    }
protected:
    virtual void Process() = 0;
private:
    size_t _period;
    size_t _lastUpdate;
};