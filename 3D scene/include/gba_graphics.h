#ifndef GBA_GRAPHICS_H
#define GBA_GRAPHICS_H

#include "gba_memory.h"
#include <math.h>
#include <stdint.h>
#define NUM_OBJECTS 1
// GBA screen is a 240x160 (3:2) 15-bit color LCD display. Here we have constants to represent that.
#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 160

TransformedObject objects[NUM_OBJECTS];

// struct to hold the transformed and projected 2D coordinates of an object.
typedef struct {
    int x;
    int y;
} TransformedObject; 

#endif