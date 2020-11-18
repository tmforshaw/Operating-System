#pragma once
#include "../Types/String.hpp"
#include "../Types/Types.hpp"

class Debug
{
public:
	static const char* DebugPrefix;
	const static uint_16 DebugSize = 2;
	static uint_16 CurrentCursorLine;

	static void Initialise();
	static void Clear();

	static void Log(const char* value);
	static void Log(Type::String value);

	static void LogChar(char value);

	static void LogInt(uint_8 value);
	static void LogInt(uint_16 value);
	static void LogInt(uint_32 value);
	static void LogInt(uint_64 value);
	static void LogInt(char value);
	static void LogInt(short value);
	static void LogInt(int value);
	static void LogInt(long long value);

	static void LogHex(uint_8 value);
	static void LogHex(uint_16 value);
	static void LogHex(uint_32 value);
	static void LogHex(uint_64 value);
	static void LogHex(char value);
	static void LogHex(short value);
	static void LogHex(int value);
	static void LogHex(long long value);
};