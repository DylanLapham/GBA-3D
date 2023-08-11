#include "gba_utility.h"

// we need a 15-bit RGB color and we can use this function to form it with 5-bit components.
static inline rgb15 RGB15(int r, int g, int b)
{
	return r | (g << 5) | (b << 10);
}

// 'clamp' a value within a needed range. We have a value and its minimum and max range.
// if the value given falls below the minimum, set it to the minimum, if it's above the max, set it to the max, or leave it unchanged.
static inline int clamp(int value, int min, int max)
{
	return (value < min ? min : (value > max ? max : value));
}