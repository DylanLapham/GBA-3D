/*
    Rough memory layout for the GBA:
	0x00000000 - 0x00003FFF - 16 KB System ROM (executable, but not readable)
	0x02000000 - 0x02030000 - 256 KB EWRAM (general purpose RAM external to the CPU)
	0x03000000 - 0x03007FFF - 32 KB IWRAM (general purpose RAM internal to the CPU)
	0x04000000 - 0x040003FF - I/O Registers
	0x05000000 - 0x050003FF - 1 KB Colour Palette RAM
	0x06000000 - 0x06017FFF - 96 KB VRAM (Video RAM)
	0x07000000 - 0x070003FF - 1 KB OAM RAM (Object Attribute Memory — discussed later)
	0x08000000 - 0x???????? - Game Pak ROM (0 to 32 MB)
	0x0E000000 - 0x???????? - Game Pak RAM
*/

#ifndef GBA_MEMORY_H
#define GBA_MEMORY_H
#include <stdint.h>

// memory addresses for the following hardware components: I/O
#define MEM_IO   0x04000000
#define MEM_PAL  0x05000000
#define MEM_VRAM 0x06000000
#define MEM_OAM  0x07000000

// register access for display and key input. 
#define REG_DISPLAY        (*((volatile uint32 *)(MEM_IO)))
#define REG_DISPLAY_VCOUNT (*((volatile uint32 *)(MEM_IO + 0x0006)))
#define REG_KEY_INPUT      (*((volatile uint32 *)(MEM_IO + 0x0130)))

#endif