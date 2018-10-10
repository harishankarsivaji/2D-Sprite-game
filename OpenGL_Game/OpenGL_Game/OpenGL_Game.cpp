//Harishankar Sivaji
//17250674
// 10 Apr 2018 

#include "stdafx.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>    // Required for sound only (consider OpenAL), 
#include <mmsystem.h>   // Again only required for sound only
#include <freeglut.h>   // GLUT OpenGL Utility Toolkit
#include "Sprite.h"     // Prototype Sprite class

Sprite player;
Sprite background;
Sprite bullet;
Sprite enemy;
Sprite enemybullet;
bool enemy_move = true;

// Score data and method
int score = 0;
char score_text[20];


// Callback function called each time screen is to be redrawn (called by OS not you directly)
static void display(void)
{															// Keep game logic out of display()...
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen and the Depth Buffer

	background.drawSprite();							// Draw background at X,Y
	player.drawSprite();								// Draw player	
	enemy.drawSprite();
	enemybullet.drawSprite();
	bullet.drawSprite();

	sprintf(score_text,"Score: %d",score);              // Display text 
	Sprite::text(830,420,score_text);

	glutSwapBuffers();									// Display buffer just drawn and then switch to draw on the other buffer
}


// Callback function to allow dynamic resizing of runtime display
static void resize(int width, int height)
{
	glViewport(0, 0, width, height);
	Sprite::spriteModeOn(0, 0);  // (0,0) Causes orginal screen size to be reused
}

// Callback to manage standard keys on the keyboard
// In this case 'q' or 'Q' causes program to terminate
static void qwerty_keys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'Q':
	case 'q': glutLeaveMainLoop(); break;  // (Stop!)
	default: break;
	}
}

// Callback for cursor keys on the keyboard
// <- and -> cursors keys cause player to move left and right
static void function_keys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		player.sprite_y_position += 10;
		break;


	case GLUT_KEY_DOWN:
		player.sprite_y_position -= 10;
		break;

	case GLUT_KEY_RIGHT:
		if (bullet.sprite_y_position > 999) {
			bullet.sprite_x_position = player.sprite_x_position +60;
			bullet.sprite_y_position = player.sprite_y_position +30;
			break;
		} 
		
	default: break;
	}
}

// Game loop
// static void idle()    // if using  glutIdleFunc(idle); this line should replace the one below


static void idle(int v)  // Runs between frame updates in doublebuffer mode, game logic goes here
{
	
	bullet.sprite_x_position += 40;
	int random = rand() % 400 + 5;
	

	if (enemy_move) 
	{
		enemy.sprite_x_position -= 15;
		if (enemy.sprite_x_position < 10) 
		{
			score = 0;
			enemy.load("enemy.bmp",1, 1, 1000, random, 80, 80, false);
			enemy_move = false;
			bullet.sprite_x_position = 1000;
		}
	}
	else {
		enemy.sprite_x_position -= 15;
		if (enemy.sprite_x_position <10) 
		{
			enemy.load("enemy.bmp", 1, 1, 1000, random, 80, 80, false);
			score = 0;
			enemy_move = true;
			bullet.sprite_x_position = 1000;
		}
	}
	

	if (enemy.collision(bullet)) 
	{
		enemy.load("enemy.bmp", 1, 1, 1000, random, 80, 80, false);
		score ++;
		bullet.sprite_y_position = 1000;
	}
	enemy.advance_frame(0, 0);

	glutPostRedisplay();      // Send message to ask for screen to be redrawn
	glutTimerFunc(100, idle, 0); // Restart the timer (remove line if using the more common glutIdleFunc(idle) approach)
}

/* Program entry point - starts here */
int main(int argc, char *argv[])
{
	Sprite::spriteModeOn(1024,480);		// Create a screen 640x480 to contain game

											// Start playing background music (uses Windows calls, there are other ways incluing OpenAL)
	PlaySound(L"music.wav", NULL, SND_LOOP | SND_ASYNC);

	// load sprite files (24bit BMP), Frames (X,Y), position (X,Y), size (Width,Height), direction
	// Use sprites with Width and Height divisible by 4, ideally powers of two e.g. 256x128

	player.load("Playership1.bmp", 1, 1, 1, 100, 80, 80, false);
	background.load("background.bmp", 1, 1, 0, 0, 1024, 480, true);
	enemy.load("enemy.bmp", 1, 1, 1000, rand()%400, 80, 80, false);
	bullet.load("Bullet.bmp", 1, 1, 1000, 1000, 15, 15, true);
	

	glutReshapeFunc(resize);			// Called when window is resized (not required)
	glutDisplayFunc(display);			// Callback function to redraw, an event created by glutPostRedisplay()
	glutKeyboardFunc(qwerty_keys);		// QWERTY Keys
	glutSpecialFunc(function_keys);		// Cursor keys etc 
	glutTimerFunc(10, idle, 0);         // Use timer callback for more consistent timing

	glutMainLoop();						// Start main loop running (go!)

	return EXIT_SUCCESS;
}

