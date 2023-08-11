#ifndef GBA_DMA_H
#define GBA_DMA_H
#include "gba_memory.h"

#define DMA_SOURCE_INCREMENT (1 << 23)
#define DMA_DEST_INCREMENT (1 << 21)
#define DMA_START (1 << 31)
// 16-bit data transfer
#define DMA_16NOW (0 << 26)
#define DMA_ENABLE (1 << 31)

// DMA channel 3 control register
#define DMA3CNT *(volatile uint32*)0x040000D4
// DMA channel 3 source and destination addresses
#define DMA3SAD *(volatile uint32*)0x040000D8
#define DMA3DAD *(volatile uint32*)0x040000DC
// DMA channel 3 word count
#define DMA3CNT_L *(volatile uint32*)0x040000D6

void dma3_cpy(volatile uint16_t* dest, volatile uint16_t* src, uint32_t count, uint32_t control);

#endif