#pragma once

#include "Constants.h"

enum class Buttons : u8
{
	SWC_KEY_NONE = 0x00,			// No key or key release
	SWC_KEY_MENU = 0x02,			// MENU
	SWC_KEY_MENU_UP = 0x03,			// Arrow key-up
	SWC_KEY_MENU_DOWN = 0x04,		// Arrow key-down
	SWC_KEY_OK = 0x07,				// OK
	SWC_KEY_ESC = 0x08,				// ESC
	SWC_KEY_MODE = 0x10,			// Mode
	SWC_KEY_SRC = 0x11,				// Source
	SWC_KEY_SEEK_DOWN = 0x12,		// Seek+
	SWC_KEY_SEEK_UP = 0x13,			// Seek-
	SWC_KEY_VOLUME_UP = 0x14,		// Volume+
	SWC_KEY_VOLUME_DOWN = 0x15,		// Volume-
	SWC_KEY_MUTE = 0x16,			// Mute
	SWC_KEY_MEMO_UP = 0x17,			// Previous song
	SWC_KEY_MEMO_DOWN = 0x18,		// Next song
	SWC_KEY_PAGE_SW = 0x20,			// Switch computer information page
	SWC_KEY_MENU4 = 0x21,			// Menu
	SWC_KEY_MEMO = 0x22,			// Memory speed interface
	SWC_KEY_BT = 0x23,				// Bluetooth button
	SWC_KEY_PUSH_TO_TALK = 0x29,	//
	SWC_KEY_VEHICLE_SETTING = 0x2A, // Vehicle settings
	SWC_KEY_VEHICLE_NAVI = 0x2B,
	SWC_KEY_MUSIC = 0x2C,
	SWC_KEY_BLUETOOTH = 0x2D,
	SWC_KEY_APPS = 0x2E,
	SWC_KEY_AIR_CONDTION_CONTROL = 0x2F,
	SWC_KEY_PHONE_ACCEPT = 0x30, // The call is answered
	SWC_KEY_PHONE_HANGUP = 0x31, // The phone hangs up
	SWC_KEY_NAVI = 0x32,		 // Navi
	SWC_KEY_RADIO = 0x33,		 // Radio
	SWC_KEY_SETUP = 0x34,		 // Setup
	SWC_KEY_ADDR = 0x35,		 // Addr
	SWC_KEY_MEDIA = 0x36,		 // Media
	SWC_KEY_TRAF = 0x37,		 // Traf
	SWC_KEY_UP = 0x38,			 // Up
	SWC_KEY_DOWN = 0x39,		 // Down
	SWC_KEY_LEFT = 0x40,		 // Left
	SWC_KEY_RIGHT = 0x41,		 // Right
	SWC_KEY_SCROLL_UP = 0x42,	 // ScrollUp
	SWC_KEY_SCROLL_DOWN = 0x43,	 // ScrollDown
	SWC_KEY_NUM1 = 0x44,		 // 1
	SWC_KEY_NUM2 = 0x45,		 // 2
	SWC_KEY_NUM3 = 0x46,		 // 3
	SWC_KEY_NUM4 = 0x47,		 // 4
	SWC_KEY_NUM5 = 0x48,		 // 5
	SWC_KEY_NUM6 = 0x49,		 // 6
	SWC_KEY_SRC2 = 0x4A,		 // SRC
	SWC_KEY_BAND = 0x50,		 // Band
	SWC_KEY_LIST = 0x51,		 // List
	SWC_KEY_SOUND = 0x52,		 // Sound
	SWC_KEY_TA = 0x53,			 // TA
	SWC_KEY_DARK = 0x54,		 // Dark
	SWC_KEY_EJECT = 0x55,		 // Disk
	SWC_KEY_RIGHT2 = 0x56,		 // Right
	SWC_KEY_LEFT2 = 0x57,		 // Left
	SWC_KEY_UP2 = 0x58,			 // Up
	SWC_KEY_DOWN2 = 0x59,		 // Down
	SWC_KEY_MENU2 = 0x5A,		 // Menu
	SWC_KEY_MENU3 = 0x5B,		 // Menu
	SWC_KEY_OK2 = 0x5C,			 //
	SWC_KEY_MUTE2 = 0x5D,
	SWC_KEY_BACK = 0x5E,  // Return
	SWC_KEY_CHECK = 0x60, // Check
	SWC_KEY_POWER = 0x80, // Power
};

