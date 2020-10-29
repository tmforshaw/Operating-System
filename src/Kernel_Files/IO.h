#pragma once
#include "Types.h"

// Master and Slave PIC Chips
#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

// Initialise control word 1
#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

void outb(uint_16 port, uint_8 val);
uint_8 inb(uint_16 port);
void RemapPIC();