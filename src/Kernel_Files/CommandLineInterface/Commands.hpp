#pragma once

namespace CLI
{
	struct Command
	{
		const char* m_name;
		void (*callFunction)(const char[], const uint_16[][2], const uint_16); // Function Pointer

		Command();
		Command(const char* name);

		Command(const char* name,
				void (*)(const char*, const uint_16[][2], const uint_16));
	};

	extern Command* cmds;

	void InitialiseCommands();
	void DestroyCommands();

	void ExecuteCommand(const char[], const uint_16[][2], const uint_16);

} // Namespace CLI