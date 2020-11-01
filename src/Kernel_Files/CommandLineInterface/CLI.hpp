#pragma once
#include "../InputOutput/Text/TextModeColourCodes.hpp"
#include "../Types/Types.hpp"

namespace CLI
{
	extern const char* commands[];

	extern uint_16 CursorPosition;
	extern uint_16 CursorLine;
	extern uint_16 FirstLetterPositions[];
	extern uint_16 FinalLetterPositions[];
	extern uint_16 FinalCursorLine;

	extern uint_16 MaxCursorLine;

	void PrintPrefix(uint_16 position = 0);
	void Initialise();

	void SetCursorPosition(uint_16 position);		  // Set the position of the cursor
	void ShiftLine(uint_16 position, short shiftAmt); // Shifts the line over and replaces the character at the index

	void ClearScreen(uint_64 ClearColour = DEFAULT_COLOUR); // Clear screen to particular colour

} // namespace CLI
