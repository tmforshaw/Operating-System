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

	void PrintPrefix();
	void Initialise();

	void SetCursorPosition(uint_16 position);				// Set the position of the cursor
	void MoveCursorPosition(uint_16 position);				// Moves the cursor and accounts for overflow
	void ClearScreen(uint_64 ClearColour = DEFAULT_COLOUR); // Clear screen to particular colour

} // namespace CLI
