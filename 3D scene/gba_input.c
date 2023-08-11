#include "gba_input.h"
#include "gba_utility.h"

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