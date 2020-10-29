#include "IO.hpp"

void outb(uint_16 port, uint_8 val)
{
	asm volatile("outb %0, %1"
				 :
				 : "a"(val), "Nd"(port));
}

uint_8 inb(uint_16 port)
{
	uint_8 returnVal;

	asm volatile("inb %1, %0"
				 : "=a"(returnVal)
				 : "Nd"(port));

	return returnVal;
}

void RemapPIC()
{
	uint_8 a1, a2; // Mask values of PIC chips

	a1 = inb(PIC1_DATA);
	a2 = inb(PIC2_DATA);

	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); // Starts initialisation sequence of master PIC chip
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4); // Starts initialisation sequence of slave PIC chip
	outb(PIC1_DATA, 0);
	outb(PIC2_DATA, 8);
	outb(PIC1_DATA, 4);
	outb(PIC2_DATA, 2);
	outb(PIC1_DATA, ICW4_8086);
	outb(PIC2_DATA, ICW4_8086);

	// Restore PICs
	outb(PIC1_DATA, a1);
	outb(PIC2_DATA, a2);
}