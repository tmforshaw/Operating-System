#include "../InputOutput/Text/TextModeColourCodes.hpp"
#include "../InputOutput/Text/TextPrint.hpp"
#include "./CLI.hpp"
#include "./Debug.hpp"

const char* Debug::DebugPrefix = "Debug: ";
uint_16 Debug::CurrentCursorLine = VGA_HEIGHT - Debug::DebugSize;
uint_8 Debug::DebugColour = BG_LIGHTBLUE | FG_DARKGREY;

void Debug::Initialise()
{
	CurrentCursorLine = VGA_HEIGHT - Debug::DebugSize;
}

void Debug::ShowDebugScreen()
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

		for (uint_64* j = (uint_64*)(VGA_MEMORY + (VGA_HEIGHT - Debug::DebugSize) * VGA_WIDTH); j < (uint_64*)(VGA_MEMORY + VGA_WIDTH * VGA_HEIGHT); j++)
			*j = value;
	}

	CLI::SetCursorPosition(PreviousCursorPosition);
}

template<typename T, typename... Tv>
void Log(T message, Tv... values)
{
	if (Debug::DebugSize > 0) // Has a debug screen
	{
		uint_16 PreviousCursorPosition = CLI::CursorPosition;

		CLI::SetCursorPosition(CLI::FirstLetterPositions[Debug::CurrentCursorLine] + Debug::CurrentCursorLine * VGA_WIDTH);

		PrintString(message);

		auto test[] = (values, ...);

		// for (auto... values)
		// {
		// 	if (i == 0)
		// 		PrintString(": ");

		// 	PrintString(values[i]);

		// 	if (i != sizeof(values) / sizeof(values[0]) - 1)
		// 		PrintString(", ");
		// }

		CLI::SetCursorPosition(PreviousCursorPosition);
	}
}

void Debug::Log(const char* value) { ::Log(value); }
void Debug::Log(Type::String value) { ::Log(value); }

void Debug::Log(const char* message, const char* values...) { ::Log(message, values); }
void Debug::Log(Type::String message, Type::String values...) { ::Log(message, values); }

void Debug::Log(Type::String message, const char* values...) { ::Log(message, values); }
void Debug::Log(const char* message, Type::String values...) { ::Log(message, values); }