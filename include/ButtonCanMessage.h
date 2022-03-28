#pragma once

#include "CanMessageBase.h"
#include "MessageTypes.h"
#include "Buttons.h"

class ButtonCanMessage : public CanMessageBase
{
public:
	ButtonCanMessage(Buttons button)
	{
		Button = button;
	}

	static const MessageTypes MessageType = MessageTypes::STEERING_WHEEL_KEY;
	Buttons Button;
	size_t CopyTo(u8* dst) override
	{
		u8* ptr = dst;
		*ptr++ = Constants::Prefix;
		const size_t size = 1 + 1 + 1 + 1;
		*ptr++ = static_cast<u8>(size);
		*ptr++ = (u8)MessageType;
		*ptr++ = (u8)Button;
		u8 checkSum = CalcCheckSum(dst, size);
		*ptr = checkSum;

		return static_cast<u8>(size) + 1;
	}
};

