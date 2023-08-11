#include "gba_dma.h"
#include "gba_utility.h"

void dma3_cpy(volatile uint16* dest, volatile uint16* src, uint32 count, uint32 control) 
{
    // disable DMA3 first!
    DMA3CNT = 0;

    // set the source and destination addresses.
    DMA3SAD = (uint32)src;
    DMA3DAD = (uint32)dest;

    // et the transfer count. We need half words.
    DMA3CNT_L = count / 2;

    // set the DMA control flags and enable.
    DMA3CNT = control | DMA_START | DMA_ENABLE;
}