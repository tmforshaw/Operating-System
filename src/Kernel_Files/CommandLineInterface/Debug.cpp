#include "../InputOutput/Text/TextModeColourCodes.hpp"
#include "../InputOutput/Text/TextPrint.hpp"
#include "./CLI.hpp"
#include "./Debug.hpp"

const char* Debug::DebugPrefix = "Debug: ";
uint_16 Debug::CurrentCursorLine = VGA_HEIGHT - Debug::DebugSize;
uint_8 Debug::DebugColour = BG_DARKGREY | FG_WHITE;

void Debug::Initialise()
{
	CurrentCursorLine = VGA_HEIGHT - Debug::DebugSize;
}

void Debug::Clear()
{
	uint_16 PreviousCursorPosition = CLI::CursorPosition;
	CLI::SetCursorPosition((VGA_HEIGHT - Debug::DebugSize) * VGA_WIDTH);

	for (uint_16 i = 0; i < DebugSize; i++) // Will not show anything if the size is zero
	{
		// Set 4 Places at a time
		uint_64 value = 0;
		value += (uint_64)Debug::DebugColour << 8;
		value += (uint_64)Debug::DebugColour << 24;
		value += (uint_64)Debug::DebugColour << 40;
		value += (uint_64)Debug::DebugColour << 56;

		for (uint_64* j = (uint_64*)(VGA_MEMORY + (VGA_HEIGHT * 2 - Debug::DebugSize * 2) * VGA_WIDTH); j < (uint_64*)(VGA_MEMORY + VGA_WIDTH * VGA_HEIGHT * 2); j++) // Times 2 becaue of colours
			*j = value;

		CLI::FirstLetterPositions[VGA_HEIGHT - Debug::DebugSize + i] = 0; // Reset first letters
	}

	CLI::SetCursorPosition(PreviousCursorPosition);
}

template<typename T>
void Log(T value)
{
	if (Debug::DebugSize > 0) // Has a debug screen
	{
		uint_16 PreviousCursorPosition = CLI::CursorPosition;
		CLI::SetCursorPosition(CLI::FirstLetterPositions[Debug::CurrentCursorLine] + Debug::CurrentCursorLine * VGA_WIDTH);

		PrintString(value, Debug::DebugColour);

		CLI::FirstLetterPositions[Debug::CurrentCursorLine] = CLI::CursorPosition - Debug::CurrentCursorLine * VGA_WIDTH; // Set new first position

		CLI::SetCursorPosition(PreviousCursorPosition);
	}
}

template<typename T>
void LogInt(T value)
{
	if (Debug::DebugSize > 0) // Has a debug screen
	{
		uint_16 PreviousCursorPosition = CLI::CursorPosition;
		CLI::SetCursorPosition(CLI::FirstLetterPositions[Debug::CurrentCursorLine] + Debug::CurrentCursorLine * VGA_WIDTH);

		PrintString(IntegerToString(value), Debug::DebugColour);

		CLI::FirstLetterPositions[Debug::CurrentCursorLine] = CLI::CursorPosition - Debug::CurrentCursorLine * VGA_WIDTH; // Set new first position

		CLI::SetCursorPosition(PreviousCursorPosition);
	}
}

template<typename T>
void LogHex(T value)
{
	if (Debug::DebugSize > 0) // Has a debug screen
	{
		uint_16 PreviousCursorPosition = CLI::CursorPosition;
		CLI::SetCursorPosition(CLI::FirstLetterPositions[Debug::CurrentCursorLine] + Debug::CurrentCursorLine * VGA_WIDTH);

		PrintString(HexToString(value), Debug::DebugColour);

		CLI::FirstLetterPositions[Debug::CurrentCursorLine] = CLI::CursorPosition - Debug::CurrentCursorLine * VGA_WIDTH; // Set new first position

		CLI::SetCursorPosition(PreviousCursorPosition);
	}
}

void Debug::Log(const char* value) { ::Log<const char*>(value); }
void Debug::Log(Type::String value) { ::Log<Type::String>(value); }

void Debug::LogChar(char value)
{
	if (Debug::DebugSize > 0) // Has a debug screen
	{
		uint_16 PreviousCursorPosition = CLI::CursorPosition;
		CLI::SetCursorPosition(CLI::FirstLetterPositions[Debug::CurrentCursorLine] + Debug::CurrentCursorLine * VGA_WIDTH);

		PrintChar(value, Debug::DebugColour);

		CLI::FirstLetterPositions[Debug::CurrentCursorLine]++; // Set new first position

		CLI::SetCursorPosition(PreviousCursorPosition);
	}
}

void Debug::LogInt(uint_8 value) { ::LogInt<uint_8>(value); }
void Debug::LogInt(uint_16 value) { ::LogInt<uint_16>(value); }
void Debug::LogInt(uint_32 value) { ::LogInt<uint_32>(value); }
void Debug::LogInt(uint_64 value) { ::LogInt<uint_64>(value); }
void Debug::LogInt(char value) { ::LogInt<char>(value); }
void Debug::LogInt(short value) { ::LogInt<short>(value); }
void Debug::LogInt(int value) { ::LogInt<int>(value); }
void Debug::LogInt(long long value) { ::LogInt<long long>(value); }

void Debug::LogHex(uint_8 value) { ::LogHex<uint_8>(value); }
void Debug::LogHex(uint_16 value) { ::LogHex<uint_16>(value); }
void Debug::LogHex(uint_32 value) { ::LogHex<uint_32>(value); }
void Debug::LogHex(uint_64 value) { ::LogHex<uint_64>(value); }
void Debug::LogHex(char value) { ::LogHex<char>(value); }
void Debug::LogHex(short value) { ::LogHex<short>(value); }
void Debug::LogHex(int value) { ::LogHex<int>(value); }
void Debug::LogHex(long long value) { ::LogHex<long long>(value); }