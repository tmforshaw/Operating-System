#include "../InputOutput/Text/TextPrint.hpp"
#include "../Memory/Heap.hpp"
#include "../Types/Types.hpp"
#include "./CLI.hpp"
#include "./Commands.hpp"
#include "./Debug.hpp"

namespace CLI
{
	// void (*Command::callFunction)(const char strArgs[], const uint_16 wordPositions[][2], const char* (&ParseWord)(const uint_16 index, const char strArgs[], const uint_16 wordPositions[][2])) {}; // Function Pointer

	Command::Command() : m_name("") {}
	Command::Command(const char* name) : m_name(name) {}

	Command::Command(const char* name,
					 void (*function)(const char* strArgs, const uint_16 wordPositions[][2], const uint_16 wordCount, const char* (&ParseWord)(const uint_16 index, const char strArgs[], const uint_16 wordPositions[][2])))
		: m_name(name), callFunction(function) {}

	void EchoCommand(const char strArgs[], const uint_16 wordPositions[][2], const uint_16 wordCount, const char* (&ParseWord)(const uint_16 index, const char strArgs[], const uint_16 wordPositions[][2]))
	{
		Debug::Log("Echo Executed");

		// Get the rest of the message

		uint_16 length = wordPositions[wordCount - 1][1] - wordPositions[0][1] - 1; // Final - End of first and -1 To get rid of initial space

		char* message = (char*)calloc(length + 1, sizeof(char)); // I could just print each char individually but this makes debugging easier

		for (uint_16 i = wordPositions[1][0] - 1; i <= wordPositions[wordCount - 1][1]; i++)
			message[i - wordPositions[1][0]] = strArgs[i];

		message[length] = 0;

		CLI::SetCursorPosition(VGA_WIDTH * (CLI::CursorLine + 1)); // Next line

		PrintString(message);

		free(message); // Free the memory of message
	}

	Command* cmds;
	uint_16 cmdAmt;

	void InitialiseCommands()
	{
		// clang-format off
        const char* cmdNames[] = {
            "echo",
            "test"
        };

        void (*functions[])(const char strArgs[], const uint_16 wordPositions[][2], const uint_16 wordCount, const char* (&ParseWord)(const uint_16 index, const char strArgs[], const uint_16 wordPositions[][2])) = {
            EchoCommand,
            nullptr
        };

		// clang-format on

		cmdAmt = sizeof(cmdNames) / sizeof(cmdNames[0]);

		cmds = (Command*)calloc(cmdAmt, sizeof(Command)); // Asign new value

		for (uint_16 i = 0; i < cmdAmt; i++)
		{
			if (functions[i] != nullptr)
				cmds[i] = Command(cmdNames[i], functions[i]);
			else
				cmds[i] = Command(cmdNames[i]);
		}
	}

	void DestroyCommands() { free(cmds); }

	void ExecuteCommand(const char strArgs[], const uint_16 wordPositions[][2], const uint_16 wordCount, const char* (&ParseWord)(const uint_16 index, const char strArgs[], const uint_16 wordPositions[][2]))
	{
		const char* commandWord = ParseWord(0, strArgs, wordPositions);
		uint_16 cmdLength = 0;
		while (commandWord[cmdLength] != 0) cmdLength++;

		for (uint_16 i = 0; i < cmdAmt; i++)
		{
			uint_16 length = 0;
			while (cmds[i].m_name[length] != 0) length++;

			if (length == cmdLength)
			{
				for (uint_16 j = 0; j < length; j++) // Check each letter is the same
				{
					if (cmds[i].m_name[j] != commandWord[j]) // If not the same
						goto ExitCompareCheck;				 // Exit full loop
				}

				cmds[i].callFunction(strArgs, wordPositions, wordCount, ParseWord); // Call the function
				break;																// Exit the loop
			}
		}
	ExitCompareCheck:

		free((void*)commandWord); // Free memory of the word
	}							  // namespace CLI
} // namespace CLI