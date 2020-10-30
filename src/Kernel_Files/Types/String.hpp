#pragma once
#include "./Types.hpp"

namespace Type
{
	class String
	{
	private:
	public:
		char* str_val; // Address of string value

		// Constructors

		String();

		String(const char* str);

		~String();

		// Assignment Operators

		void operator=(const String other);
		void operator=(const char* other);

		char& operator[](uint_32 index);

		uint_32 Length() const;
		char CharAt(uint_32 index) const;
	};

} // Namespace Type