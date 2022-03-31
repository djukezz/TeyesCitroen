#pragma once

#include <c_types.h>
#include "IPAddress.h"

struct Constants final
{
	static const unsigned long BaudRate = 19200;

	static const u8 Prefix = 0xFD;
	static const size_t LengthOffset = 1;
	static const size_t TypeOffset = 2;
	static const size_t DataOffset = 3;

	static const size_t MaxMessageSize = 32;

	static constexpr const char* SSID = "TeyesCitroen";
	static constexpr const char* Password = "TeyesCitroen";
	static IPAddress LocalIp;
};

inline IPAddress Constants::LocalIp = IPAddress{192, 168, 43, 1};
