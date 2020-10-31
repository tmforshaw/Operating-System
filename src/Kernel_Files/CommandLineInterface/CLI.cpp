#include "../InputOutput/IO.hpp"
#include "../InputOutput/Text/TextPrint.hpp"
#include "../Types/String.hpp"
#include "./CLI.hpp"

namespace CLI
{
	const char* outPrefix = "$ ";

	uint_16 CursorPosition = 0;
	uint_16 CursorLine = 0;
	uint_16 FirstLetterPositions[VGA_HEIGHT];
	uint_16 FinalLetterPositions[VGA_HEIGHT];

	// clang-format off
	const char* commands[] = {
		"echo"
    };

	// clang-format on

	void PrintPrefix() { PrintString(outPrefix); }

	void Initialise()
	{
		CursorLine = 0;

		for (uint_32 i = 0; i < VGA_HEIGHT; i++) // Set FirstPositions
			while (outPrefix[FirstLetterPositions[i]] != 0) FirstLetterPositions[i]++;

		for (uint_32 i = 0; i < VGA_HEIGHT; i++) // Set FinalPositions
			FinalLetterPositions[i] = FirstLetterPositions[i];
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

	void MoveCursorPosition(uint_16 position)
	{
		uint_16 newPosition = position;

		// Handle overflow
		if (position < CLI::FirstLetterPositions[position / VGA_WIDTH] + (position / VGA_WIDTH) * VGA_WIDTH)
			newPosition = CLI::FirstLetterPositions[position / VGA_WIDTH] + (position / VGA_WIDTH) * VGA_WIDTH;
		else if (position > CLI::FinalLetterPositions[position / VGA_WIDTH] + (position / VGA_WIDTH) * VGA_WIDTH)
			newPosition = CLI::FinalLetterPositions[position / VGA_WIDTH] + (position / VGA_WIDTH) * VGA_WIDTH;

		SetCursorPosition(newPosition);
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

		// Reset FinalPositions
		for (uint_32 i = 0; i < VGA_HEIGHT; i++) // Set FinalPositions
			FinalLetterPositions[i] = FirstLetterPositions[i];

		PrintPrefix();
	}

	void ParseCommands(uint_8 line, uint_16 position)
	{
	}

} // Namespace CLI