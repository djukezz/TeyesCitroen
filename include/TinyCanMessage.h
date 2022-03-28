#pragma once

#include "CanMessageBase.h"

class TinyCanMessage : public CanMessageBase
{
public:
	TinyCanMessage(u8 data)
	{
		_data = data;
	}

	size_t CopyTo(u8* dst) override
	{
		*dst = _data;
		return 1;
	}	

private:
	u8 _data;
};

