#pragma once
#include "Types.h"

struct MemorySegmentHeader
{
	uint_64 MemoryLength;
	MemorySegmentHeader* NextSegment;
	MemorySegmentHeader* PreviousSegment;
	MemorySegmentHeader* NextFreeSegment;
	MemorySegmentHeader* PreviousFreeSegment;
	bool Free;
};

void InitialiseHeap(uint_64 heapAddress, uint_64 heapLength);
void* malloc(uint_64 size);
void free(void* address);
void* calloc(uint_64 size);