#pragma once

#include <functional>

#include "CanMessageBase.h"
#include "Buttons.h"

class CitroenCanParser final
{
public:
	CitroenCanParser(size_t size, std::function<void(CanMessageBase *)> callback);
	~CitroenCanParser();
	void Add(u8 b);

private:
	size_t _size;
	u8 *_buffer;
	size_t _count;
	std::function<void(CanMessageBase *)> _callback;

	void Fire(CanMessageBase *msg);
	void FireTiny(u8 b);
	void FireButton(Buttons b);
	void FireUnknown(size_t size);
	void FireCondinioner();
	int ParseImpl();
	void ShiftLeft(size_t size);
	void ProcessCanMessage(int);
	void ProcessTrashMessage(int);
};
