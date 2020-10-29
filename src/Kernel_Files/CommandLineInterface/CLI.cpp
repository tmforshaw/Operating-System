#include "../InputOutput/Text/TextPrint.hpp"
#include "CLI.hpp"

namespace CLI
{
	const char* outPrefix = "$ ";

	// int size = 0;
	// while (outPrefix[size] != 0) size++; // Null-terminated

	// const uint_8 prefixLen
	// 	= outPrefix.size();

	// clang-format off
	const char* commands[] = {
		"echo"
    };

	// clang-format on

	void PrintPrefix() { PrintString(outPrefix); }

	void Initialise()
	{
		// PrintPrefix();
	}

	void ParseCommands(uint_8 line, uint_16 position)
	{
	}

} // Namespace CLI