#include "IDT.h"
#include "IO.h"
#include "KBHandlers.h"
#include "KBScanCodeSet1.h"
#include "TextPrint.h"

extern IDT64 _idt[256];
extern uint_64 isr1;
extern "C" void LoadIDT();

void (*MainKeyboardHandler)(uint_8, uint_8);

void InitialiseIDT()
{
	_idt[1].zero = 0;
	_idt[1].offset_low = (uint_16)((uint_64)&isr1 & 0x000000000000FFFF);
	_idt[1].offset_mid = (uint_16)(((uint_64)&isr1 & 0x00000000FFFF0000) >> 16);
	_idt[1].offset_high = (uint_32)(((uint_64)&isr1 & 0xFFFFFFFF00000000) >> 32);
	_idt[1].ist = 0;
	_idt[1].selector = 0x08;
	_idt[1].types_attr = 0x8E;

	RemapPIC();

	outb(0x21, 0xFD);
	outb(0xA1, 0xFF);
	LoadIDT();
}

extern "C" void isr1_handler() // Handle Interrupt
{
	uint_8 scanCode = inb(0x60);
	uint_8 chr = 0;

	if (scanCode < 0x3A) // Doesn't include a seperate character for when it is released
		chr = KBSet1::ScanCodeLookupTable[scanCode];

	if (MainKeyboardHandler != nullptr) // Not a nullptr
		MainKeyboardHandler(scanCode, chr);

	outb(0x20, 0x20);
	outb(0xA0, 0x20);
}