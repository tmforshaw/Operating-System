#include "Memory.h"

void memset(void* start, uint_64 value, uint_64 size)
{
	if (size <= 8) // Have to do it byte by byte
	{
		uint_8* valPtr = (uint_8*)&value;
		for (uint_8* ptr = (uint_8*)start; ptr < (uint_8*)((uint_64)start + size); ptr++)
		{
			*ptr = *valPtr;
			valPtr++;
		}

		return;
	}

	// Calculate number of 64-Bit OPs that can be done
	uint_64 proceedingBytes = size % 8;		  // How many bytes are after number of bytes possible with 64-Bit OPs
	uint_64 newSize = size - proceedingBytes; // How many 64-Bit OPs can be done

	// Do 64-Bit OPs
	for (uint_64* ptr = (uint_64*)start; ptr < (uint_64*)((uint_64)start + newSize); ptr++)
		*ptr = value;

	// Do the final 8-Bit OPs
	uint_8* valPtr = (uint_8*)&value;
	for (uint_8* ptr = (uint_8*)((uint_64)start + newSize); ptr < (uint_8*)((uint_64)start + size); ptr++)
	{
		*ptr = *valPtr;
		valPtr++;
	}
}

void memcpy(void* destination, void* source, uint_64 size)
{
	if (size <= 8) // Have to do it byte by byte
	{
		uint_8* valPtr = (uint_8*)source;
		for (uint_8* ptr = (uint_8*)destination; ptr < (uint_8*)((uint_64)destination + size); ptr++)
		{
			*ptr = *valPtr;
			valPtr++;
		}

		return;
	}

	// Calculate number of 64-Bit OPs that can be done
	uint_64 proceedingBytes = size % 8;		  // How many bytes are after number of bytes possible with 64-Bit OPs
	uint_64 newSize = size - proceedingBytes; // How many 64-Bit OPs can be done
	uint_64* srcPtr = (uint_64*)source;

	// Do 64-Bit OPs
	for (uint_64* destPtr = (uint_64*)destination; destPtr < (uint_64*)((uint_64)destination + newSize); destPtr++)
	{
		*destPtr = *srcPtr;
		srcPtr++;
	}

	// Do the final 8-Bit OPs
	uint_8* srcPtr8 = (uint_8*)((uint_64)source + newSize);
	for (uint_8* destPtr8 = (uint_8*)((uint_64)destination + newSize); destPtr8 < (uint_8*)((uint_64)destination + size); destPtr8++)
	{
		*destPtr8 = *srcPtr8;
		srcPtr8++;
	}
}