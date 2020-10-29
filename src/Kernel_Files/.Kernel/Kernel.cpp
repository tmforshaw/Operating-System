#include "../InputOutput/Interrupts/IDT.h"
#include "../InputOutput/Keyboard/KBHandlers.h"
#include "../InputOutput/Text/TextPrint.h"
#include "../Memory/Heap.h"
#include "../Memory/MemoryMap.h"

extern "C" void
	_StartKernel()
{
	ClearScreen(); // Clear Screen
	InitialiseIDT();
	MainKeyboardHandler = &KeyboardHandler; // Set Keyboard Handler
	InitialiseHeap(0x100000, 0x100000);
	MemoryMapEntry** UsableMemoryMaps = GetUsableMemoryRegions(); // Find usable memory

	PrintString("Hello World!\n\r");

	return;
}