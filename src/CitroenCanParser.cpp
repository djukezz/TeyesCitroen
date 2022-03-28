#include "CitroenCanParser.h"

#include "CanMessageBase.h"
#include "TinyCanMessage.h"
#include "UnknownCanMessage.h"
#include "ButtonCanMessage.h"
#include "ConditionerCanMessage.h"

CitroenCanParser::CitroenCanParser(size_t size, std::function<void(CanMessageBase *)> callback)
{
	_size = size;
	_buffer = new u8[_size];
	_callback = callback;
	_count = 0;
}

CitroenCanParser::~CitroenCanParser()
{
	delete[](_buffer);
}

void CitroenCanParser::Add(u8 b)
{
	// add byte to buffer
	_buffer[_count++] = b;
	while (true)
	{
		int answer = ParseImpl();
		if (answer == 0)
			break; // buffer is empty or message is incomplete

		int msgSize = std::abs(answer);
		bool isCanMessage = answer > 0;

		if (isCanMessage)
			ProcessCanMessage(msgSize); // parse message to typed
		else
			ProcessTrashMessage(msgSize); // parse failed, bypass trash to output as is

		// remove processed data from buffer
		_count -= msgSize;
		if (_count > 0)
			std::memmove(_buffer, _buffer + msgSize, _count);
	}
}

// if result is greater than 0 it's a valid packet. The absolute value of the result is a number of bytes processed
int CitroenCanParser::ParseImpl()
{
	if (_count == 0)
		return 0; // buffer is empty

	if (_buffer[0] != Constants::Prefix)
		return -1; // wrong first byte, shift by 1

	const size_t minSize = Constants::TypeOffset + 1;
	if (_count < minSize)
		return 0; // message is incomplete

	u8 length = _buffer[Constants::LengthOffset];
	if (length > Constants::MaxMessageSize)		
		return -1; // message is too big, it looks like trash 

	if (_count < length + (size_t)1)
		return 0; // message is incomplete

	u8 packetCheckSum = _buffer[length];
	u8 referenceCheckSum = CanMessageBase::CalcCheckSum(_buffer, length);

	if (packetCheckSum != referenceCheckSum)
		return -1; // wrong checksum, so it's not a valid message

	return length + 1; // shift by whole message size (length + 1 prefix byte)
}

void CitroenCanParser::ShiftLeft(size_t size)
{
	std::memmove(_buffer, _buffer + size, size);
	_count -= size;
}

void CitroenCanParser::Fire(CanMessageBase *msg)
{
	_callback(msg);
}

void CitroenCanParser::FireTiny(u8 b)
{
	TinyCanMessage msg(b);
	Fire(&msg);
}

void CitroenCanParser::FireButton(Buttons b)
{
	ButtonCanMessage msg(b);
	Fire(&msg);
}

void CitroenCanParser::FireUnknown(size_t size)
{
	UnknownCanMessage msg(_buffer, size);
	Fire(&msg);
}

void CitroenCanParser::FireCondinioner()
{
	CondinionerCanMessage msg(_buffer);
	Fire(&msg);
}

void CitroenCanParser::ProcessCanMessage(int size)
{
	MessageTypes type = static_cast<MessageTypes>(_buffer[Constants::TypeOffset]);
	switch (type)
	{
	case MessageTypes::STEERING_WHEEL_KEY:
	{
		Buttons b = static_cast<Buttons>(_buffer[Constants::DataOffset]);
		FireButton(b);
	}
	break;
	case MessageTypes::AIR_CONDITIONING_INFO:
	{
		if (size == CondinionerCanMessage::TotalSize)
			FireCondinioner();
	}
	break;
	default:
	{
		FireUnknown(size);
	}
	break;
	}
}

void CitroenCanParser::ProcessTrashMessage(int size)
{
	if (size == 1)
		FireTiny(_buffer[0]);
	else
		FireUnknown(size);
}
