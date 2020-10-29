#pragma once

#define FG_BLACK 0x00
#define FG_BLUE 0x01
#define FG_GREEN 0x02
#define FG_CYAN 0x03
#define FG_RED 0x04
#define FG_MAGENTA 0x05
#define FG_BROWN 0x06
#define FG_LIGHTGREY 0x07
#define FG_DARKGREY 0x08
#define FG_LIGHTBLUE 0x09
#define FG_LIGHTGREEN 0x0A
#define FG_LIGHTCYAN 0x0B
#define FG_LIGHTRED 0x0C
#define FG_LIGHTMAGENTA 0x0D
#define FG_YELLOW 0x0E
#define FG_WHITE 0x0F

#define BG_BLACK 0x00
#define BG_BLUE 0x10
#define BG_GREEN 0x20
#define BG_CYAN 0x30
#define BG_RED 0x40
#define BG_MAGENTA 0x50
#define BG_BROWN 0x60
#define BG_LIGHTGREY 0x70
#define BG_LIGHTBLACK 0x80
#define BG_LIGHTBLUE 0x90
#define BG_LIGHTGREEN 0xA0
#define BG_LIGHTCYAN 0xB0
#define BG_LIGHTRED 0xC0
#define BG_LIGHTMAGENTA 0xD0
#define BG_YELLOW 0xE0
#define BG_WHITE 0xF0

// Defaults

#define DEFAULT_BG BG_BLUE
#define DEFAULT_FG FG_WHITE
#define DEFAULT_COLOUR (DEFAULT_BG | DEFAULT_FG)