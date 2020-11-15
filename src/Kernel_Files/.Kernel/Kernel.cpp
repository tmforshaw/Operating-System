#include "../CommandLineInterface/CLI.hpp"
#include "../CommandLineInterface/Debug.hpp"
#include "../InputOutput/Interrupts/IDT.hpp"
#include "../InputOutput/Keyboard/KBHandlers.hpp"
#include "../InputOutput/Text/TextPrint.hpp"
#include "../Memory/Heap.hpp"
#include "../Memory/Memory.hpp"
#include "../Memory/MemoryMap.hpp"
#include "../Types/String.hpp"
#include "../Types/Types.hpp"

// void *_Unwind_Resume = nullptr;
// void *__gxx_personality_v0 = nullptr;

extern "C" void _StartKernel()
{
	InitialiseIDT();
	MainKeyboardHandler = &ConsoleKeyboardHandler; // Set Keyboard Handler
	InitialiseHeap(0x100000, 0x100000);
	MemoryMapEntry** UsableMemoryMaps = GetUsableMemoryRegions(); // Find usable memory

	CLI::Initialise(); // Initialise Command Line Interface
	Debug::Initialise();

	PrintString("echo one two three aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

	CLI::ParseCommand(0);

	// CLI::ParseCommand(CLI::CursorLine);
	// Type::String testStr = "Test";

	// // Calculate length
	// uint_16 length = testStr.Length();

	// testStr.str_val = (char*)realloc(testStr.str_val, (length + 2) * sizeof(char));

	// void* newMem = calloc(length + 2, sizeof(char));
	// memcpy(newMem, (void*)testStr.str_val, length);

	// PrintString(HexToString((uint_64)testStr.str_val));

	// free(testStr.str_val);

	// Debug::LogHex((uint_64)testStr.str_val);

	// testStr.str_val = (char*)newMem;

	// Debug::Log(" ");

	// Debug::LogHex((uint_64)testStr.str_val);

	// PrintString("   ");
	// PrintString(HexToString((uint_64)testStr.str_val));

	// testStr.str_val[length] = 'A';
	// testStr.str_val[length + 1] = 0; // Null-terminate

	// PrintString(testStr);

	// // Calculate length
	// length = testStr.Length();

	// testStr.str_val = (char*)realloc(testStr.str_val, (length + 2) * sizeof(char));

	// void* newMem2 = calloc(length + 2, sizeof(char));
	// memcpy(newMem, (void*)testStr.str_val, length);
	// free(testStr.str_val);
	// testStr.str_val = (char*)newMem;

	// testStr.str_val[length] = 'B';
	// testStr.str_val[length + 1] = 0; // Null-terminate

	// PrintString(testStr);

	// testStr += 'A';

	// PrintString(testStr);

	// testStr += 'B';

	// PrintString("\n\r");
	// PrintString(testStr.str_val);

	return;
}

// CANNOT DELETE THE LAST LETTER OF A LINE IF YOU ARE ON THE NULL CHAR