/*
	Author: Dylan Lapham
	README: Some variable names as well as functions are taken from open-source manner code from the internet. All necessary rights to those code snippets go to 
	Joe Savage from reinterpretcast.com, published on August 21, 2015.
	Link: https://www.reinterpretcast.com/writing-a-game-boy-advance-game

	The tutorial from the internet was a tutorial to make a simple PONG game. I took some examples from that and re-did the structs and memory access to make it a 
	fully 3D environment.
	
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

// to check for status
typedef enum {
	false = 0,
	true = 1
} bool;

// GBA screen is a 240x160 (3:2) 15-bit color LCD display. Here we have constants to represent that. 
#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 160

// memory addresses for the following hardware components: I/O
#define MEM_IO   0x04000000
#define MEM_PAL  0x05000000
#define MEM_VRAM 0x06000000
#define MEM_OAM  0x07000000

// register access for display and key input. 
#define REG_DISPLAY        (*((volatile uint32 *)(MEM_IO)))
#define REG_DISPLAY_VCOUNT (*((volatile uint32 *)(MEM_IO + 0x0006)))
#define REG_KEY_INPUT      (*((volatile uint32 *)(MEM_IO + 0x0130)))

// controls. These are key codes.
// 0x0040 represents the "UP" arrow key on the Game Boy Advance, and the others represent their respective buttons. 0x03FF does represent any other button.
#define KEY_UP      0x0040
#define KEY_DOWN    0x0080
#define KEY_LEFT    0x0020
#define KEY_RIGHT   0x0010 
#define KEY_ANY     0x03FF

// object attributes.
#define OBJECT_ATTR0_Y_MASK 0x0FF
#define OBJECT_ATTR1_X_MASK 0x1FF

// some type definitions for an unsigned char and others.
typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef uint16 rgb15;

// object attributes
typedef struct obj_attrs 
{
	uint16 Ycord;       // Y coordinate and object mode.
	uint16 Xcord;       // X coordinate and object size.
	uint16 tileIndex;   // this will suit the tile index and object shape.
	uint16 alignment;   // ensures proper alignment

    // some additional properties are needed for a game of this magnitude.
    uint32 isEnemy : 1;             // a single bit to check whether an object is hostile or not. Enemy (1) not enemy (0).
    uint32 isDestructible : 1;      // this is a more ambitious undertaking but I am at least putting it here for a proof of concept.
    uint32 health : 10;             // another thing that I may not get to in the near future, but it is a compelling idea.
    uint32 reserved : 20;           // there has to be a more efficient way to keep track of future needs. TODO: research this. 
} __attribute__((packed, aligned(4))) obj_attrs;

// 4-bit per pixel tiles as well as tile blocks.
// the idea here is that 4-bit graphics means each pixel can have 16 color variations. 2^4 = 16
// I could make this more robust by going with 8-bit or even 16-bit but I know the limit of what I should stick to in this setting by myself.
typedef uint32    tile_4bpp[8];
typedef tile_4bpp tile_block[512];

// object attribute memory, tile memory, and pallete memory. Defined some macros.
#define oam_mem            ((volatile obj_attrs *)MEM_OAM)
#define tile_mem           ((volatile tile_block *)MEM_VRAM)
#define object_palette_mem ((volatile rgb15 *)(MEM_PAL + 0x200))

// we need a 15-bit RGB color and we can use this function to form it with 5-bit components.
static inline rgb15 RGB15(int r, int g, int b)
{
	return r | (g << 5) | (b << 10);
}

typedef struct obj_attrs_3d
{
	// X, Y, and Z coordinates given in a 3D plane.
	int x;
	int y;
	int z;
} obj_attrs_3d;

// setting the objects position is a matter of using its attributes.
static inline void set_object_position(volatile obj_attrs_3d *object, int x, int y, int z)
{
	// here, we set the X, Y, and Z coordiantes of the plane of the 3D object itself.
	object->x = x;
	object->y = y;
	object->z = z;
}

// 'clamp' a value within a needed range. We have a value and its minimum and max range.
// if the value given falls below the minimum, set it to the minimum, if it's above the max, set it to the max, or leave it unchanged.
static inline int clamp(int value, int min, int max)
{
	return (value < min ? min : (value > max ? max : value));
}

// I am going to experiment here with the camera, off of my first thought, I am trying this. These are some random positions.
int cameraX = 100;
int cameraY = 50;
int cameraZ = -10;

void updateInput()
{
	// key states represents the state of the various keys in the form of a bit mask.
	// this is a logical 1 = pressed, 0 = false or not pressed.
	// REG_KEY_INPUT is the complement in order to achieve this. This is necessary for greater intuitiveness.
	uint32 key_states = ~REG_KEY_INPUT & KEY_ANY;

	if(key_states & KEY_UP)
	{
		// decrementing the z-axis view by a number will simulate the movement 'up'
		cameraZ -= 2;
	}

	if(key_states & KEY_DOWN)
	{
		cameraZ += 2;
	}

	if(key_states & KEY_LEFT)
	{
		cameraX -= 2;
	}

	if(key_states & KEY_RIGHT)
	{
		cameraX += 2;
	}
}

// updating the game logic is a matter of checking the camera's position and clamping it in the bounds necessary. The goal is to ensure we don't move through objects.
void updateGameLogic()
{
	uint32 key_states = REG_KEY_INPUT & KEY_ANY;

	if(key_states & KEY_UP)
	{
		cameraZ -= 1;
		cameraZ = clamp(cameraZ, -100, 100);
	}

	if(key_states & KEY_DOWN)
	{
		cameraZ += 1;
		cameraZ = clamp(cameraZ, -100, 100);
	}

	if(key_states & KEY_LEFT)
	{
		cameraX += 1;
	}

	if(key_states & KEY_RIGHT)
	{
		cameraX -= 1;
	}
}

// responsible for clearing the screen so we can start 'clean' before rendering the next frame. This should be called before rendering.
// the GBA's screen resolution is 240x160 pixels, and each pixel can have a 15-bit color (RGB555 format), meaning it can represent 32,768 different colors. 
// the color of each pixel is stored in the Video RAM (VRAM), which is the memory region starting from address 0x06000000.
// NOTE: this is fairly inefficient as far as clearing the screen goes. There are things to research for more complex 3D enviornments, such as double buffering,
// adaptive tile refresh, etc. However, this is fine to start.
void clearScreen()
{
	int i;
	// here, we need a pointer to the VRAM memory region of the GBA.
	// quick explanation of 'volatile': it tells the compiler that a particular variable may change at any time, without any action being taken by the code the compiler finds 
	// nearby. It is used in many memory-mapped hardware registers or memory locations that can be changed by the hardware or other devices. The compiler in turn, assumes that
	// its value can change easily and takes extra precautions to ensure that the variable is read from memory and not cached in registers such that it doesn't optimize
	// writes or reads which can ruin the behavior.
	volatile uint16* vram = (volatile uint16*)MEM_VRAM;
	
	// black is fine here for now to clear the screen with a color.
	uint16 clearColor = 0x0000;

	// total pixels = 240 * 160
	int totalPixels = SCREEN_WIDTH * SCREEN_HEIGHT;

	// set each pixel to the relevant color.
	for(i = 0; i < totalPixels; i++)
	{
		vram[i] = clearColor;
	}
}

/*
	Game loop:

	while(running)
	{
		updateInput();
		updateGameLogic();
		clearScreen();
		renderScene();
		updateDisplay();
	}
*/
