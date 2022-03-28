#pragma once

#include "Constants.h"

class CanMessageBase
{
public:
	CanMessageBase(){}
	virtual ~CanMessageBase() {}
	virtual size_t CopyTo(u8* dst) = 0;
	static u8 CalcCheckSum(u8* data, size_t size)
	{
		u8 sum = 0;
		for (size_t i = 1; i < size; i++)
		{
			sum += data[i];
		}

		return sum;
	}
};