class ButtonEx final
{
public:
	static const char *ToString(Buttons b)
	{
		switch (b)
		{
		case Buttons::SWC_KEY_NONE:
			return "NONE";
		case Buttons::SWC_KEY_MENU:
			return "MENU";
		case Buttons::SWC_KEY_MENU_UP:
			return "MENU_UP";
		case Buttons::SWC_KEY_MENU_DOWN:
			return "MENU_DOWN";
		case Buttons::SWC_KEY_OK:
			return "OK";
		case Buttons::SWC_KEY_ESC:
			return "ESC";
		case Buttons::SWC_KEY_MODE:
			return "MODE";
		case Buttons::SWC_KEY_SRC:
			return "SRC";
		case Buttons::SWC_KEY_SEEK_DOWN:
			return "SEEK_DOWN";
		case Buttons::SWC_KEY_SEEK_UP:
			return "SEEK_UP";
		case Buttons::SWC_KEY_VOLUME_UP:
			return "VOLUME_UP";
		case Buttons::SWC_KEY_VOLUME_DOWN:
			return "VOLUME_DOWN";
		case Buttons::SWC_KEY_MUTE:
			return "MUTE";
		case Buttons::SWC_KEY_MEMO_UP:
			return "MEMO_UP";
		case Buttons::SWC_KEY_MEMO_DOWN:
			return "MEMO_DOWN";
		case Buttons::SWC_KEY_PAGE_SW:
			return "PAGE_SW";
		case Buttons::SWC_KEY_MENU4:
			return "MENU4";
		case Buttons::SWC_KEY_MEMO:
			return "MEMO";
		case Buttons::SWC_KEY_BT:
			return "BT";
		case Buttons::SWC_KEY_PUSH_TO_TALK:
			return "PUSH_TO_TALK";
		case Buttons::SWC_KEY_VEHICLE_SETTING:
			return "VEHICLE_SETTING";
		case Buttons::SWC_KEY_VEHICLE_NAVI:
			return "VEHICLE_NAVI";
		case Buttons::SWC_KEY_MUSIC:
			return "MUSIC";
		case Buttons::SWC_KEY_BLUETOOTH:
			return "BLUETOOTH";
		case Buttons::SWC_KEY_APPS:
			return "APPS";
		case Buttons::SWC_KEY_AIR_CONDTION_CONTROL:
			return "AIR_CONDTION_CONTROL";
		case Buttons::SWC_KEY_PHONE_ACCEPT:
			return "PHONE_ACCEPT";
		case Buttons::SWC_KEY_PHONE_HANGUP:
			return "PHONE_HANGUP";
		case Buttons::SWC_KEY_NAVI:
			return "NAVI";
		case Buttons::SWC_KEY_RADIO:
			return "RADIO";
		case Buttons::SWC_KEY_SETUP:
			return "SETUP";
		case Buttons::SWC_KEY_ADDR:
			return "ADDR";
		case Buttons::SWC_KEY_MEDIA:
			return "MEDIA";
		case Buttons::SWC_KEY_TRAF:
			return "TRAF";
		case Buttons::SWC_KEY_UP:
			return "UP";
		case Buttons::SWC_KEY_DOWN:
			return "DOWN";
		case Buttons::SWC_KEY_LEFT:
			return "LEFT";
		case Buttons::SWC_KEY_RIGHT:
			return "RIGHT";
		case Buttons::SWC_KEY_SCROLL_UP:
			return "SCROLL_UP";
		case Buttons::SWC_KEY_SCROLL_DOWN:
			return "SCROLL_DOWN";
		case Buttons::SWC_KEY_NUM1:
			return "NUM1";
		case Buttons::SWC_KEY_NUM2:
			return "NUM2";
		case Buttons::SWC_KEY_NUM3:
			return "NUM3";
		case Buttons::SWC_KEY_NUM4:
			return "NUM4";
		case Buttons::SWC_KEY_NUM5:
			return "NUM5";
		case Buttons::SWC_KEY_NUM6:
			return "NUM6";
		case Buttons::SWC_KEY_SRC2:
			return "SRC2";
		case Buttons::SWC_KEY_BAND:
			return "BAND";
		case Buttons::SWC_KEY_LIST:
			return "LIST";
		case Buttons::SWC_KEY_SOUND:
			return "SOUND";
		case Buttons::SWC_KEY_TA:
			return "TA";
		case Buttons::SWC_KEY_DARK:
			return "DARK";
		case Buttons::SWC_KEY_EJECT:
			return "EJECT";
		case Buttons::SWC_KEY_RIGHT2:
			return "RIGHT2";
		case Buttons::SWC_KEY_LEFT2:
			return "LEFT2";
		case Buttons::SWC_KEY_UP2:
			return "UP2";
		case Buttons::SWC_KEY_DOWN2:
			return "DOWN2";
		case Buttons::SWC_KEY_MENU2:
			return "MENU2";
		case Buttons::SWC_KEY_MENU3:
			return "MENU3";
		case Buttons::SWC_KEY_OK2:
			return "OK2";
		case Buttons::SWC_KEY_MUTE2:
			return "MUTE2";
		case Buttons::SWC_KEY_BACK:
			return "BACK";
		case Buttons::SWC_KEY_CHECK:
			return "CHECK";
		case Buttons::SWC_KEY_POWER:
			return "POWER";
		default:
			return "[UNKNOWN]";
		}
	}
};