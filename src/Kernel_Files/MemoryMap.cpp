#include "MemoryMap.h"
#include "TextPrint.h"

uint_8 UsableMemoryRegionCount;

MemoryMapEntry* UsableMemoryRegions[10];

void PrintMemoryMap(MemoryMapEntry* memoryMap, uint_16 position)
{
	SetCursorPosition(position);

	PrintString("Memory Base: ");
	PrintString(IntegerToString(memoryMap->BaseAddress));
	SetCursorPosition(position + 80); // New line

	PrintString("Region Length: ");
	PrintString(IntegerToString(memoryMap->RegionLength));
	SetCursorPosition(position + 160); // New line

	PrintString("Memory Type: ");
	PrintString(IntegerToString(memoryMap->RegionType));
	SetCursorPosition(position + 240); // New line

	PrintString("Memory Attributes: ");
	PrintString(HexToString(memoryMap->ExtendedAttributes));
	SetCursorPosition(position + 400); // New line
}

bool MemoryRegionsGot = false;

MemoryMapEntry** GetUsableMemoryRegions()
{
	if (MemoryRegionsGot)
		return UsableMemoryRegions;

	uint_8 usableRegionIndex;
	for (uint_8 i = 0; i < sizeof(UsableMemoryRegions) / sizeof(UsableMemoryRegions[0]); i++)
	{
		MemoryMapEntry* memMap = (MemoryMapEntry*)0x5000;
		memMap += i;

		if (memMap->RegionType == 1) // Is Regular RAM
		{
			UsableMemoryRegions[usableRegionIndex] = memMap;
			usableRegionIndex++;
		}
	}

	UsableMemoryRegionCount = usableRegionIndex;

	MemoryRegionsGot = true;
	return UsableMemoryRegions;
}