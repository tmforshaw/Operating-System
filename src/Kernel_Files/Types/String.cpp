#include "String.hpp"
#include "Types.hpp"

class Type::String
{
private:
	char* str_val = ""; // Address of string value

public:
	// Constructors

	String() {}

	String(const char* str)
	{
		*this = str;
	}

	String(const Type::String str)
	{
		*this = str;
	}

	~String() {}

	// Assignment Operators

	void operator=(const Type::String& other) // Change str_val
	{
		for (uint_32 i = 0; i < other.Length(); i++)
			*(this->str_val + i) = other.CharAt(i);

		*(this->str_val + other.Length()) = 0; // Null-terminate
	}

	void operator=(const char*& other) // Change str_val
	{
		// Calculate length
		uint_32 length = 0;
		while (str_val[length] != 0) length++; // Null-terminated (Don't count null char)

		for (uint_32 i = 0; i < length; i++)
			*(this->str_val + i) = other[i];

		*(this->str_val + length) = 0; // Null-terminate
	}

	char& operator[](uint_32 index)
	{
		if (index >= 0 && index < this->Length()) // Within bounds
			return str_val[index];
		else
			// Invalid Index
			return; // Return NULL
	}

	uint_32 Length() const
	{
		uint_32 length = 0;
		while (str_val[length] != 0) length++; // Null-terminated (Don't count null char)

		return length;
	}

	char CharAt(uint_32 index) const
	{
		if (index >= 0 && index < this->Length()) // Within bounds
			return str_val[index];
		else
			// Invalid Index
			return; // Return NULL
	}
};

Type::String str = "Hello World";