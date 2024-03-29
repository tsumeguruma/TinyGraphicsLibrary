/*
 * Common.h
 */

#ifndef TGL_CORE_COMMON_H_
#define TGL_CORE_COMMON_H_

namespace tgl {

static const char* defaultFontPath = "/usr/share/fonts/truetype/takao-gothic/TakaoPGothic.ttf";

enum class Align {
	Left,
	Center,
	Right,
};

enum class VAlign {
	Top,
	Bottom,
	Center,
	Baseline,
};

enum class Key
{
	Key_Unknown = -1,
	Key_Escape = 0x01000000,// misc keys
	Key_Tab = 0x01000001,

	Key_Return = 0x01000004,
	Key_Enter = 0x01000005,
	Key_Insert = 0x01000006,
	Key_Delete = 0x01000007,

	Key_Home = 0x01000010,// cursor movement
	Key_End = 0x01000011,

	Key_Left = 0x01000012,
	Key_Up = 0x01000013,
	Key_Right = 0x01000014,
	Key_Down = 0x01000015,

	Key_PageUp = 0x01000016,
	Key_PageDown = 0x01000017,

	Key_Shift = 0x01000020,// modifiers
	Key_Control = 0x01000021,
	Key_Alt = 0x01000023,

	Key_F1 = 0x01000030,// function keys
	Key_F2 = 0x01000031,
	Key_F3 = 0x01000032,
	Key_F4 = 0x01000033,
	Key_F5 = 0x01000034,
	Key_F6 = 0x01000035,
	Key_F7 = 0x01000036,
	Key_F8 = 0x01000037,
	Key_F9 = 0x01000038,
	Key_F10 = 0x01000039,
	Key_F11 = 0x0100003a,
	Key_F12 = 0x0100003b,

	Key_Space = 0x20,// 7 bit printable ASCII

	Key_Asterisk = 0x2a,		// *
	Key_Plus = 0x2b,			// +
	Key_Comma = 0x2c,			// ,
	Key_Minus = 0x2d,			// -
	Key_Period = 0x2e,		// .
	Key_Slash = 0x2f,			// /

	Key_0 = 0x30,
	Key_1 = 0x31,
	Key_2 = 0x32,
	Key_3 = 0x33,
	Key_4 = 0x34,
	Key_5 = 0x35,
	Key_6 = 0x36,
	Key_7 = 0x37,
	Key_8 = 0x38,
	Key_9 = 0x39,

	Key_Colon = 0x3a,			// :
	Key_Semicolon = 0x3b,	// ;
	Key_Less = 0x3c,			// <
	Key_Equal = 0x3d,			// =
	Key_Greater = 0x3e,		// >
	Key_Question = 0x3f,		// ?
	Key_At = 0x40,			// @

	Key_A = 0x41,
	Key_B = 0x42,
	Key_C = 0x43,
	Key_D = 0x44,
	Key_E = 0x45,
	Key_F = 0x46,
	Key_G = 0x47,
	Key_H = 0x48,
	Key_I = 0x49,
	Key_J = 0x4a,
	Key_K = 0x4b,
	Key_L = 0x4c,
	Key_M = 0x4d,
	Key_N = 0x4e,
	Key_O = 0x4f,
	Key_P = 0x50,
	Key_Q = 0x51,
	Key_R = 0x52,
	Key_S = 0x53,
	Key_T = 0x54,
	Key_U = 0x55,
	Key_V = 0x56,
	Key_W = 0x57,
	Key_X = 0x58,
	Key_Y = 0x59,
	Key_Z = 0x5a,
};

} /* namespace tgl */

#endif /* COMMON_H_ */
