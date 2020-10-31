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

	void ShiftLine(uint_16 position, uint_16 shiftAmt) // Shifts the line over and replaces the character at the index
	{
		uint_16 line = position / VGA_WIDTH;
		uint_16 charBuffer = *(VGA_MEMORY + line * VGA_WIDTH + FinalLetterPositions[line]);
		uint_16 colourBuffer = *(VGA_MEMORY + line * VGA_WIDTH + FinalLetterPositions[line] + 1);
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

		Debug::ShowDebugScreen();

		// Reset FinalPositions
		for (uint_16 i = 0; i < VGA_HEIGHT; i++) // Set FinalPositions
			FinalLetterPositions[i] = FirstLetterPositions[i];
	}

	void ParseCommands(uint_8 line, uint_16 position)
	{
	}

} // Namespace CLI