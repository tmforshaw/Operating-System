#include "../Types/Types.hpp"
#include "./Commands.hpp"
#include "./Debug.hpp"

namespace CLI
{
	namespace Commands
	{
		struct Command
		{
			const char* m_name;
			void (*callFunction)(const char strArgs[], const uint_16 wordPositions[][2], const char* (&ParseWord)(const uint_16 index, const char strArgs[], const uint_16 wordPositions[][2])) {}; // Function Pointer

			Command() : m_name("") {}
			Command(const char* name) : m_name(name) {}

			Command(const char* name,
					void (*function)(const char* strArgs, const uint_16 wordPositions[][2], const char* (&ParseWord)(const uint_16 index, const char strArgs[], const uint_16 wordPositions[][2])))
				: m_name(name), callFunction(function) {}
		};

		void EchoCommand(const char strArgs[], const uint_16 wordPositions[][2], const char* (&ParseWord)(const uint_16 index, const char strArgs[], const uint_16 wordPositions[][2]))
		{
			Debug::Log("Echo Executed");
		}

		// clang-format off
        const Command commands[] = {
            Command("echo", EchoCommand)
        };
		// clang-format on

		void ExecuteCommand(const char strArgs[], const uint_16 wordPositions[][2], const char* (&ParseWord)(const uint_16 index, const char strArgs[], const uint_16 wordPositions[][2]))
		{
			const char* commandWord = ParseWord(0, strArgs, wordPositions);

            Debug::Log(commandWord);

			uint_16 length = 0;
			while (commandWord[length] != 0) 
            {
                Debug::LogInt(commandWord[length]);
                Debug::LogChar(' ');

                length++;
            };
		}

	} // namespace Commands
} // Namespace CLI