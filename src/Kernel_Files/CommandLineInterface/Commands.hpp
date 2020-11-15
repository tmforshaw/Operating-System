#pragma once

namespace CLI
{
	namespace Commands
	{
		struct Command;

		// clang-format off
	    extern const Command commands[];

		// clang-format on

		void ExecuteCommand(const char strArgs[], const uint_16 wordPositions[][2], const char* (&ParseWord)(const uint_16 index, const char strArgs[], const uint_16 wordPositions[][2]));

	} // Namespace Commands

} // Namespace CLI