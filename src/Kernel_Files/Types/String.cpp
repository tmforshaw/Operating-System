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
		return str_val[this->Length()]; // Return the null element
}

char Type::String::operator[](uint_16 index) const
{
	if (index >= 0 && index < this->Length()) // Within bounds
		return str_val[index];
	else
		// Invalid Index
		return str_val[this->Length()]; // Return the null element
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

const char* Type::String::GetStringVal() const
{
	return this->str_val;
}

void Type::String::ToLower()
{
	for (uint_16 i = 0; i < this->Length(); i++)
	{
		if ((*this)[i] >= 'A' && (*this)[i] <= 'Z') // Is Uppercase
			(*this)[i] -= 'A';						// Make Lowercase
	}
}

void Type::String::ToUpper()
{
	for (uint_16 i = 0; i < this->Length(); i++)
	{
		if ((*this)[i] >= 'a' && (*this)[i] <= 'z') // Is Uppercase
			(*this)[i] += 'A';						// Make Lowercase
	}
}

Type::String Type::String::ToLower(Type::String str)
{
	Type::String copyStr = str;
	copyStr.ToLower();

	return copyStr;
}

const char* Type::String::ToLower(const char* str)
{
	Type::String copyStr = str;
	copyStr.ToLower();

	return copyStr.GetStringVal();
}

Type::String Type::String::ToUpper(Type::String str)
{
	Type::String copyStr = str;
	copyStr.ToUpper();

	return copyStr;
}

const char* Type::String::ToUpper(const char* str)
{
	Type::String copyStr = str;
	copyStr.ToUpper();

	return copyStr.GetStringVal();
}

// Operators

bool Type::String::operator==(const String& other) const
{
	if (this->Length() != other.Length())
		return false;

	for (uint_16 i = 0; i < this->Length(); i++)
		if ((*this)[i] != other[i])
			return false;

	return true;
}

bool Type::String::operator!=(const String& other) const
{
	return !(*this == other);
}