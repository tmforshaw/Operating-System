#include "../IO.hpp"
#include "./TextPrint.hpp"

uint_16 PositionFromCoords(uint_8 x, uint_8 y) // Turn an X and Y value into the index of the position on screen
{
	return y * VGA_WIDTH + x;
}

void PrintString(const char* str, uint_8 colour) // Used to print a string to the screen
{
	uint_8* charPtr = (uint_8*)str;
	uint_16 index = CLI::CursorPosition;

	while (*charPtr != 0) // Null terminated string
	{
		switch (*charPtr)
		{
		case '\n':
			index += VGA_WIDTH;
			break;
		case '\r':
			index -= index % VGA_WIDTH;
			break;
		default:
			*(VGA_MEMORY + index * 2) = *charPtr;	// Multiplied by 2 because of formatting value
			*(VGA_MEMORY + index * 2 + 1) = colour; // Set the formatting value
			index++;
			CLI::FinalLetterPositions[CLI::CursorLine]++;
		}

		charPtr++;
	}

	CLI::SetCursorPosition(index);
}

void PrintString(Type::String str, uint_8 colour) // Used to print a string to the screen
{
	uint_16 index = CLI::CursorPosition;

	for (uint_16 i = 0; i < str.Length(); i++)
	{
		// Handle overflow
		switch (str[i])
		{
		case '\n':
			index += VGA_WIDTH;
			break;
		case '\r':
			index -= index % VGA_WIDTH;
			break;
		default:
			*(VGA_MEMORY + index * 2) = str[i];		// Multiplied by 2 because of formatting value
			*(VGA_MEMORY + index * 2 + 1) = colour; // Set the formatting value
			index++;

			if (index == CLI::FinalLetterPositions[CLI::CursorLine] + CLI::CursorLine * VGA_WIDTH) // Is the last char on line
				CLI::FinalLetterPositions[CLI::CursorLine]++;

			if (CLI::CursorLine < CLI::CursorPosition / VGA_WIDTH) // Check if we are on next line
			{
				CLI::CursorLine++;

				// if (CLI::CursorLine > CLI::FinalCursorLine) // If that was the final line
				// 	CLI::FinalCursorLine++;
			}
		}
	}

	CLI::SetCursorPosition(index);
}

void PrintChar(char chr, uint_8 colour)
{
	if (CLI::CursorPosition != CLI::FinalLetterPositions[CLI::CursorLine] + CLI::CursorLine * VGA_WIDTH) // Isn't the last char on line
		CLI::ShiftLine(CLI::CursorPosition, 1);

	CLI::FinalLetterPositions[CLI::CursorLine]++; // Increment final position

	// Set the Video Memory of char and colour
	*(VGA_MEMORY + CLI::CursorPosition * 2) = chr;
	*(VGA_MEMORY + CLI::CursorPosition * 2 + 1) = colour;

	uint_16 PrevCursorLine = CLI::CursorLine;

	CLI::SetCursorPosition(CLI::CursorPosition + 1);

	if (PrevCursorLine < CLI::CursorLine)			// Check if we are on the next line
		if (CLI::CursorLine > CLI::FinalCursorLine) // If that was the final line
			CLI::FinalCursorLine++;
}

char GetCharAtPos(uint_16 position) { return *(VGA_MEMORY + CLI::CursorPosition); }

char HexToStringOutput[128];
template<typename T>
const char* HexToString(T value) // Can be used to print the address of a string or print a hex value
{
	T* valPtr = &value;
	uint_8* ptr;
	uint_8 temp;
	uint_8 size = sizeof(T) * 2 - 1;
	uint_8 i;

	for (i = 0; i < size; i++)
	{
		ptr = (uint_8*)valPtr + i;

		temp = (*ptr & 0xF0) >> 4; // Extract left part of Hex
		HexToStringOutput[size - (i * 2 + 1)] = temp + (temp > 9 ? 55 : 48);
		temp = *ptr & 0x0F; // Extract Right part of Hex
		HexToStringOutput[size - (i * 2 + 0)] = temp + (temp > 9 ? 55 : 48);
	}

	HexToStringOutput[size + 1] = 0; // Null terminate stringS

	return HexToStringOutput;
}

const char* HexToString(uint_8 value) { return HexToString<uint_8>(value); }
const char* HexToString(uint_16 value) { return HexToString<uint_16>(value); }
const char* HexToString(uint_32 value) { return HexToString<uint_32>(value); }
const char* HexToString(uint_64 value) { return HexToString<uint_64>(value); }
const char* HexToString(char value) { return HexToString<char>(value); }
const char* HexToString(short value) { return HexToString<short>(value); }
const char* HexToString(int value) { return HexToString<int>(value); }
const char* HexToString(long long value) { return HexToString<long long>(value); }

char IntegerToStringOutput[128];
template<typename T>
const char* IntegerToString(T value)
{
	bool isNegative = 0;

	if (value < 0)
	{
		isNegative = 1;
		value *= -1;					// Remove sign
		IntegerToStringOutput[0] = '-'; // Add sign char
	}

	uint_8 size = 0;
	uint_64 sizeTester = (uint_64)value;

	while (sizeTester / 10 > 0)
	{
		sizeTester /= 10;
		size++;
	}

	uint_8 index = 0;
	uint_64 newValue = (uint_64)value;

	while (newValue / 10 > 0)
	{
		uint_8 remainder = newValue % 10;
		newValue /= 10;
		IntegerToStringOutput[isNegative + size - index] = remainder + '0';

		index++;
	}

	uint_8 remainder = newValue % 10;
	IntegerToStringOutput[isNegative + size - index] = remainder + '0';
	IntegerToStringOutput[isNegative + size + 1] = 0;

	return IntegerToStringOutput;
}

const char* IntegerToString(uint_8 value) { return IntegerToString<uint_8>(value); }
const char* IntegerToString(uint_16 value) { return IntegerToString<uint_16>(value); }
const char* IntegerToString(uint_32 value) { return IntegerToString<uint_32>(value); }
const char* IntegerToString(uint_64 value) { return IntegerToString<uint_64>(value); }
const char* IntegerToString(char value) { return IntegerToString<char>(value); }
const char* IntegerToString(short value) { return IntegerToString<short>(value); }
const char* IntegerToString(int value) { return IntegerToString<int>(value); }
const char* IntegerToString(long long value) { return IntegerToString<long long>(value); }

char FloatToStringOutput[128];
const char* FloatToString(float value, uint_8 decimalPlaces)
{
	char* intPtr = (char*)IntegerToString((int)value);
	char* floatPtr = FloatToStringOutput;

	if (value < 0) // Negative
		value *= -1;

	while (*intPtr != 0)
	{
		*floatPtr = *intPtr;
		intPtr++;
		floatPtr++;
	}

	*floatPtr = '.'; // Decimal Point
	floatPtr++;

	float newValue = value - (int)value; // Only decimal portion

	for (uint_8 i = 0; i < decimalPlaces; i++)
	{
		newValue *= 10;
		*floatPtr = (int)newValue + '0';
		newValue -= (int)newValue;

		floatPtr++;
	}

	*floatPtr = 0; // Null terminated

	return FloatToStringOutput;
}