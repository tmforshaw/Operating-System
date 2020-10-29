#include "../../CommandLineInterface/CLI.hpp"
#include "../Text/TextPrint.hpp"
#include "KBHandlers.hpp"

bool LShiftPressed = false;
bool RShiftPressed = false;
uint_8 LastScanCode = 0x00;

void StandardKeyboardHandler(uint_8 scanCode, uint_8 chr)
{
	if (chr != 0) // Not a null character
	{
		if (LShiftPressed || RShiftPressed)
			PrintChar(chr - 32); // Make uppercase
		else
			PrintChar(chr);
	}
	else
	{
		switch (scanCode)
		{
		case 0x8E: // Backspace
			SetCursorPosition(CursorPosition - 1);
			PrintChar(' ');
			SetCursorPosition(CursorPosition - 1);
			FinalLetterPosition - 2; // Take one off where the final letter is, and another because print char increments it
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
			FinalLetterPosition += VGA_WIDTH;
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
		if (CursorPosition + VGA_WIDTH < FinalLetterPosition)
			SetCursorPosition(CursorPosition + VGA_WIDTH);
		break;
	case 0x48: // Up Arrow
		SetCursorPosition(CursorPosition - VGA_WIDTH);
		break;
	case 0x4B: // Left Arrow
		if (CursorPosition > CLI::FirstPosition)
			SetCursorPosition(CursorPosition - 1);
		break;
	case 0x4D: // Right Arrow
		if (CursorPosition < FinalLetterPosition)
			SetCursorPosition(CursorPosition + 1); // Stop being able to go past end of line
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

void DebugKeyboardHandler(uint_8 scanCode, uint_8 chr)
{
	switch (scanCode)
	{
	case 0x1D:
		PrintString("Char: ");
		PrintChar(GetCharAtPos(CursorPosition));
		break;
	case 0x9D:
		PrintChar(' ');
		break;
	default:
		PrintChar(' ');
		PrintString(HexToString(scanCode));
	}
}