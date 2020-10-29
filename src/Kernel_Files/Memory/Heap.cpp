#include "Heap.h"
#include "Memory.h"

MemorySegmentHeader* FirstFreeMemorySegment;

void InitialiseHeap(uint_64 heapAddress, uint_64 heapLength)
{
	// Define first Memory segment
	FirstFreeMemorySegment = (MemorySegmentHeader*)heapAddress;
	FirstFreeMemorySegment->MemoryLength = heapLength - sizeof(MemorySegmentHeader);
	FirstFreeMemorySegment->NextFreeSegment = nullptr;
	FirstFreeMemorySegment->PreviousFreeSegment = nullptr;
	FirstFreeMemorySegment->NextFreeSegment = nullptr;
	FirstFreeMemorySegment->PreviousFreeSegment = nullptr;
	FirstFreeMemorySegment->Free = true;
}

void CombineFreeSegments(MemorySegmentHeader* a, MemorySegmentHeader* b)
{
	if (a == nullptr || b == nullptr)
		return;

	if (a < b)
	{
		a->MemoryLength += b->MemoryLength + sizeof(MemorySegmentHeader);
		a->NextSegment = b->NextSegment;
		a->NextFreeSegment = b->NextFreeSegment;
		b->NextSegment->PreviousSegment = a;
		b->NextSegment->PreviousFreeSegment = a;
		b->NextFreeSegment->PreviousFreeSegment = a;
	}
	else
	{
		b->MemoryLength += a->MemoryLength + sizeof(MemorySegmentHeader);
		b->NextSegment = a->NextSegment;
		b->NextFreeSegment = a->NextFreeSegment;
		a->NextSegment->PreviousSegment = b;
		a->NextSegment->PreviousFreeSegment = b;
		a->NextFreeSegment->PreviousFreeSegment = b;
	}
}

void* calloc(uint_64 size)
{
	void* mallocVal = malloc(size);

	memset(mallocVal, 0, size); // Set to zero

	return mallocVal;
}

void* calloc(uint_64 num, uint_64 size)
{
	return calloc(num * size);
}

void* realloc(void* address, uint_64 newSize)
{
	MemorySegmentHeader* OldSegmentHeader;
	AlignedMemorySegmentHeader* AMSH = (AlignedMemorySegmentHeader*)address - 1;

	if (AMSH->IsAligned)
		OldSegmentHeader = (MemorySegmentHeader*)(uint_64)AMSH->MemorySegmentHeaderAddress;
	else
		OldSegmentHeader = (MemorySegmentHeader*)address - 1;

	uint_64 smallerSize = newSize;

	// Set the smaller size
	if (OldSegmentHeader->MemoryLength < newSize)
		smallerSize = OldSegmentHeader->MemoryLength;

	// Create a new memory segment of size newSize
	void* newMem = malloc(newSize);

	memcpy(newMem, address, smallerSize); // Copy the contents of address into new memory segment
	free(address);						  // Free address

	return newMem;
}

void* malloc(uint_64 size)
{
	// Ensure it is aligned to 8 bytes to take advantage of
	// memory alignment
	uint_64 remainder = size & 8;
	size -= remainder;
	if (remainder != 0) size += 8;

	MemorySegmentHeader* CurrentMemorySegment = FirstFreeMemorySegment;

	while (true)
	{
		if (CurrentMemorySegment->MemoryLength >= size)
		{
			if (CurrentMemorySegment->MemoryLength > size + sizeof(MemorySegmentHeader))
			{
				// Add a new Memory Segment after this one
				MemorySegmentHeader* newSegmentHeader = (MemorySegmentHeader*)((uint_64)CurrentMemorySegment + sizeof(MemorySegmentHeader) + size);

				newSegmentHeader->Free = true;
				newSegmentHeader->MemoryLength = ((uint_64)CurrentMemorySegment->MemoryLength - sizeof(MemorySegmentHeader) + size);
				newSegmentHeader->NextFreeSegment = CurrentMemorySegment->NextFreeSegment;
				newSegmentHeader->NextSegment = CurrentMemorySegment->NextSegment;
				newSegmentHeader->PreviousSegment = CurrentMemorySegment;
				newSegmentHeader->PreviousFreeSegment = CurrentMemorySegment->PreviousFreeSegment;

				CurrentMemorySegment->NextFreeSegment = newSegmentHeader;
				CurrentMemorySegment->NextSegment = CurrentMemorySegment;
				CurrentMemorySegment->MemoryLength = size;
			}

			if (CurrentMemorySegment == FirstFreeMemorySegment)
				FirstFreeMemorySegment = CurrentMemorySegment->NextFreeSegment;

			CurrentMemorySegment->Free = false;

			if (CurrentMemorySegment->PreviousFreeSegment != nullptr)
				CurrentMemorySegment->PreviousFreeSegment->NextFreeSegment = CurrentMemorySegment->NextFreeSegment;

			if (CurrentMemorySegment->NextFreeSegment != nullptr)
				CurrentMemorySegment->NextFreeSegment->PreviousFreeSegment = CurrentMemorySegment->PreviousFreeSegment;

			if (CurrentMemorySegment->PreviousSegment != nullptr)
				CurrentMemorySegment->PreviousSegment->NextFreeSegment = CurrentMemorySegment->NextFreeSegment;

			if (CurrentMemorySegment->NextSegment != nullptr)
				CurrentMemorySegment->NextSegment->PreviousSegment = CurrentMemorySegment->PreviousFreeSegment;

			return CurrentMemorySegment + 1; // +1 Adds sizeof(MemorySegmentHeader)
		}

		if (CurrentMemorySegment->NextFreeSegment == nullptr) // No free memory
		{
			return nullptr;
		}

		CurrentMemorySegment = CurrentMemorySegment->NextFreeSegment; // Iterate
	}

	return nullptr; // Should never get here
}

