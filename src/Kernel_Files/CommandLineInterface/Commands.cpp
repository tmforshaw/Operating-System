#include "./Commands.hpp"

struct Command
{
	const char* name;
	virtual void callFuntion(const char* args) {}
};

struct EchoCommand : Command
{
	// void callFunction(const char* args) override
	// {
	// }
};

namespace CLI
{
	// clang-format off
	const char* commands[] = {
		"echo"
    };

	// clang-format on

} // Namespace CLI