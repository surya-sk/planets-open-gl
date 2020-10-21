
/************************************************************************************

File: 			planets.c

Description:	A complete OpenGL program that draws a solar system

Author:			Surya Kashyap

*************************************************************************************/

//include libary header files
#include <freeglut.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "planets.h"

// window dimensions
GLint windowHeight = 800;
GLint windowWidth = 800;

// determines whether stars should be shown
GLint showStars = 0;

// pi
const float PI = 3.141592653;

// coordinates for the 6 planets
GLfloat P1[3] = { -0.6, 0.5, 0.0 };
GLfloat P2[3] = { -0.6, 0.53, 0.0 };
GLfloat P3[3] = { 0.4, 0.2, 0.0 };
GLfloat P4[3] = { 0.43, 0.6, 0.0 };
GLfloat P5[3] = { 0.5, 0.5, 0.0 };
GLfloat P6[3] = { 0.6, 0.3, 0.0 };


// angle for rotating planets and moons
float theta = 0;

GLfloat arr[21] = { 0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,-0.1,-0.2,-0.3,-0.4,-0.5,-0.6,-0.7,-0.8,-0.9,-1.0 };

GLfloat starPoints[21];

GLfloat starColors[2] = { 0.0, 1.0 };

GLfloat z = 0.0;

/************************************************************************


Function:		getRandomNumber


Description:	Generates a random number from min to max


*************************************************************************/

int getRandomNumber(int min, int max)
{
	int num = (rand() % (max - min + 1) + min);
	return num;
}

/************************************************************************


Function:		initializeGL


Description:	Initializes the OpenGL rendering context for display.


*************************************************************************/
void initializeGL()
{
	srand(time(0));

	// set the background color
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// set default drawing color
	glColor3f(1.0, 1.0, 1.0);

	// set to projection mode 
	glMatrixMode(GL_PROJECTION);

	// load identity matrix
	glLoadIdentity();

	// set the shade model
	glShadeModel(GL_SMOOTH);

	// enable smooth line drawing
	glEnable(GL_LINE_SMOOTH);

	glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);

	// assign random numbers to star points
	for (int i = 0; i < 200; i++)
	{
		starPoints[i] = arr[getRandomNumber(0,20)];
	}


}

/************************************************************************


Function:		myDisplay


Description:	The display function


*************************************************************************/

void myDisplay()
{
	// clear the screen 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//initialize quad
	GLUquadric *quad;
	quad = gluNewQuadric();

	glMatrixMode(GL_MODELVIEW);
	//glPopMatrix();
	glLoadIdentity();
	// draw the sun
	glColor3f(1.0, 1.0, -1.0);
	glTranslatef(0.0, 0.5, z);
	gluSphere(quad, 0.3, 100, 20);
	//glLoadIdentity();
	

	glTranslatef(0.4, 0, 0);
	drawPlanetsAndMoons(quad);

	//glPushMatrix();

	// draw stars if s is pressed
	if (showStars)
	{
		drawStars();
	}
	
	// switch to the other buffer
	glutSwapBuffers();
}

/************************************************************************


Function:		drawStars


Description:	Draws stars with randomly changing colors


*************************************************************************/
void drawStars()
{
	glLoadIdentity();
	glPointSize(1.0);
	// draw stars
	glBegin(GL_POINTS);
	for (int i = 0; i < 200; i++)
	{
		glColor3f(starColors[getRandomNumber(0, 1)], starColors[getRandomNumber(0, 1)], starColors[getRandomNumber(0, 1)]);
		glVertex3f(starPoints[i], starPoints[i++], -1.0);
	}
	glEnd();
}

