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
		char operator[](uint_16 index) const;

		uint_16 Length() const;
		char CharAt(uint_16 index) const;

		const char* GetStringVal() const;

		void ToLower();
		void ToUpper();

		static Type::String ToLower(Type::String str);
		static const char* ToLower(const char* str);

		static Type::String ToUpper(Type::String str);
		static const char* ToUpper(const char* str);

		// Boolean Operators

		bool operator==(const String& other) const;
		bool operator!=(const String& other) const;
	};

} // Namespace Type