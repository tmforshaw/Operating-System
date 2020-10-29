#include "Heap.h"
#include "IDT.h"
#include "KBHandlers.h"
#include "MemoryMap.h"
#include "TextPrint.h"

void InitKernel()
{
	ClearScreen();							// Clear Screen
	InitialiseIDT();						// Initialise Interupt Descriptor Table
	MainKeyboardHandler = &KeyboardHandler; // Set KeyboardHandler
	InitialiseHeap(0x100000, 0x100000);		// Initialise The Heap
}

extern "C" void _StartKernel()
{
	InitKernel();

	(*MainKeyboardHandler)(0x00, 'P');
	KeyboardHandler(0x00, 'P');

	// MemoryMapEntry** UsableMemoryMaps = GetUsableMemoryRegions(); // Find usable memory

	PrintString("Hello World");

	return;
}