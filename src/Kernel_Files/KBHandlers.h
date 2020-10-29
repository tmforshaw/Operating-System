#pragma once
#include "Types.h"

extern bool LShiftPressed;
extern bool RShiftPressed;
extern uint_8 LastScanCode;

void StandardKeyboardHandler(uint_8 scanCode, uint_8 chr);
void KeyboardHandler0xE0(uint_8 scanCode);
void KeyboardHandler(uint_8 scanCode, uint_8 chr);