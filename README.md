# GBA-3D
**Tools used**
1. A CLI terminal for Windows such as msys. Link: https://www.msys2.org/
2. A set of homebrew toolchains for the GBA, used for compiling and linking a C program into a GBA game. The go-to resource and best option is devkitPro. Link: https://devkitpro.org/
3. A GBA emulator.

**About this project**
I have always loved and dreamed to work with embedded systems. More specifically, I am fascinated with the potential of some of the most well-known embedded systems worldwide. I have a major interest in computer graphics, it was due to this that I discovered the potential of handheld gaming devices. This market is an incredibly intriguing endeavor for someone who wants to experience the potential of embedded systems and practice their existing knowledge of several other fields. My research led me to start programming on Nintendo's 2001 32-bit handheld game console, the Game Boy Advance (GBA). My goal through this project is to learn many of the fundamentals and practices that come with programming on an embedded system, such as more intricate processes with the C language, low-level hardware and software interaction, and most importantly, efficiency with respect to the idea of interacting with low-level hardware. My journey has been equal parts enlightening and testing.

**Relevance to my line of work**
I initially became aware of the potential of the GBA upon realizing the plethora of examples of GBA titles that are wildly complex in nature. There are many GBA games that utilize 3D environments and ray-casting technology. The power to interface with low-level hardware and create something that seems so much larger than life for a small device is endlessly fascinating to me. I did own a GBA when I was younger and I remember being amazed at the technology at hand as well as the potential to play on a device of that magnitude.

With respect to embedded systems programming, programming on the GBA is an interesting and somewhat difficult undertaking when compared to its contemporaries. The device is just old enough that it struggles to render as well as keep pace with many environments, yet it is new enough that the amount of public documentation for the device is seemingly thin. Moreover, the use of many embedded systems keywords is vital to programming on the system. My journey has taught me the importance of certain segments of code being easily cached in registers and ruining the behavior. I have learned the implementation of the keyword 'volatile' which tells the processor that a particular variable may change at any time, therefore, extra precautions should be taken to ensure that the variable is read from memory and not cached. I use this several times in my project whilst working with the GBAs video RAM. A more in-depth explanation is given in the gba_graphics.h file. Furthermore, I also learned the importance of the directive #pragma and its uses in machine-specific compiler directives and the importance of storing hex-based variables for use in interfacing the C language with hexadecimal properties. In summary, memory mapping and register access is a fickle friend of the GBA, therefore, these ideas of optimization on an embedded system are incredibly important.

**Direct Memory Access (DMA)**
Direct Memory Access, or DMA, is such an important topic when it comes to optimization on a device like the Game Boy Advance that it deserves its own section of the documentation. The CPU on the device is incredibly fragile, it cannot handle too much stress from direct rendering, therefore, it is important to use DMA. The process of DMA is pretty simple in theory: move data between memory locations, without using the CPU. Eliminating the need for the CPU to execute data instructions in a sequential manner allows the CPU to remain free so that it can perform other tasks. On an embedded system that needs the capacity to render graphics, using DMA to transfer frame buffers, and tiles, among others, drastically improves performance. The general gist of the process in code involves a pointer to the destination and source memory locations, the control flags needed, and the number of 16-bit words that should be transferred. 

**Future of project**
I would like to expand upon this knowledge and create a more robust framework for the GBA. Perhaps, a full game engine is in the cards at some point, and eventually, a simple game with basic particle effects. The process for this would be a much larger undertaking.

**Learning and reflection**
My knowledge of the subtleties when it comes to programming embedded systems has improved exponentially. I am reinvigorated by the challenge and excitement of programming at a low-level interface. Balancing the pros and cons of the hardware and/or software at hand is a different world for someone just starting, however, I feel much more confident in my abilities and my potential to succeed in this world.

Learning not just to write code, but to write code that is robust, accounts for optimization, and utilizes the low-level ideas of programming an embedded system was a challenge at first. I overcame these challenges and learned a lot on the way to creating something very unique.
****

The scene was compiled using msys and the devkitpro library. The process is as follows:

1. ./arm-none-eabi-gcc -c main.c -mthumb-interwork -mthumb -O2 -o main.o
2. ./arm-none-eabi-gcc main.o -mthumb-interwork -mthumb -specs=gba.specs -o main.elf
3. ./arm-none-eabi-objcopy -v -O binary main.elf main.gba
4. ./gbafix main.gba

This cross compiles the C code into instructions on the GBA and links it into an executable. I ran this on the emulator mGBA. 
