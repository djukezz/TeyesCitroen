#pragma once

#include "Updatable.h"
#include "Buttons.h"
#include "ButtonCanMessage.h"
#include "Log.h"

struct ButtonItem
{
    ButtonItem(Buttons orig, Buttons shortPress, Buttons longPress)
        : Orig(orig), Short(shortPress), Long(longPress)
    {
    }
    ButtonItem(Buttons orig, Buttons anyPress)
        : ButtonItem(orig, anyPress, anyPress)
    {
    }
    ButtonItem()
        : ButtonItem(Buttons::SWC_KEY_NONE, Buttons::SWC_KEY_NONE)
    {
    }

    Buttons Orig;
    Buttons Short;
    Buttons Long;

    bool IsLongPresent()
    {
        return Short != Long;
    }

    bool IsEmpty()
    {
        return Orig == Buttons::SWC_KEY_NONE;
    }
};

class ButtonsProcessing : public Updatable
{
public:
    ButtonsProcessing(std::function<void(CanMessageBase *)> callback)
    {
        _callback = callback;
    }

    void Init()
    {        
        LogButtonsInfo();
    }

    void Process(ButtonCanMessage *const msg)
    {
        Buttons orig = msg->Button;
        if (orig == Buttons::SWC_KEY_NONE)
        {
            TrySendPrevious(false);
            Send(Buttons::SWC_KEY_NONE);
            return;
        }

        for (auto &item : _items)
        {
            if (item.Orig == orig)
            {
                Process(item);
                return;
            }
        }

        Log::GetInstance()->WriteDebug("Button %s", ButtonEx::ToString(orig));
        CanMessageBase *const ptr = msg;
        _callback(ptr);
    }
    void Update() override
    {
        if (_time == 0)
            return;

        if (GetTime() - _time < _longPressTime)
            return;

        TrySendPrevious(true);
    }

private:
    void Process(ButtonItem &item)
    {
        if (!item.IsLongPresent())
        {
            Log::GetInstance()->WriteDebug("Button %s ---> %s", ButtonEx::ToString(item.Orig), ButtonEx::ToString(item.Short));
            Send(item.Short);
            return;
        }

        _pressed = item;
        _time = GetTime();

        Log::GetInstance()->WriteDebug("Register button %s -> %s -----> %s",
                                       ButtonEx::ToString(_pressed.Orig), ButtonEx::ToString(_pressed.Short), ButtonEx::ToString(_pressed.Long));
    }

    void TrySendPrevious(bool longPress)
    {
        if (!_pressed.IsEmpty())
        {
            if (longPress)
                Log::GetInstance()->WriteDebug("Button %s -----> %s", ButtonEx::ToString(_pressed.Orig), ButtonEx::ToString(_pressed.Long));
            else
                Log::GetInstance()->WriteDebug("Button %s -> %s", ButtonEx::ToString(_pressed.Orig), ButtonEx::ToString(_pressed.Short));
            Send(longPress ? _pressed.Long : _pressed.Short);
            _pressed = ButtonItem();
        }
        _time = 0;
    }

    void Send(Buttons b)
    {
        ButtonCanMessage msg(b);
        CanMessageBase *ptr = &msg;
        _callback(ptr);
    }

    void LogButtonsInfo()
    {
        auto log = Log::GetInstance();
        log->WriteDebug("Buttons settings:");
        for (auto &item : _items)
        {
            if (item.IsLongPresent())
                log->WriteDebug("%s -> %s -----> %s",
                                ButtonEx::ToString(item.Orig), ButtonEx::ToString(item.Short), ButtonEx::ToString(item.Long));
            else
                log->WriteDebug("%s ---> %s",
                                ButtonEx::ToString(item.Orig), ButtonEx::ToString(item.Short));
        }
    }

    std::function<void(CanMessageBase *)> _callback;
    ButtonItem _pressed;
    size_t _time = 0;
    static const size_t _longPressTime = 1000;
    std::vector<ButtonItem> _items{
        ButtonItem(Buttons::SWC_KEY_ESC, Buttons::SWC_KEY_ESC, Buttons::SWC_KEY_MENU),
        ButtonItem(Buttons::SWC_KEY_PHONE_ACCEPT, Buttons::SWC_KEY_PHONE_ACCEPT, Buttons::SWC_KEY_BT),
        ButtonItem(Buttons::SWC_KEY_LIST, Buttons::SWC_KEY_NAVI),
        ButtonItem(Buttons::SWC_KEY_MENU_UP, Buttons::SWC_KEY_MUSIC),
        ButtonItem(Buttons::SWC_KEY_MENU_DOWN, Buttons::SWC_KEY_RADIO),
    };
};
