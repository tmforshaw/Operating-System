#include "Heap.h"
#include "IDT.h"
#include "KBHandlers.h"
#include "MemoryMap.h"
#include "TextPrint.h"

extern "C" void _StartKernel()
{
	ClearScreen(); // Clear Screen
	InitialiseIDT();
	MainKeyboardHandler = &KeyboardHandler; // Set Keyboard Handler
	InitialiseHeap(0x100000, 0x100000);
	MemoryMapEntry** UsableMemoryMaps = GetUsableMemoryRegions(); // Find usable memory

	uint_64* TestAddress = (uint_64*)aligned_alloc(0x4000, 0x08);
	*TestAddress = 12345678;

	PrintString(HexToString((uint_64)TestAddress));
	PrintString("\n\r");

	return;
}