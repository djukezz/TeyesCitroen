#pragma once

#include "Periodic.h"
#include "Led.h"
#include "Log.h"
#include "Esp.h"

class StatusInfo final : public Periodic
{
public:
    StatusInfo(Led* const led, size_t period = 10000)
    :Periodic(period), _led(led)
    {
    }
    ~StatusInfo() {}
protected:    
    void Process() override
    {
        if(_led)
            _led->Pulse(100);
        Log::GetInstance()->WriteDebug("heap: %i", ESP.getFreeHeap());
    };
private:
    Led* const _led;
};