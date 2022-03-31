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
#include "StatusInfo.h"
#include "UpdatableCollection.h"
#include "ModeSelector.h"
#include "FOTA.h"
#include "NVRamData.h"

#define _inputSerial Serial	  // GPIO3 (RX)
#define _outputSerial Serial1 // GPIO2 (TX)

unsigned long _lastInfoTime = 0;
const unsigned long _infoPeriod = 5000;

void MessageCallback(CanMessageBase *);
Buttons RemapButton(Buttons btn);

CitroenCanParser _parser(Constants::MaxMessageSize * 2, MessageCallback);
u8 _sendBuffer[Constants::MaxMessageSize];
Led _redLed(15);
Led _blueLed(13);
Led _greenLed(12);
WiFiUDP *_udp = nullptr;
StatusInfo _status(&_blueLed);
ModeSelector _modeSelector(4);
FOTA _fota(&_inputSerial);
BootModes _bootMode;

void setup()
{
	NVRamData nvRam;
	_bootMode = nvRam.BootMode;
	if(_bootMode == BootModes::FOTA)
	{
		nvRam.Reset();
		nvRam.Save();
		
		_fota.Setup();
		return;
	}

	bool isDebugMode = _bootMode == BootModes::DEBUG;
	_outputSerial.begin(Constants::BaudRate);
	_inputSerial.begin(Constants::BaudRate);
	_redLed.Init();
	_blueLed.Init();
	_greenLed.Init();

	WiFi.persistent(false);

	if(isDebugMode)
	{
		_greenLed.Pulse(1000);

		Log::Init(&Constants::LocalIp, &_inputSerial);

		WiFi.mode(WIFI_AP);
		WiFi.softAPConfig(Constants::LocalIp, IPAddress(127, 0, 0, 1), IPAddress(255, 255, 255, 0));
		WiFi.softAP(Constants::SSID, Constants::Password);
		_inputSerial.println();
		_inputSerial.println("--- DEBUG MODE ---");

		_udp = new WiFiUDP();
		_udp->begin(6666);
	}
	else
	{
		WiFi.setSleepMode(WIFI_MODEM_SLEEP); delay(1);
  		WiFi.forceSleepBegin(); delay(1);
	}

	Log::GetInstance()->WriteDebug("Setup completed");
	Log::GetInstance()->WriteDebug("Flash size: %d", ESP.getFlashChipRealSize());
	Log::GetInstance()->WriteDebug("Reset reason: %d", ESP.getResetInfoPtr()->reason);
}

void loop()
{
	if(_bootMode == BootModes::FOTA)
	{
		_fota.Loop();
		return;
	}

	UpdatableCollection::UpdateAll();

	int b = _inputSerial.read();
	if (b >= 0)
		_parser.Add(static_cast<u8>(b));

	if (_udp)
	{
		int packetSize = _udp->parsePacket();
		if (packetSize == 2)
		{
			char buff[packetSize + 1];
			_udp->read(buff, packetSize);
			buff[packetSize] = 0;
			char *end;
			Buttons b = static_cast<Buttons>(strtol(buff, &end, 16));

			Log::GetInstance()->WriteDebug("REMOTE %s", ButtonEx::ToString(b));
			ButtonCanMessage msg(b);
			MessageCallback(&msg);
		}
	}
}

void MessageCallback(CanMessageBase *msg)
{
	if (ButtonCanMessage *button = dynamic_cast<ButtonCanMessage *>(msg))
	{
		Buttons b1 = button->Button;
		Buttons b2 = RemapButton(b1);

		_modeSelector.ButtonPressed(b1);

		if (b1 == b2)
			Log::GetInstance()->WriteDebug("Button %s", ButtonEx::ToString(b1));
		else
		{
			auto remapped = ButtonCanMessage(b2);
			msg = &remapped;
			Log::GetInstance()->WriteDebug("Button %s -> %s", ButtonEx::ToString(b1), ButtonEx::ToString(b2));
		}
	}
	else if (auto tiny = dynamic_cast<TinyCanMessage *>(msg))
	{
		u8 v = 0;
		tiny->CopyTo(&v);
		Log::GetInstance()->WriteDebug("Bypasses %02X", v);
	}
	else if (auto conditioner = dynamic_cast<CondinionerCanMessage *>(msg))
	{
		float t1 = conditioner->GetT1();
		float t2 = conditioner->GetT2();
		Log::GetInstance()->WriteDebug("Conditioner t1=%.1f, t2=%.1f", t1, t2);
		conditioner->FixTemperatures();
	}
	else if (auto unknown = dynamic_cast<UnknownCanMessage *>(msg))
	{
		auto messageType = MessageTypeEx::ToString(unknown->GetMessageType());
		auto unknownSize = msg->CopyTo(_sendBuffer);
		Log::GetInstance()->AppendDebug("%s", messageType);
		for (size_t i = 0; i < unknownSize; i++)
			Log::GetInstance()->AppendDebug(" %02X", _sendBuffer[i]);
		Log::GetInstance()->FlushDebug();
	}

	size_t size = msg->CopyTo(_sendBuffer);
	_outputSerial.write(_sendBuffer, size);

	_redLed.Pulse(100);
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