#pragma once

#include "Constants.h"

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

class FOTA
{
public:
    FOTA(HardwareSerial *const serial)
        : _serial(serial)
    {
    }

    void Setup()
    {
        WiFi.persistent(false);

        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(Constants::LocalIp, IPAddress(127, 0, 0, 1), IPAddress(255, 255, 255, 0));
        WiFi.softAP(Constants::SSID, Constants::Password);

        if (_serial)
        {
            _serial->begin(Constants::BaudRate);
            _serial->println("");
        }

        if (_serial)
        {
            _serial->println("");
            _serial->print("AP SSID: ");
            _serial->println(Constants::SSID);
            _serial->print("IP address: ");
            _serial->println(Constants::LocalIp);
        }

        _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                   { request->send(200, "text/plain", "Hi! I am ESP8266."); });

        AsyncElegantOTA.begin(&_server); // Start ElegantOTA
        _server.begin();

        if (_serial)
            _serial->println("HTTP server started");
    }

    void Loop() {}

private:
    AsyncWebServer _server{80};
    HardwareSerial *const _serial;
};