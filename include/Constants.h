#pragma once

#include <c_types.h>

struct Constants final
{
	static const unsigned long BaudRate = 19200;

	static const u8 Prefix = 0xFD;
	static const size_t LengthOffset = 1;
	static const size_t TypeOffset = 2;
	static const size_t DataOffset = 3;

	static const size_t MaxMessageSize = 32;
};

