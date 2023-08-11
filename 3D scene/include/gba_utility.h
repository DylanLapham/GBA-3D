#ifndef GBA_UTILITY_H
#define GBA_UTILITY_H
#include <stdint.h>

// object attributes.
#define OBJECT_ATTR0_Y_MASK 0x0FF
#define OBJECT_ATTR1_X_MASK 0x1FF

// some type definitions for an unsigned char and others.
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef uint16 rgb15;

rgb15 RGB15(int r, int g, int b);
int clamp(int value, int min, int max);

#endif