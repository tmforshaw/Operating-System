#pragma once
#include "../../Types/String.hpp"
#include "../../Types/Types.hpp"
#include "./TextModeColourCodes.hpp"

uint_16 PositionFromCoords(uint_8 x, uint_8 y);						// Turn an X and Y value into the index of the position on screen
void PrintString(const char* str, uint_8 colour = DEFAULT_COLOUR);	// Used to print a string to the screen
void PrintString(Type::String str, uint_8 colour = DEFAULT_COLOUR); // Used to print a string to the screen
void PrintChar(char chr, uint_8 colour = DEFAULT_COLOUR);
char GetCharAtPos(uint_16 position);

const char* HexToString(uint_8 value);
const char* HexToString(uint_16 value);
const char* HexToString(uint_32 value);
const char* HexToString(uint_64 value);
const char* HexToString(char value);
const char* HexToString(short value);
const char* HexToString(int value);
const char* HexToString(long long value);

const char* IntegerToString(uint_8 value);
const char* IntegerToString(uint_16 value);
const char* IntegerToString(uint_32 value);
const char* IntegerToString(uint_64 value);
const char* IntegerToString(char value);
const char* IntegerToString(short value);
const char* IntegerToString(int value);
const char* IntegerToString(long long value);

const char* FloatToString(float value, uint_8 decimalPlaces);