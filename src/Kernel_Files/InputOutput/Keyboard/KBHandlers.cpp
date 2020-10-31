#include "../../CommandLineInterface/CLI.hpp"
#include "../Text/TextPrint.hpp"
#include "./KBHandlers.hpp"

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
			if (CLI::CursorPosition > CLI::FirstLetterPositions[CLI::CursorLine])
			{
				CLI::MoveCursorPosition(CLI::CursorPosition - 1);
				PrintChar(' ');
				CLI::MoveCursorPosition(CLI::CursorPosition - 1);
				CLI::FinalLetterPositions[CLI::CursorLine] - 2; // Take one off where the final letter is, and another because print char increments it
			}
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
		CLI::MoveCursorPosition(CLI::CursorPosition + VGA_WIDTH);
		break;
	case 0x48: // Up Arrow
		CLI::MoveCursorPosition(CLI::CursorPosition - VGA_WIDTH);
		break;
	case 0x4B: // Left Arrow
		CLI::MoveCursorPosition(CLI::CursorPosition - 1);
		break;
	case 0x4D:											  // Right Arrow
		CLI::MoveCursorPosition(CLI::CursorPosition + 1); // Stop being able to go past end of line
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
		PrintChar(GetCharAtPos(CLI::CursorPosition));
		break;
	case 0x9D:
		PrintChar(' ');
		break;
	default:
		PrintChar(' ');
		PrintString(HexToString(scanCode));
	}
}