void* aligned_alloc(uint_64 alignment, uint_64 size)
{
	// Align alignment to 8 bytes
	uint_64 alignmentRemainder = alignment % 8;
	alignment -= alignmentRemainder;
	if (alignmentRemainder != 0) alignment += 8;

	// Align size to 8 bytes
	uint_64 sizeRemainder = size % 8;
	size -= sizeRemainder;
	if (sizeRemainder != 0) size += 8;

	// Allow us to move return value around without overflow
	uint_64 fullSize = size + alignment;

	void* mallocVal = malloc(fullSize);
	uint_64 address = (uint_64)mallocVal;

	// Calculate aligned address
	uint_64 remainder = address % alignment;
	address -= remainder;
	if (remainder != 0) // Allows free function to know where Header is
	{
		address += alignment;

		AlignedMemorySegmentHeader* AMSH = (AlignedMemorySegmentHeader*)address - 1;
		AMSH->IsAligned = true;
		AMSH->MemorySegmentHeaderAddress = (uint_64)mallocVal - sizeof(MemorySegmentHeader);
	}

	return (void*)address;
}

void free(void* address)
{
	MemorySegmentHeader* CurrentMemorySegment;
	AlignedMemorySegmentHeader* AMSH = (AlignedMemorySegmentHeader*)address - 1;

	if (AMSH->IsAligned)
		CurrentMemorySegment = (MemorySegmentHeader*)(uint_64)AMSH->MemorySegmentHeaderAddress;
	else
		CurrentMemorySegment = (MemorySegmentHeader*)address - 1;

	CurrentMemorySegment->Free = true;

	if (CurrentMemorySegment < FirstFreeMemorySegment)
		FirstFreeMemorySegment = CurrentMemorySegment;

	if (CurrentMemorySegment->NextFreeSegment != nullptr)
	{
		if (CurrentMemorySegment->NextFreeSegment->PreviousFreeSegment < CurrentMemorySegment)
			CurrentMemorySegment->NextFreeSegment->PreviousFreeSegment = CurrentMemorySegment;
	}

	if (CurrentMemorySegment->PreviousFreeSegment != nullptr)
	{
		if (CurrentMemorySegment->PreviousFreeSegment->NextFreeSegment > CurrentMemorySegment)
			CurrentMemorySegment->PreviousFreeSegment->NextFreeSegment = CurrentMemorySegment;
	}

	if (CurrentMemorySegment->NextSegment != nullptr)
	{
		CurrentMemorySegment->NextSegment->PreviousSegment = CurrentMemorySegment;

		if (CurrentMemorySegment->NextSegment->Free)
			CombineFreeSegments(CurrentMemorySegment, CurrentMemorySegment->NextSegment);
	}

	if (CurrentMemorySegment->PreviousSegment != nullptr)
	{
		CurrentMemorySegment->PreviousSegment->NextSegment = CurrentMemorySegment;

		if (CurrentMemorySegment->PreviousSegment->Free)
			CombineFreeSegments(CurrentMemorySegment, CurrentMemorySegment->PreviousSegment);
	}
}