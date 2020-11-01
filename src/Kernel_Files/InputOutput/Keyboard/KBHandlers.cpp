#include "../../CommandLineInterface/CLI.hpp"
#include "../../CommandLineInterface/Debug.hpp"
#include "../Text/TextPrint.hpp"
#include "./KBHandlers.hpp"

bool LShiftPressed = false;
bool RShiftPressed = false;
uint_8 LastScanCode = 0x00;

void StandardKeyboardHandler(uint_8 scanCode, uint_8 chr)
{
	if (chr != 0) // Not a null character
	{
		if (CLI::CursorPosition < CLI::MaxCursorLine * VGA_WIDTH) // Stop overflow
		{
			if (LShiftPressed || RShiftPressed)
				PrintChar(chr - 32); // Make uppercase
			else
				PrintChar(chr);
		}

		if (CLI::CursorPosition == CLI::MaxCursorLine * VGA_WIDTH) // Reset position
			CLI::SetCursorPosition(CLI::MaxCursorLine * VGA_WIDTH - 1);
	}
	else
	{
		switch (scanCode)
		{
		case 0x8E: // Backspace
			if (CLI::CursorPosition > CLI::FirstLetterPositions[CLI::CursorLine])
			{
				bool wasOnNullFinalChar = (CLI::CursorPosition == CLI::FinalLetterPositions[CLI::CursorLine] + CLI::CursorLine * VGA_WIDTH);

				// Allows you to delete the char before CursorPosition when on the final character of a line (the space)
				CLI::ShiftLine(CLI::CursorPosition - wasOnNullFinalChar, -1); // Remove character
				if (CLI::FinalLetterPositions[CLI::CursorLine] > CLI::FirstLetterPositions[CLI::CursorLine])
					CLI::FinalLetterPositions[CLI::CursorLine]--;
				else
					CLI::FinalLetterPositions[CLI::CursorLine] = CLI::FirstLetterPositions[CLI::CursorLine];

				if (CLI::CursorPosition % VGA_WIDTH == 0) // Just left a previous line
				{
					CLI::SetCursorPosition(CLI::FinalLetterPositions[CLI::CursorLine - 1] + (CLI::CursorLine - 1) * VGA_WIDTH);

					if (CLI::FinalLetterPositions[CLI::CursorLine + 1] == CLI::FirstLetterPositions[CLI::CursorLine + 1])
						CLI::FinalCursorLine--;
				}
				else
					CLI::SetCursorPosition(CLI::CursorPosition - 1); // Set new position
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

			if (CLI::CursorLine < CLI::MaxCursorLine) // Stop overflow
			{
				if (CLI::CursorLine == CLI::FinalCursorLine) // On the final line
				{
					CLI::FinalCursorLine++;
					CLI::SetCursorPosition(CLI::FirstLetterPositions[CLI::CursorLine + 1] + (CLI::CursorLine + 1) * VGA_WIDTH);
					break;
				}

				// Same code as Down Arrow
				if (CLI::CursorPosition + VGA_WIDTH <= CLI::FinalLetterPositions[CLI::CursorLine + 1] + (CLI::CursorLine + 1) * VGA_WIDTH) // If you are above the char of the below line
					CLI::SetCursorPosition(CLI::CursorPosition + VGA_WIDTH);
				else
					CLI::SetCursorPosition(CLI::FinalLetterPositions[CLI::CursorLine + 1] + (CLI::CursorLine + 1) * VGA_WIDTH);
			}
			break;
		default:
			break;
		}
	}

	LastScanCode = scanCode;
}

int a = 0;

void KeyboardHandler0xE0(uint_8 scanCode)
{
	switch (scanCode)
	{
	case 0x50: // Down Arrow

		if (CLI::CursorPosition < CLI::FirstLetterPositions[CLI::FinalCursorLine] + CLI::FinalCursorLine * VGA_WIDTH) // Not too far down
		{
			if (CLI::CursorPosition + VGA_WIDTH <= CLI::FinalLetterPositions[CLI::CursorLine + 1] + (CLI::CursorLine + 1) * VGA_WIDTH)
				CLI::SetCursorPosition(CLI::CursorPosition + VGA_WIDTH);
			else
				CLI::SetCursorPosition(CLI::FinalLetterPositions[CLI::CursorLine + 1] + (CLI::CursorLine + 1) * VGA_WIDTH);
		}
		else
		{
			CLI::SetCursorPosition(CLI::FinalLetterPositions[CLI::FinalCursorLine] + CLI::FinalCursorLine * VGA_WIDTH);
		}

		break;
	case 0x48: // Up Arrow

		// Gets rid of overflow
		if (CLI::CursorPosition > VGA_WIDTH)
		{
			if (CLI::CursorPosition - VGA_WIDTH > CLI::FinalLetterPositions[CLI::CursorLine - 1] + (CLI::CursorLine - 1) * VGA_WIDTH) // Next line doesn't have enough letters
				CLI::SetCursorPosition(CLI::FinalLetterPositions[CLI::CursorLine - 1] + (CLI::CursorLine - 1) * VGA_WIDTH);
			else
				CLI::SetCursorPosition(CLI::CursorPosition - VGA_WIDTH);
		}
		else
		{
			CLI::SetCursorPosition(CLI::FirstLetterPositions[0]);
		}

		break;
	case 0x4B: // Left Arrow

		// Gets rid of overflow
		if (CLI::CursorPosition >= CLI::FirstLetterPositions[CLI::CursorLine] + 1)
		{
			if (CLI::CursorPosition - 1 < CLI::FirstLetterPositions[CLI::CursorLine] + CLI::CursorLine * VGA_WIDTH)			// Went too far left
				CLI::SetCursorPosition(CLI::FinalLetterPositions[CLI::CursorLine - 1] + (CLI::CursorLine - 1) * VGA_WIDTH); // Loop it back to last letter of last light
			else
				CLI::SetCursorPosition(CLI::CursorPosition - 1); // Loop it back to last letter of last light
		}
		else
		{
			CLI::SetCursorPosition(CLI::FirstLetterPositions[CLI::CursorLine]);
		}

		break;
	case 0x4D: // Right Arrow

		if (CLI::CursorPosition >= CLI::FinalLetterPositions[CLI::CursorLine] + CLI::CursorLine * VGA_WIDTH) // Went too far right
		{
			if (CLI::CursorLine < CLI::FinalCursorLine) // Is less than last cursor line
				CLI::SetCursorPosition(CLI::FirstLetterPositions[CLI::CursorLine + 1] + (CLI::CursorLine + 1) * VGA_WIDTH);
			else
				CLI::SetCursorPosition(CLI::FinalLetterPositions[CLI::CursorLine] + CLI::CursorLine * VGA_WIDTH);
		}
		else
		{
			CLI::SetCursorPosition(CLI::CursorPosition + 1);
		}

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
		PrintString(IntegerToString(scanCode));
	}
}