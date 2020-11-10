#include "../CommandLineInterface/CLI.hpp"
#include "../InputOutput/Text/TextPrint.hpp"
#include "./MemoryMap.hpp"

uint_8 UsableMemoryRegionCount;

MemoryMapEntry* UsableMemoryRegions[250];

void PrintMemoryMap(MemoryMapEntry* memoryMap, uint_16 position)
{
	CLI::SetCursorPosition(position);

	PrintString("Memory Base: ");
	PrintString(IntegerToString(memoryMap->BaseAddress));
	CLI::SetCursorPosition(position + VGA_WIDTH); // New line

	PrintString("Region Length: ");
	PrintString(IntegerToString(memoryMap->RegionLength));
	CLI::SetCursorPosition(position + VGA_WIDTH * 2); // New line

	PrintString("Memory Type: ");
	PrintString(IntegerToString(memoryMap->RegionType));
	CLI::SetCursorPosition(position + VGA_WIDTH * 3); // New line

	PrintString("Memory Attributes: ");
	PrintString(HexToString(memoryMap->ExtendedAttributes));
	CLI::SetCursorPosition(position + VGA_WIDTH * 5); // New line
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