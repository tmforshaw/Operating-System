#include "../Memory/Heap.hpp"
#include "String.hpp"

class cus::string
{
private:
	char* str_val;

	// Getters
	char GetChar(uint_32 index) { return str_val[index]; }

	void operator=(const cus::string& other) // Change str_val
	{
		// this->str_val = other->str_val;
	}
};
