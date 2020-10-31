#pragma once

namespace KBSet1
{
	// clang-format off
    const char ScanCodeLookupTable[] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
        '-', '=', 0, 0,
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
        0, 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
        '`', 0, 
        '#', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
        0, '*', 0, ' '
    };
	// clang-format on
} // Namespace KBSet1

// 0x80 < scancode < 0xE0 Is a released key