
/************************************************************************************

File: 			planets.c

Description:	A complete OpenGL program that draws a solar system

Author:			Surya Kashyap

*************************************************************************************/

//include libary header files
#include <freeglut.h>

//window dimensions
GLint windowHeight = 600;
GLint windowWidth = 600;



/************************************************************************


Function:		initializeGL


Description:	Initializes the OpenGL rendering context for display.


*************************************************************************/
void initializeGL()
{
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

	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

/************************************************************************


Function:		myDisplay


Description:	The display function


*************************************************************************/

void myDisplay()
{
	// clear the screen 
	glClear(GL_COLOR_BUFFER_BIT);

	//initialize quad
	GLUquadric *quad;
	quad = gluNewQuadric();

	// draw the sun
	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(0.0, 0.3, 0.0);
	gluSphere(quad, 0.2, 100, 20);
	glLoadIdentity();
	
	// planet
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(-0.6, 0.5,0.0);
	gluSphere(quad, 0.05, 100, 20);



	// planet
	glLoadIdentity();
	glColor3f(1.0, 0.5, 0.0);
	glTranslatef(-0.6, 0.53, 1.0);
	gluSphere(quad, 0.04, 100, 20);

	// planet
	glLoadIdentity();
	glColor3f(0.5, 1.0, 0.5);
	glTranslatef(0.4, -0.2, 0.0);
	gluSphere(quad, 0.03, 100, 20);

	// planet
	glLoadIdentity();
	glColor3f(0.0, 1.0, 0.0);
	glTranslatef(-0.3, -0.1, 0.0);
	gluSphere(quad, 0.05, 100, 20);

	// planet
	glLoadIdentity();
	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(0.5, 0.5, 0.0);
	gluSphere(quad, 0.065, 100, 20);

	// planet
	glLoadIdentity();
	glColor3f(1.0, 0.0, 0.5);
	glTranslatef(0.6, 0.5, 0.0);
	gluSphere(quad, 0.06, 100, 20);

	// switch to the other buffer
	glutSwapBuffers();
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

	// initialize the rendering context
	initializeGL();

	// go into perpetual loop
	glutMainLoop();

}