#include "KBHandlers.h"
#include "TextPrint.h"

bool LShiftPressed = false;
bool RShiftPressed = false;
uint_8 LastScanCode = 0x00;

void StandardKeyboardHandler(uint_8 scanCode, uint_8 chr)
{
	if (chr != 0) // Not a null character
	{
		switch (LShiftPressed || RShiftPressed)
		{
		case true:
			PrintChar(chr - 32); // Make uppercase
			break;
		case false:
			PrintChar(chr);
			break;
		}
	}
	else
	{
		switch (scanCode)
		{
		case 0x8E: // Backspace
			SetCursorPosition(CursorPosition - 1);
			PrintChar(' ');
			SetCursorPosition(CursorPosition - 1);
			break;
		case 0x2A: // LShift
			LShiftPressed = true;
			break;
		case 0xAA: // LShift Released
			LShiftPressed = false;
			break;
		case 0x36: // RShift
			RShiftPressed = true;
			break;
		case 0xB6: // RShift Released
			RShiftPressed = false;
			break;
		case 0x9C: // Enter
			PrintString("\n\r");
			break;
		default:
			break;
		}
	}

	LastScanCode = scanCode;
}

void KeyboardHandler0xE0(uint_8 scanCode)
{
	switch (scanCode)
	{
	case 0x50: // Down Arrow
		SetCursorPosition(CursorPosition + VGA_WIDTH);
		break;
	case 0x48: // Up Arrow
		SetCursorPosition(CursorPosition - VGA_WIDTH);
		break;
	default:
		break;
	}

	LastScanCode = scanCode;
}

void KeyboardHandler(uint_8 scanCode, uint_8 chr)
{
	switch (LastScanCode)
	{
	case 0xE0: // Arrow
		KeyboardHandler0xE0(scanCode);
		break;
	default:
		StandardKeyboardHandler(scanCode, chr);
	}
}