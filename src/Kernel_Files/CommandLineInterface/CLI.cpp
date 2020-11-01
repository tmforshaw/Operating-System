#include "../InputOutput/IO.hpp"
#include "../InputOutput/Text/TextPrint.hpp"
#include "../Types/String.hpp"
#include "./CLI.hpp"
#include "./Debug.hpp"

namespace CLI
{
	Type::String outPrefix = "$ ";

	uint_16 CursorPosition = 0;
	uint_16 CursorLine = 0;
	uint_16 FirstLetterPositions[VGA_HEIGHT];
	uint_16 FinalLetterPositions[VGA_HEIGHT];
	uint_16 FinalCursorLine = 0;

	uint_16 MaxCursorLine = VGA_HEIGHT - Debug::DebugSize;

	// clang-format off
	const char* commands[] = {
		"echo"
    };

	// clang-format on

	void PrintPrefix(uint_16 position)
	{
		SetCursorPosition(position);

		PrintString(outPrefix);

		FirstLetterPositions[CursorLine] += outPrefix.Length();
	}

	void Initialise()
	{
		ClearScreen(); // This will initialise CursorLine, CursorPosition, FinalCursorLine, FirstPositions and FinalPositions
	}

	void SetCursorPosition(uint_16 position) // Set the position of the cursor
	{
		// Send Commands Across IO Bus
		outb(0x3D4, 0x0F);
		outb(0x3D5, (uint_8)(position & 0xFF));
		outb(0x3D4, 0x0E);
		outb(0x3D5, (uint_8)((position >> 8) & 0xFF));

		CursorPosition = position;
		CursorLine = position / VGA_WIDTH;
	}

	void ShiftLine(uint_16 position, short shiftAmt) // Shifts the line over (bytes) and replaces the character at the index
	{
		const bool sign = shiftAmt >> (sizeof(shiftAmt) - 1); // Get the most significant bit

		shiftAmt *= (sign ? -1 : +1); // Absolute value of shiftAmt

		uint_16 temp = (position >= 0) ?
						   *((uint_16*)VGA_MEMORY + position) :
						   (DEFAULT_COLOUR << 8) + ' ';

		uint_16* buffer = (position >= 0 && position < CLI::MaxCursorLine * VGA_WIDTH) ?
							  ((uint_16*)VGA_MEMORY + (sign ? (position / VGA_WIDTH + 1) * VGA_WIDTH - 1 : position)) :
							  &temp;

		for (uint_16 i = 0; i < shiftAmt; i++)
		{
			for (short j = (sign ? (position / VGA_WIDTH + 1) * VGA_WIDTH - 1 : 0);
				 (sign ? position + j >= position :
						 position + j < (position / VGA_WIDTH + 1) * VGA_WIDTH);
				 j += (sign ? -1 : +1))
			{
				temp = *((uint_16*)VGA_MEMORY + position + j);

				*((uint_16*)VGA_MEMORY + position + j) = *buffer;

				*buffer = temp;
			}

			// ((sign) ?
			// 	 *((uint_16*)VGA_MEMORY + position) :
			// 	 *buffer)
			// 	= (DEFAULT_COLOUR << 8) + ' '; // Set value to space

			*buffer = (DEFAULT_COLOUR << 8) + ' '; // Set value to space)
		}
	}

	void ClearScreen(uint_64 ClearColour) // Clear screen to particular colour
	{
		// Set 4 Places at a time
		uint_64 value = 0;
		value += ClearColour << 8;
		value += ClearColour << 24;
		value += ClearColour << 40;
		value += ClearColour << 56;

		for (uint_64* i = (uint_64*)VGA_MEMORY; i < (uint_64*)(VGA_MEMORY + VGA_WIDTH * VGA_HEIGHT); i++)
			*i = value;

		SetCursorPosition(0); // Reset cursor position

		FinalCursorLine = 0;

		// Reset FirstPositions
		for (uint_16 i = 0; i < VGA_HEIGHT; i++) // Set FirstPositions
			FirstLetterPositions[i] = 0;

		PrintPrefix();

		Debug::Clear();

		// Reset FinalPositions
		for (uint_16 i = 0; i < VGA_HEIGHT; i++) // Set FinalPositions
			FinalLetterPositions[i] = FirstLetterPositions[i];
	}

	void ParseCommands(uint_8 line, uint_16 position)
	{
	}

} // Namespace CLI