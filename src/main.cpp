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
#include "ButtonsProcessing.h"
#include "UdpCommandReceiver.h"

#define _inputSerial Serial	  // GPIO3 (RX)
#define _outputSerial Serial1 // GPIO2 (TX)

void MessageCallback(CanMessageBase *);
void SendMessageImpl(CanMessageBase *);

CitroenCanParser _parser(Constants::MaxMessageSize * 2, MessageCallback);
ButtonsProcessing _buttonsProcessing(SendMessageImpl);
u8 _sendBuffer[Constants::MaxMessageSize];
Led _redLed(15);
Led _blueLed(13);
Led _greenLed(12);
StatusInfo _status(&_blueLed);
ModeSelector _modeSelector(4);
FOTA _fota(&_inputSerial);
BootModes _bootMode;
UdpCommandReceiver* _commandReceiver;

void setup()
{
	_outputSerial.begin(Constants::BaudRate);
	_inputSerial.begin(Constants::BaudRate);

	NVRamData nvRam{};
	_bootMode = nvRam.BootMode;

	_inputSerial.println();
	_inputSerial.print("Boot mode: ");
	_inputSerial.println(BootModeEx::ToString(_bootMode));

	if(_bootMode == BootModes::FOTA)
	{
		nvRam.Reset();
		nvRam.Save();
		
		_fota.Setup();
		return;
	}

	bool isDebugMode = _bootMode == BootModes::DEBUG;
	_redLed.Init();
	_blueLed.Init();
	_greenLed.Init();

	WiFi.persistent(false);

	if(isDebugMode)
	{
		_greenLed.Pulse(10000);

		Log::Init(&Constants::LocalIp, &_inputSerial);

		WiFi.mode(WIFI_AP);
		WiFi.softAPConfig(Constants::LocalIp, IPAddress(127, 0, 0, 1), IPAddress(255, 255, 255, 0));
		WiFi.softAP(Constants::SSID, Constants::Password);	

		_commandReceiver = new UdpCommandReceiver(MessageCallback);	
	}
	else
	{
		WiFi.setSleepMode(WIFI_MODEM_SLEEP); delay(1);
  		WiFi.forceSleepBegin(); delay(1);
	}

	_buttonsProcessing.Init();

	Log::GetInstance()->WriteDebug("Setup completed");
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
}

void MessageCallback(CanMessageBase *msg)
{
	if (ButtonCanMessage *button = dynamic_cast<ButtonCanMessage *>(msg))
	{
		_modeSelector.Process(button->Button);
		_buttonsProcessing.Process(button);
		return; // buttons have their own special behavior 
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

	SendMessageImpl(msg);
}

void SendMessageImpl(CanMessageBase *msg)
{
	size_t size = msg->CopyTo(_sendBuffer);
	_outputSerial.write(_sendBuffer, size);

	_redLed.Pulse(100);
}