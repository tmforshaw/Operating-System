#include "../CommandLineInterface/CLI.hpp"
#include "../InputOutput/Interrupts/IDT.hpp"
#include "../InputOutput/Keyboard/KBHandlers.hpp"
#include "../InputOutput/Text/TextPrint.hpp"
#include "../Memory/Heap.hpp"
#include "../Memory/MemoryMap.hpp"
#include "../Types/String.hpp"
#include "../Types/Types.hpp"

extern "C" void _StartKernel()
{
	ClearScreen(); // Clear Screen
	InitialiseIDT();
	MainKeyboardHandler = &KeyboardHandler; // Set Keyboard Handler
	InitialiseHeap(0x100000, 0x100000);
	MemoryMapEntry** UsableMemoryMaps = GetUsableMemoryRegions(); // Find usable memory

	CLI::Initialise(); // Initialise Command Line Interface

	return;
}
