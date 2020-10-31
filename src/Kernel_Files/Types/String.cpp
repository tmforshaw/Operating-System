#include "../InputOutput/Text/TextPrint.hpp"
#include "../Memory/Heap.hpp"
#include "./String.hpp"

// Constructors

Type::String::String() : str_val((char*)calloc(sizeof(char))) {} // Only the null terminator

Type::String::String(const char* str)
{
	*this = str;
}

Type::String::~String()
{
	// free(str_val); // BROKEN AND I DON'T KNOW WHY
}

// Assignment Operators

void Type::String::operator=(const String other) // Change str_val
{
	free(this->str_val);										 // Free memory
	this->str_val = (char*)calloc(other.Length(), sizeof(char)); // Allocate new memory

	for (uint_16 i = 0; i < other.Length(); i++)
		this->str_val[i] = other.CharAt(i);

	this->str_val[other.Length()] = 0; // Null-terminate
}

void Type::String::operator=(const char* other) // Change str_val
{
	// Calculate length
	uint_16 length = 0;
	while (other[length] != 0) length++; // Null-terminated (Don't count null char)

	for (uint_16 i = 0; i < length; i++)
		this->str_val[i] = other[i];

	this->str_val[length] = 0; // Null-terminate
}

char& Type::String::operator[](uint_16 index)
{
	if (index >= 0 && index < this->Length()) // Within bounds
		return str_val[index];
	else
		// Invalid Index
		return str_val[0]; // Return first element
}

uint_16 Type::String::Length() const
{
	uint_16 length = 0;
	while (this->str_val[length] != 0) length++; // Null-terminated (Don't count null char)

	return length;
}

char Type::String::CharAt(uint_16 index) const
{
	if (index >= 0 && index < this->Length()) // Within bounds
		return this->str_val[index];
	else
		// Invalid Index
		return 0; // Return NULL
}