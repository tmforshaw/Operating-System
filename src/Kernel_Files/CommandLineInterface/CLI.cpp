#include "../InputOutput/IO.hpp"
#include "../InputOutput/Text/TextPrint.hpp"
#include "../Memory/Heap.hpp"
#include "../Types/String.hpp"
#include "./CLI.hpp"
#include "./Commands.hpp"
#include "./Debug.hpp"

namespace CLI
{
	const char* outPrefix = "$ ";
	const char* welcomeMessage = "Welcome to the Operating System";

	uint_16 CursorPosition = 0;
	uint_16 CursorLine = 0;
	uint_16 FirstLetterPositions[VGA_HEIGHT];
	uint_16 FinalLetterPositions[VGA_HEIGHT];
	uint_16 CurrentTypingLine = 0;
	uint_16 FinalCursorLine = 0;

	uint_16 MaxCursorLine = VGA_HEIGHT - Debug::DebugSize;

	char charGrid[VGA_HEIGHT][VGA_WIDTH];
	uint_8 colGrid[VGA_HEIGHT][VGA_WIDTH];

	void PrintPrefix(uint_16 line)
	{
		SetCursorPosition(VGA_WIDTH * line); // 0th position on the line

		PrintString(outPrefix);

		int len = 0;
		while (outPrefix[len] != 0) len++;

		FirstLetterPositions[line] = len;

		CurrentTypingLine = line;
	}

	void DisplayScreen()
	{
		for (uint_16 i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++)
		{
			// Set 4 Places at a time
			uint_64 value = 0;
			value += ((uint_64)colGrid[i / VGA_WIDTH][i % VGA_WIDTH] << 8) + ((uint_64)charGrid[i / VGA_WIDTH][i % VGA_WIDTH]);
			i++;
			value += ((uint_64)colGrid[i / VGA_WIDTH][i % VGA_WIDTH] << (8 * 3)) + ((uint_64)charGrid[i / VGA_WIDTH][i % VGA_WIDTH] << (8 * 2));
			i++;
			value += ((uint_64)colGrid[i / VGA_WIDTH][i % VGA_WIDTH] << (8 * 5)) + ((uint_64)charGrid[i / VGA_WIDTH][i % VGA_WIDTH] << (8 * 4));
			i++;
			value += ((uint_64)colGrid[i / VGA_WIDTH][i % VGA_WIDTH] << (8 * 7)) + ((uint_64)charGrid[i / VGA_WIDTH][i % VGA_WIDTH] << (8 * 6));

			*((uint_64*)VGA_MEMORY + i / 4) = value;
		}
	}

	void Initialise()
	{
		ClearScreen(DEFAULT_COLOUR, true); // This will initialise CursorLine, CursorPosition, FinalCursorLine, FirstPositions and FinalPositions

		InitialiseCommands(); // Initialise the commands

		DisplayScreen();
	}

	void Destroy()
	{
		DestroyCommands();
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
		const bool sign = shiftAmt >> (sizeof(shiftAmt) * 8 - 1); // Get the most significant bits

		shiftAmt *= (sign ? -1 : +1); // Absolute value of shiftAmt

		for (uint_16 k = 0; k < shiftAmt; k++)
		{
			uint_16 i = position / VGA_WIDTH;
			short j = sign ? VGA_WIDTH - 1 : position % VGA_WIDTH;

			uint_16 buffer = (sign ? j - 1 >= 0 : j + 1 < VGA_WIDTH) ?
								 (((uint_16)colGrid[i][j - 1] << 8) + (uint_16)charGrid[i][j - 1]) :
								 ((DEFAULT_COLOUR << 8) + ' '); // High is col, Low is char

			uint_16 temp; // High is col, Low is char

			for (; (sign ? j >= position % VGA_WIDTH : j < VGA_WIDTH); j += (sign ? -1 : +1)) // Breaks when on new line
			{
				temp = ((uint_16)colGrid[i][j] << 8) + (uint_16)charGrid[i][j];

				colGrid[i][j] = (uint_8)(buffer >> 8);	  // Get High
				charGrid[i][j] = (uint_8)(buffer & 0xFF); // Get Low

				buffer = temp;
			}

			buffer = (DEFAULT_COLOUR << 8) + ' ';
		}
		CLI::DisplayScreen();
	}

	void ClearScreen(uint_64 ClearColour, bool showWelcome) // Clear screen to particular colour
	{
		for (uint_16 i = 0; i < VGA_HEIGHT; i++)
			for (uint_16 j = 0; j < VGA_WIDTH; j++)
			{
				colGrid[i][j] = DEFAULT_COLOUR;
				charGrid[i][j] = ' ';
			}

		SetCursorPosition(0); // Reset cursor position
		FinalCursorLine = 0;

		// Reset FirstPositions
		for (uint_16 i = 0; i < VGA_HEIGHT; i++) // Set FirstPositions
			FirstLetterPositions[i] = 0;

		if (showWelcome)
		{
			// Display a welcome message with 2 newlines
			PrintString(welcomeMessage);
			PrintString("\n\n\r");

			uint_16 length = 0;
			while (welcomeMessage[length] != 0) length++;
			FirstLetterPositions[0] = length;
		}

		PrintPrefix(CLI::CursorLine); // Print the prefix

		Debug::Clear();

		// Reset FinalPositions
		for (uint_16 i = 0; i < VGA_HEIGHT; i++) // Set FinalPositions
			FinalLetterPositions[i] = FirstLetterPositions[i];
	}

	void GetWordPos(const char* string, const uint_16 wordCount, uint_16 (&wordPositions)[][2])
	{
		bool onWord = false;

		uint_16 length = 0;
		while (string[length] != 0) length++;

		for (uint_16 i = 0, wordPosIndex = 0; i < length; i++)
		{
			if (string[i] != ' ' && !onWord) // Just arrived on a word
			{
				onWord = true;
				wordPositions[wordPosIndex][0] = i; // Set startpos
				continue;
			}

			if (string[i] == ' ' && onWord) // Just left a word
			{
				onWord = false;
				wordPositions[wordPosIndex][1] = i - 1; // Set endpos
				wordPosIndex++;
				continue;
			}

			if (i == length - 1)							 // On last one and hasn't been set
				wordPositions[wordPosIndex][1] = length - 1; // Set final position
		}
	}

	void ParseCommand(const uint_16& line)
	{
		char buffer[VGA_WIDTH - CLI::FirstLetterPositions[line]];

		// Get Word Count and String

		uint_16 wordCount = 0;
		bool onWord = false;

		for (uint_16 i = 0; i < sizeof(buffer) / sizeof(buffer[0]); i++)
		{
			buffer[i] = charGrid[line][i + CLI::FirstLetterPositions[line]];

			if (buffer[i] != ' ' && !onWord) // Just arrived on a word
			{
				onWord = true;
				wordCount++;
			}

			if (buffer[i] == ' ' && onWord) // Just left a word
				onWord = false;
		}

		// Find word positions

		uint_16 wordPositions[wordCount][2];
		GetWordPos(buffer, wordCount, wordPositions); // Sets the wordPositions

		// Pass the buffer, wordPositions, and wordCount
		ExecuteCommand(buffer, wordPositions, wordCount);
	}

} // Namespace CLI