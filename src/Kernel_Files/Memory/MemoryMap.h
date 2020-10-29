#pragma once
#include "../.Kernel/Types.h"

struct MemoryMapEntry
{
	uint_64 BaseAddress;
	uint_64 RegionLength;
	uint_32 RegionType;
	uint_32 ExtendedAttributes;
};

extern uint_32 MemoryRegionCount;
extern uint_8 UsableMemoryRegionCount;

void PrintMemoryMap(MemoryMapEntry* memoryMap, uint_16 position);
MemoryMapEntry** GetUsableMemoryRegions();