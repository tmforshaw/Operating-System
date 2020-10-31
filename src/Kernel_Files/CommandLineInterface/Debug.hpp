#pragma once
#include "../Types/String.hpp"
#include "../Types/Types.hpp"

class Debug
{
public:
	static const char* DebugPrefix;
	const static uint_16 DebugSize = 2;
	static uint_16 CurrentCursorLine;
	static uint_8 DebugColour;

	static void Initialise();
	static void ShowDebugScreen();

	static void Log(const char* value);
	static void Log(Type::String value);

	static void Log(const char* message, const char* values...);
	static void Log(Type::String message, Type::String values...);

	static void Log(Type::String message, const char* values...);
	static void Log(const char* message, Type::String values...);
};