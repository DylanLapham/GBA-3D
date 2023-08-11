# GBA-3D
**Tools used**
1. A CLI terminal for windows such as msys. Link: https://www.msys2.org/
2. A set of homebrew toolchains for the GBA, used for compiling and linking a C program into a GBA game. The go-to resource and best option is devkitPro. Link: https://devkitpro.org/
3. A GBA emulator.
**About this project**
I have always loved and dreamed to work with embedded systems. More specifically, I am fascinated with the potential of some of the most well-known embedded systems worldwide. I have a major interest in computer graphics, it was due to this that I discovered the potential of handheld gaming devices. This market is an incredibly intriguing endeavour for someone who wants to experience the potential of embedded systems and practice their existing knowledge of several other fields. My research led me to start programming on Nintendo's 2001 32-bit handheld game console, the Game Boy Advance (GBA). My goal through this project is to learn many of the fundamentals and practices that come with programming on an embedded system, such as more intricate processes with the C language, low-level hardware and software interaction, and most importantly, efficiency with respect to the idea of interacting with low-level hardware. My journey has been equal parts enlightening and testing.
**Relevance to my line of work**
I initially became aware of the potential of the GBA upon realizing the plethora of examples of GBA titles that are wildly complex in nature. There are many GBA games that utilize 3D environments and ray-casting technology. The power to interface with low-level hardware and create something that seems so much larger than life for a small device is endlessly fascinating to me. I did own a GBA when I was younger and I remember being amazed at the technology at hand as well as the potential to play on a device of that magnitude.

With respect to embedded systems programming, programming on the GBA is an interesting and somewhat difficult undertaking when compared to its contemporaries. The device is just old enough that it struggles to render as well as keep pace with many enviornments, yet it is new enough that the amount of documentation the it is seemingly thin. Moreover, the use of many embedded systems keywords is vital to programming on the system. My journey has taught me the importance of certain segments of code being easily cached in registers and ruining the behavior. I have learned the implementation of the keyword 'volatile' which tells the processor that a particular variable may change at any time, therefore, extra precautions should be taken to ensure that the variable is read from memory and not cached. I use this several times in my project whilst working with the GBAs video RAM. A more in-depth explanation is given in the gba_graphics.h file. Furthermore, I also learned the importance of the directive #pragma and its uses in machine specific compiler directives.
****

This scene was rendered with my knowledge of game graphics and the C language. The scene was compiled using msys and the devkitpro library. The process is as follows:

./arm-none-eabi-gcc -c main.c -mthumb-interwork -mthumb -O2 -o main.o
./arm-none-eabi-gcc main.o -mthumb-interwork -mthumb -specs=gba.specs -o main.elf
./arm-none-eabi-objcopy -v -O binary main.elf main.gba
./gbafix main.gba

This cross compiles the C code into instructions on the GBA and links it into an executable. I ran this on the emulator mGBA. 
