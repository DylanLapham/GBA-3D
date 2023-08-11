#include "gba_graphics.h"
#include "gba_utility.h"
#include "gba_dma.h"

typedef struct obj_attrs_3d {
	// X, Y, and Z coordinates given in a 3D plane.
	int x;
	int y;
	int z;
} obj_attrs_3d;

// object attributes
#pragma pack(push, 4)
typedef struct obj_attrs {
	uint16 Ycord;       // Y coordinate and object mode.
	uint16 Xcord;       // X coordinate and object size.
	uint16 tileIndex;   // this will suit the tile index and object shape.
	uint16 alignment;   // ensures proper alignment
} obj_attrs;

// 4-bit per pixel tiles as well as tile blocks.
// the idea here is that 4-bit graphics means each pixel can have 16 color variations. 2^4 = 16
// I could make this more robust by going with 8-bit or even 16-bit but I know the limit of what I should stick to in this setting by myself.
typedef uint32    tile_4bpp[8];
typedef tile_4bpp tile_block[512];

/* 
	responsible for clearing the screen so we can start 'clean' before rendering the next frame. This should be called before rendering.
	the GBA's screen resolution is 240x160 pixels, and each pixel can have a 15-bit color (RGB555 format), meaning it can represent 32,768 different colors. 
	the color of each pixel is stored in the Video RAM (VRAM), which is the memory region starting from address 0x06000000.
	NOTE: this is fairly inefficient as far as clearing the screen goes. There are things to research for more complex 3D enviornments, such as double buffering,
	adaptive tile refresh, etc. However, this is fine to start.
*/ 
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
	uint16 clearColor = RGB15(0, 0, 0);

	// total pixels = 240 * 160
	int totalPixels = SCREEN_WIDTH * SCREEN_HEIGHT;

	// set each pixel to the relevant color.
	for(i = 0; i < totalPixels; i++)
	{
		vram[i] = clearColor;
	}
}

/*
	this is the crux of the program. There is a lot to unpack here in terms of the 'under the hood' explanation. First, some helper functions below:
	essentially, the goal here is to transform these coordinates relative to the position of the camera and its orientation. This is essentially an illusion, but is necessary
 	to acheive the graphics needed with the limitations of the GBA. Here is a helpful summary of the operations themselves.

	Translation: Move the entire 3D world so that the camera is at the origin (0, 0, 0). This step is necessary to simplify subsequent transformations.
	Rotation: Apply the camera's yaw, pitch, and roll angles to rotate the 3D world around the camera's position at the origin.
	View Transformation: Move the 3D world back to its original position so that it appears as if the camera is at the specified (x, y, z) position.

	Yaw – rotation along the Z axis
	Pitch -rotation along the Y axis
	Roll – rotation along the X axis
*/

// number of objects.
TransformedObject objects[NUM_OBJECTS];
void renderScene()
{
	for (int i = 0; i < NUM_OBJECTS; i++) 
	{
        renderObject(objects[i].x, objects[i].y);
    }

}

void updateDisplay()
{
	while(REG_DISPLAY_VCOUNT >= 160);
	while(REG_DISPLAY_VCOUNT < 160);

	volatile uint16* vram = (volatile uint16*)MEM_VRAM;
    volatile uint16* displayBuffer = (volatile uint16*)0x06000000;
    dma3_cpy(displayBuffer, vram, SCREEN_WIDTH * SCREEN_HEIGHT / 2, DMA_16NOW);
}

// rotate Y: yaw.
void rotateY(float *x, float *z, float yaw)
{
	float cosYaw = cosf(yaw);
	float sinYaw = sinf(yaw);
	float tempX = *x;
	*x = tempX * cosYaw - *z * sinYaw;
	*z = tempX * sinYaw + *z * cosYaw;
}

// rotate X: pitch.
void rotateX(float *y, float *z, float pitch)
{
	float cosPitch = cosf(pitch);
	float sinPitch = sinf(pitch);
	float tempY = *y;
	*y = tempY * cosPitch - *z * sinPitch;
	*z = tempY * sinPitch + *z * cosPitch;
}

// translate, rotate, and transform.
void cameraTransform(float *x, float *y, float *z, float cameraX, float cameraY, float cameraZ, float yaw, float pitch, float roll) 
{
	// translation: move to origin.
	*x -= cameraX;
	*y -= cameraY;
	*z -= cameraZ;

	// rotation: apply yaw, pitch, and roll.
	rotateY(x, z, yaw);
	rotateX(y, z, pitch);

	// view transformation: move the 3D world back to its original position so that it appears as if the camera is at the specified (x, y, z) position.
	*x += cameraX;
	*y += cameraY;
	*z += cameraZ;
}

// 3D -> 2D
void perspectiveProjection(float *x, float *y, float *z, float cameraX, float cameraY, float cameraZ)
{
	// focal length: distance between camera and screen. Room for experimentation.
	float focalLength = 160.0f;

	// calculating projected x and y coordinates based on focal length, or camera position.
	*x = cameraX + (*x - cameraX) * focalLength / (*z - cameraZ);
    *y = cameraY + (*y - cameraY) * focalLength / (*z - cameraZ);

	// in the GBA's coordinate system, the top-left corner is (0,0) in terms of a plane. We need to adjust the y-coordinate accordingly.
    *y = SCREEN_HEIGHT - *y;
}

// setting the objects position is a matter of using its attributes.
static inline void set_object_position(volatile obj_attrs_3d *object, int x, int y, int z)
{
	// here, we set the X, Y, and Z coordiantes of the plane of the 3D object itself.
	object->x = x;
	object->y = y;
	object->z = z;
}

// we can use this to render various objects of different complexities. Starting simple with a rectangle perhaps.
void renderObject(int x, int y)
{
	int dx, dy;
	// 16 x 16
	int width = 16; int height = 16;

	// set color: RGB555
	uint16 color = RGB15(31, 0, 0);

	// we need one more pointer to the VRAM region
	volatile uint16* vram = (volatile uint16*)MEM_VRAM;

	// draw: the hard way (kind of) this will do for now.
	for(dy = 0; dy < height; dy++)
	{
		for(int dx = 0; dx < width; dx++)
		{
			vram[(y + dy) * SCREEN_WIDTH + (x + dx)] = color;
		}
	}
}
