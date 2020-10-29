#include "../InputOutput/Text/TextPrint.hpp"
#include "../Types/String.hpp"
#include "CLI.hpp"

namespace CLI
{
	const char* outPrefix = "$ ";

	uint_16 FirstPosition = 2;

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