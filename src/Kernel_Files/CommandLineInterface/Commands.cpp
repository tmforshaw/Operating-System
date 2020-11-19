#include "../InputOutput/Text/TextPrint.hpp"
#include "../Memory/Heap.hpp"
#include "../Types/Types.hpp"
#include "./CLI.hpp"
#include "./Commands.hpp"
#include "./Debug.hpp"

namespace CLI
{
	namespace Commands
	{
		void EchoCommand(const char strArgs[], const uint_16 wordPositions[][2], const uint_16 wordCount)
		{
			Debug::Log("Echo Executed");

			// Get the rest of the message

			uint_16 length = wordPositions[wordCount - 1][1] - wordPositions[0][1]; // Final - End of first and -1 To get rid of initial space

			char* message = (char*)calloc(length, sizeof(char)); // I could just print each char individually but this makes debugging easier

			for (uint_16 i = wordPositions[1][0]; i <= wordPositions[wordCount - 1][1]; i++)
				message[i - wordPositions[1][0]] = strArgs[i];

			message[length] = 0;

			Debug::LogInt(length);

			PrintString("\n\r"); // Next line

			PrintString(message);

			free(message); // Free the memory of message

			// Change line
			PrintString("\n\r"); // Increment line
			CLI::PrintPrefix(CLI::CursorLine);
		}

		void ClearCommand(const char strArgs[], const uint_16 wordPositions[][2], const uint_16 wordCount)
		{
			CLI::ClearScreen();
			CLI::DisplayScreen();
		}
	} // namespace Commands

	Command::Command() : m_name("") {}
	Command::Command(const char* name) : m_name(name) {}

	Command::Command(const char* name,
					 void (*function)(const char*, const uint_16[][2], const uint_16))
		: m_name(name), callFunction(function) {}

	Command* cmds;
	uint_16 cmdAmt;

	void InitialiseCommands()
	{
		// clang-format off
        const char* cmdNames[] = {
            "echo",
            "clear"
        };

        void (*functions[])(const char[], const uint_16[][2], const uint_16) = {
            Commands::EchoCommand,
            Commands::ClearCommand
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

	void DestroyCommands()
	{
		free(cmds);
	}

	const char* ParseWord(const uint_16 index, const char buffer[], const uint_16 wordPositions[][2])
	{
		uint_16 length = wordPositions[index][1] - wordPositions[index][0] + 1;
		char* word = (char*)calloc(length + 1, sizeof(char)); // Allocate memory for word

		for (uint_16 j = 0; j < length; j++)
			word[j] = buffer[wordPositions[index][0] + j];

		word[length] = 0; // Null terminate

		return (const char*)word;
	}

	void ExecuteCommand(const char strArgs[], const uint_16 wordPositions[][2], const uint_16 wordCount)
	{
		const char* commandWord = ParseWord(0, strArgs, wordPositions);
		uint_16 cmdLength = 0;
		while (commandWord[cmdLength] != 0) cmdLength++;

		uint_16 length;

		for (uint_16 i = 0; i < cmdAmt; i++)
		{
			length = 0;
			while (cmds[i].m_name[length] != 0) length++;

			if (length == cmdLength)
			{
				for (uint_16 j = 0; j < length; j++) // Check each letter is the same
				{
					if (cmds[i].m_name[j] != commandWord[j]) // If not the same
						goto ExitFalse;						 // Exit loop
				}

				cmds[i].callFunction(strArgs, wordPositions, wordCount); // Call the function
				break;													 // Exit the loop
			}
			else
			{
			ExitFalse:
				if (i == cmdAmt - 1) // Exited false and was the last one
				{
					PrintString("\n\rThe command ");
					PrintString(commandWord);
					PrintString(" is unknown\n\r");
					PrintPrefix(CLI::CursorLine);
				}
			}
		}

		free((void*)commandWord); // Free memory of the word
	}							  // namespace CLI
} // namespace CLI