/************************************************************************


Function:		drawPlanersAndMoons


Description:	Draws 6 planets and 4 moons


*************************************************************************/
void drawPlanetsAndMoons(GLUquadric * quad)
{
	// planet
	glLoadIdentity();
	glColor3f(1.0, 0.0, 0.0);
	glRotatef(theta * 10, 0.0, 1.0, 0.0);
	glTranslatef(P1[0], P1[1], 1.0);
	gluSphere(quad, 0.05, 100, 20);

	// moon
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	glRotatef(theta * 10, 0.0, 1.0, 0.0);
	glTranslatef(P1[0] + 0.06, P1[1] - 0.02, P1[2]);
	gluSphere(quad, 0.01, 100, 20);

	// planet
	glLoadIdentity();
	glColor3f(1.0, 0.5, 0.0);
	glRotatef(theta * 10, 0.0, 1.0, 0.0);
	glTranslatef(P1[0], P2[1], P2[2]);
	gluSphere(quad, 0.04, 100, 20);

	// planet
	glLoadIdentity();
	glColor3f(0.5, 1.0, 0.5);
	glRotatef(theta * 10, 0.0, 1.0, 0.0);
	glTranslatef(P1[0], P3[1], P3[2]);
	gluSphere(quad, 0.03, 100, 20);

	// moon
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	glRotatef(theta * 10, 0.0, 1.0, 0.0);
	glTranslatef(P1[0] + 0.06, P3[1] + 0.04, P3[2]);
	gluSphere(quad, 0.01, 100, 20);

	// planet
	glLoadIdentity();
	glColor3f(0.0, 1.0, 0.0);
	glRotatef(theta * 20, 0.0, 1.0, 0.0);
	glTranslatef(P1[0], P4[1], P4[2]);
	gluSphere(quad, 0.05, 100, 20);

	// moon
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	glRotatef(theta * 20, 0.0, 1.0, 0.0);
	glTranslatef(P1[0] + 0.03, P4[1] - 0.02, P4[2]);
	gluSphere(quad, 0.01, 100, 20);

	// planet
	glLoadIdentity();
	glColor3f(0.0, 0.0, 1.0);
	glRotatef(theta * 40, 0.0, 1.0, 0.0);
	glTranslatef(P5[0], P5[1], P5[2]);
	gluSphere(quad, 0.065, 100, 20);

	// moon
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	glRotatef(theta * 40, 0.0, 1.0, 0.0);
	glTranslatef(P5[0] + 0.04, P5[1] - 0.06, P5[2]);
	gluSphere(quad, 0.01, 100, 20);

	// planet
	glLoadIdentity();
	glColor3f(1.0, 0.0, 0.5);
	glRotatef(theta * 10, 0.0, 1.0, 0.0);
	glTranslatef(P6[0], P6[1], P6[2]);
	gluSphere(quad, 0.06, 100, 20);
}


/************************************************************************


Function:		myIdle


Description:	 Handles idle functionality 


*************************************************************************/
void myIdle()
{	
	//increase angle
	theta += 0.01; 

	// force glut to redraw display
	glutPostRedisplay();
}

/************************************************************************


Function:		myKey


Description:	Handles key press functionality 


*************************************************************************/
void myKey(unsigned char key, int x, int y)
{
	switch (key)
	{
	// show or hide stars if 's' is pressed
	case ('s'):
		if (showStars)
		{
			showStars = 0;
		}
		else
		{
			showStars = 1;
		}
		break;
	}
}


/************************************************************************

Function:		main

Description:	Sets up the openGL rendering context and the windowing

system, then begins the display loop.


*************************************************************************/
void main(int argc, char** argv)
{
	//initialize toolkit
	glutInit(&argc, argv);

	// set display mode
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	// set window size
	glutInitWindowSize(windowWidth, windowHeight);

	// open screen window
	glutCreateWindow("Planets");

	// set the display function
	glutDisplayFunc(myDisplay);

	// idle function
	glutIdleFunc(myIdle);

	// keyboard function
	glutKeyboardFunc(myKey);

	// initialize the rendering context
	initializeGL();

	// go into perpetual loop
	glutMainLoop();

}