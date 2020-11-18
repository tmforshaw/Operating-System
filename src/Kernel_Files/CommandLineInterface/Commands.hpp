#pragma once

namespace CLI
{
	struct Command
	{
		const char* m_name;
		void (*callFunction)(const char strArgs[], const uint_16 wordPositions[][2], const uint_16 wordCount, const char* (&ParseWord)(const uint_16 index, const char strArgs[], const uint_16 wordPositions[][2])); // Function Pointer

		Command();
		Command(const char* name);

		Command(const char* name,
				void (*function)(const char* strArgs, const uint_16 wordPositions[][2], const uint_16 wordCount, const char* (&ParseWord)(const uint_16 index, const char strArgs[], const uint_16 wordPositions[][2])));
	};

	extern Command* cmds;

	void InitialiseCommands();
	void DestroyCommands();

	void ExecuteCommand(const char strArgs[], const uint_16 wordPositions[][2], const uint_16 wordCount, const char* (&ParseWord)(const uint_16 index, const char strArgs[], const uint_16 wordPositions[][2]));

} // Namespace CLI