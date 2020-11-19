#pragma once
#include "../InputOutput/Text/TextModeColourCodes.hpp"
#include "../Types/Types.hpp"

#define VGA_MEMORY (uint_8*)0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

namespace CLI
{
	extern uint_16 CursorPosition;
	extern uint_16 CursorLine;
	extern uint_16 FirstLetterPositions[];
	extern uint_16 FinalLetterPositions[];
	extern uint_16 CurrentTypingLine;
	extern uint_16 FinalCursorLine;

	extern uint_16 MaxCursorLine;

	extern char charGrid[][VGA_WIDTH];
	extern uint_8 colGrid[][VGA_WIDTH];

	void PrintPrefix(uint_16 line = 0);
	void DisplayScreen();
	void Initialise();
	void Destroy();

	void SetCursorPosition(uint_16 position); // Set the position of the cursor
	void ShiftLine(uint_16 position, short);  // Shifts the line over and replaces the character at the index

	void ClearScreen(uint_64 ClearColour = DEFAULT_COLOUR, bool showWelcome = false); // Clear screen to particular colour

	void ParseCommand(const uint_16& line);

} // namespace CLI
