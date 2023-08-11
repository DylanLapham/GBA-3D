/*
	Author: Dylan Lapham
	README: Some variable names as well as functions are taken from open-source manner code from the internet. All necessary rights to those code snippets go to 
	Joe Savage from reinterpretcast.com, published on August 21, 2015.
	Link: https://www.reinterpretcast.com/writing-a-game-boy-advance-game

	The tutorial from the internet was a tutorial to make a simple PONG game. I took some examples I learned from that and re-did the structs and memory access to make it a 
	fully 3D environment.
*/

#include "gba_input.h"
#include "gba_graphics.h"
#include "gba_dma.h"
#include "gba_utility.h"
#include "gba_memory.h"

int main()
{
	// TODO: I want to add an option to stop running of course, say when 'key select' is pressed. I will experiment.
	// initializing objects is necessary for now.
	objects[0].x = SCREEN_WIDTH / 2;
	objects[0].y = SCREEN_HEIGHT / 2;
	
	// game loop:
	while(1)
	{
		updateInput();
		updateGameLogic();
		clearScreen();
		renderScene();
		updateDisplay();
	}

}