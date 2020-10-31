#pragma once
#include "./Types.hpp"

namespace Type
{
	class String
	{
	private:
		char* str_val; // Address of string value

	public:
		// Constructors

		String();

		String(const char* str);

		~String();

		// Assignment Operators

		void operator=(const String other);
		void operator=(const char* other);

		char& operator[](uint_16 index);

		uint_16 Length() const;
		char CharAt(uint_16 index) const;
	};

} // Namespace Type