#pragma once

#include "Constants.h"

enum class MessageTypes : u8
{
	UNKNOWN = 0,
	BACKLIGHT_INFO = 0x01,
	STEERING_WHEEL_KEY = 0x02,
	AIR_CONDITIONING_INFO = 0x21,
	STEERING_WHEEL_ANGLE = 0x29,
	FULL_RADAR_INFO = 0x30,
	REVERSE_RADAR_INFO = 0x32,
	COMPUTE_INFO_PAGE0 = 0x33,
	COMPUTE_INFO_PAGE1 = 0x34,
	COMPUTE_INFO_PAGE2 = 0x35,
	EXTERN_TEMP = 0x36,
	ALARM_RECORD_INFO = 0x37,
	VEHICLE_STATUS = 0x38,
	VEHICLE_FUNC_INFO = 0x39,
	VEHICLE_DIAGNOSTIC_INFO = 0x3A,
	RECORD_SPEED_VALUE = 0x3B,
	SPEED_INFO = 0x3D,
	SPEED_ALARM_DIALOG = 0x3F,
	PROTOCOL_VERSION_INFO = 0x7F,
};

class MessageTypeEx final
{
public:
	static const char *ToString(MessageTypes t)
	{
		switch (t)
		{
		case MessageTypes::BACKLIGHT_INFO:
			return "BACKLIGHT_INFO";
		case MessageTypes::STEERING_WHEEL_KEY:
			return "STEERING_WHEEL_KEY";
		case MessageTypes::AIR_CONDITIONING_INFO:
			return "AIR_CONDITIONING_INFO";
		case MessageTypes::STEERING_WHEEL_ANGLE:
			return "STEERING_WHEEL_ANGLE";
		case MessageTypes::FULL_RADAR_INFO:
			return "FULL_RADAR_INFO";
		case MessageTypes::REVERSE_RADAR_INFO:
			return "REVERSE_RADAR_INFO";
		case MessageTypes::COMPUTE_INFO_PAGE0:
			return "COMPUTE_INFO_PAGE0";
		case MessageTypes::COMPUTE_INFO_PAGE1:
			return "COMPUTE_INFO_PAGE1";
		case MessageTypes::COMPUTE_INFO_PAGE2:
			return "COMPUTE_INFO_PAGE2";
		case MessageTypes::EXTERN_TEMP:
			return "EXTERN_TEMP";
		case MessageTypes::ALARM_RECORD_INFO:
			return "ALARM_RECORD_INFO";
		case MessageTypes::VEHICLE_STATUS:
			return "VEHICLE_STATUS";
		case MessageTypes::VEHICLE_FUNC_INFO:
			return "VEHICLE_FUNC_INFO";
		case MessageTypes::VEHICLE_DIAGNOSTIC_INFO:
			return "VEHICLE_DIAGNOSTIC_INFO";
		case MessageTypes::RECORD_SPEED_VALUE:
			return "RECORD_SPEED_VALUE";
		case MessageTypes::SPEED_INFO:
			return "SPEED_INFO";
		case MessageTypes::SPEED_ALARM_DIALOG:
			return "SPEED_ALARM_DIALOG";
		case MessageTypes::PROTOCOL_VERSION_INFO:
			return "PROTOCOL_VERSION_INFO";
		default:
			return "[UNKNOWN]";
		}
	}
};
