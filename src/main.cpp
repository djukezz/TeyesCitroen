#include <Arduino.h>

#include "ESP8266WiFi.h"

#include "ButtonCanMessage.h"
#include "CanMessageBase.h"
#include "CitroenCanParser.h"
#include "TinyCanMessage.h"
#include "UnknownCanMessage.h"
#include "ConditionerCanMessage.h"
#include "Log.h"
#include "Led.h"

//#define DEBUG_MSG

#define _inputSerial Serial	  // GPIO3 (RX)
#define _outputSerial Serial1 // GPIO2 (TX)

unsigned long _lastInfoTime = 0;
const unsigned long _infoPeriod = 5000;

void MessageCallback(CanMessageBase *);
Buttons RemapButton(Buttons btn);

CitroenCanParser _parser(Constants::MaxMessageSize * 2, MessageCallback);
u8 _sendBuffer[Constants::MaxMessageSize];
Led _redLed(15);
Led _builtInLed(13);
IPAddress _localIp(192, 168, 43, 1);
#ifdef DEBUG_MSG
Log _log(_localIp, nullptr);
#else
Log _log;
#endif
WiFiUDP _udp;

void setup()
{
	_outputSerial.begin(Constants::BaudRate);
	_inputSerial.begin(Constants::BaudRate);
	_redLed.Init();
	_builtInLed.Init();

	WiFi.persistent(false);

#ifdef DEBUG_MSG
	WiFi.mode(WIFI_AP);
	WiFi.softAPConfig(_localIp, IPAddress(127, 0, 0, 1), IPAddress(255, 255, 255, 0));
	WiFi.softAP("KeyRemap", "4952217929");
	_inputSerial.println();
	_udp.begin(6666);
#else
	WiFi.mode(WIFI_OFF);
	WiFi.forceSleepBegin();
#endif

	_log.WriteDebug("----------\r\nSetup completed");
}

void loop()
{
	int b = _inputSerial.read();
	if (b >= 0)
		_parser.Add(static_cast<u8>(b));

	auto currentTime = millis();
	if (currentTime > _lastInfoTime + _infoPeriod)
	{
		auto heap = ESP.getFreeHeap();
		_log.WriteDebug("heap: %d", heap);

		_lastInfoTime = currentTime;

		_builtInLed.Toggle();
	}

	int packetSize = _udp.parsePacket();
	if (packetSize == 2)
	{
		char buff[packetSize + 1];
		_udp.read(buff, packetSize);
		buff[packetSize] = 0;
		char *end;
		Buttons b = static_cast<Buttons>(strtol(buff, &end, 16));
		_log.WriteDebug("REMOTE %s", ButtonEx::ToString(b));
		ButtonCanMessage msg(b);
		MessageCallback(&msg);
	}
}

void MessageCallback(CanMessageBase *msg)
{
	if (ButtonCanMessage *button = dynamic_cast<ButtonCanMessage *>(msg))
	{
		Buttons b1 = button->Button;
		Buttons b2 = RemapButton(b1);

		if (b1 == b2)
			_log.WriteDebug("Button %s", ButtonEx::ToString(b1));
		else
		{
			auto remapped = ButtonCanMessage(b2);
			msg = &remapped;
			_log.WriteDebug("Button %s -> %s", ButtonEx::ToString(b1), ButtonEx::ToString(b2));
		}
	}
	else if (auto tiny = dynamic_cast<TinyCanMessage *>(msg))
	{
		u8 v = 0;
		tiny->CopyTo(&v);
		_log.WriteDebug("Bypasses '%02X'", v);
	}
	else if (auto conditioner = dynamic_cast<CondinionerCanMessage *>(msg))
	{
		float t1 = conditioner->GetT1();
		float t2 = conditioner->GetT2();
		_log.WriteDebug("T1=%.1f, T2=%.1f", t1, t2);
		conditioner->FixTemperatures();
	}	
	else if (auto unknown = dynamic_cast<UnknownCanMessage *>(msg))
	{
		auto messageType = MessageTypeEx::ToString(unknown->GetMessageType());
		auto unknownSize = msg->CopyTo(_sendBuffer);
		_log.WriteDebug(messageType, _sendBuffer, unknownSize);
	}

	size_t size = msg->CopyTo(_sendBuffer);
	_outputSerial.write(_sendBuffer, size);

	_redLed.Toggle();
}

Buttons RemapButton(Buttons btn)
{
	switch (btn)
	{
	case Buttons::SWC_KEY_LIST:
		return Buttons::SWC_KEY_MENU;
	case Buttons::SWC_KEY_MENU_DOWN:
		return Buttons::SWC_KEY_MEDIA;
	case Buttons::SWC_KEY_MENU_UP:
		return Buttons::SWC_KEY_NAVI;
	default:
		return btn;
	}
}