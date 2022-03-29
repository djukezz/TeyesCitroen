#pragma once

#include "Periodic.h"
#include "Led.h"
#include "Log.h"
#include "Esp.h"

class StatusInfo final : public Periodic
{
public:
    StatusInfo(Led* const led, Log** const log, size_t period = 10000)
    :Periodic(period), _led(led), _logPtr(log)
    {
    }
    ~StatusInfo() {}
    void Init() override
    {
        if(_logPtr)
            _log = *_logPtr;
    }
protected:    
    void Process() override
    {
        if(_led)
            _led->Pulse(100);
        if(_log)
            _log->WriteDebug("heap: %i", ESP.getFreeHeap());
    };
private:
    Led* const _led;
    Log* _log = nullptr;
    Log** const _logPtr;
};