#ifndef GBA_INPUT_H
#define GBA_INPUT_H

#include "gba_memory.h"

// controls. These are key codes.
// 0x0040 represents the "UP" arrow key on the Game Boy Advance, and the others represent their respective buttons. 0x03FF does represent any other button.
#define KEY_UP      0x0040
#define KEY_DOWN    0x0080
#define KEY_LEFT    0x0020
#define KEY_RIGHT   0x0010 
#define KEY_ANY     0x03FF

void updateInput();
void updateGameLogic();

#endif