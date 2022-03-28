#pragma once

#include <cstring>

#include "MessageTypes.h"
#include "CanMessageBase.h"

class UnknownCanMessage : public CanMessageBase
{
public:
	UnknownCanMessage(u8* data, size_t size)
	{
		_size = size;
		std::memcpy(&_data, data, _size);
	}

	size_t CopyTo(u8* dst) override
	{
		std::memcpy(dst, &_data, _size);
		return _size;
	}

	MessageTypes GetMessageType()
	{
		if(_size <= Constants::TypeOffset)
			return MessageTypes::UNKNOWN;
		return static_cast<MessageTypes>(_data[Constants::TypeOffset]);
	}

private:
	u8 _data[Constants::MaxMessageSize];
	size_t _size;
};

