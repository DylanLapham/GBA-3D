# GBA-3D
A basic 3D environment on the GBA.

This scene was rendered with my knowledge of game graphics and the C language. The scene was compiled using msys and the devkitpro library. The process is as follows:

arm-none-eabi-gcc -c main.c -mthumb-interwork -mthumb -O2 -o main.o
arm-none-eabi-gcc main.o -mthumb-interwork -mthumb -specs=gba.specs -o main.elf
arm-none-eabi-objcopy -v -O binary main.elf main.gba
gbafix main.gba

This cross compiles the C code into instructions on the GBA and links it into an executable. I ran this on the emulator mGBA.
