
/************************************************************************************

File: 			planets.c

Description:	A complete OpenGL program that draws a solar system

Author:			Surya Kashyap

*************************************************************************************/

//include libary header files
#include <freeglut.h>
#include <math.h>

//window dimensions
GLint windowHeight = 800;
GLint windowWidth = 800;

const float PI = 3.141592653;

GLfloat P1[3] = { -0.6, 0.5, -1.0 };
GLfloat P2[3] = { -0.6, 0.53, -1.0 };
GLfloat P3[3] = { 0.4, -0.2, -1.0 };
GLfloat P4[3] = { 0.43, -0.25, -1.0 };
GLfloat P5[3] = { 0.5, 0.5, -1.0 };
GLfloat P6[3] = { 0.6, 0.3, -1.0 };

float theta = 0;

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

	glOrtho(-1.0, 1.0, -1.0, 1.0, 0.1, 10.0);
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
	glLoadIdentity();
	// draw the sun
	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(0.0, 0.5, 0.0);
	gluSphere(quad, 0.3, 100, 20);
	glLoadIdentity();
	
	// planet
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(P1[0], P1[1],P1[2]);
	gluSphere(quad, 0.05, 100, 20);

	// moon
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(P1[0]+0.06, P1[1]-0.02, P1[2]);
	gluSphere(quad, 0.01, 100, 20);

	// planet
	glLoadIdentity();
	glColor3f(1.0, 0.5, 0.0);
	glTranslatef(P2[0], P2[1], P2[2]);
	gluSphere(quad, 0.04, 100, 20);

	// planet
	glLoadIdentity();
	glColor3f(0.5, 1.0, 0.5);
	glTranslatef(P3[0], P3[1], P3[2]);
	gluSphere(quad, 0.03, 100, 20);

	// moon
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(P3[0] + 0.06, P3[1]+0.04, P3[2]);
	gluSphere(quad, 0.01, 100, 20);

	// planet
	glLoadIdentity();
	glColor3f(0.0, 1.0, 0.0);
	glTranslatef(P4[0], P4[1], P4[2]);
	gluSphere(quad, 0.05, 100, 20);

	// moon
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(P4[0] + 0.03, P4[1]-0.02, P4[2]);
	gluSphere(quad, 0.01, 100, 20);

	// planet
	glLoadIdentity();
	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(P5[0], P5[1], P5[2]);
	gluSphere(quad, 0.065, 100, 20);

	// moon
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(P5[0]+0.04, P5[1]-0.06, P5[2]);
	gluSphere(quad, 0.01, 100, 20);

	// planet
	glLoadIdentity();
	glColor3f(1.0, 0.0, 0.5);
	glTranslatef(P6[0], P6[1], P6[2]);
	gluSphere(quad, 0.06, 100, 20);


	// switch to the other buffer
	glutSwapBuffers();
}

/************************************************************************


Function:		revolvePlanet


Description:	Moves given planet around the sun


*************************************************************************/
void revolvePlanet(double xRadius, double yRadius, GLfloat planet[3], double yPos, double speed)
{
	planet[0] = xRadius * cos(theta * speed) + 0.0;
	planet[1] = yRadius  * sin(theta * speed) + yPos ;
	planet[2] = -1.0;
}

void myIdle()
{	
	theta += 0.001; 
	if (theta < 2 * PI)
	{
		revolvePlanet(0.5, 0.05, P1, 0.5, 2.2);
		revolvePlanet(0.7, 0.05, P2, 0.6, 5.5);
		revolvePlanet(0.3, 0.07, P3, 0.1, 3.0);
		revolvePlanet(0.4, 0.03, P4, 0.2, 2.15);
		revolvePlanet(0.8, 0.06, P5, 0.5, 1.5);
		revolvePlanet(0.4, 0.04, P6, 0.7, 2.6);
	}
	else
	{
		theta = 0.0;
	}

	glutPostRedisplay();
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

	// initialize the rendering context
	initializeGL();

	// go into perpetual loop
	glutMainLoop();

}