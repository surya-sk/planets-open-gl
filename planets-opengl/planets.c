
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

#pragma warning(disable:4996)

// window dimensions
GLint windowHeight = 600;
GLint windowWidth = 600;

// determines whether stars should be shown
GLint showStars = 0;
// determines whether the sun's corona should be shown
GLint showCorona = 0;

// pi
const float PI = 3.141592653;

// coordinates for the 6 planets
GLfloat P1[3] = { -0.6, 0.0, 0.0 };
GLfloat P2[3] = { -0.6, 0.0, 0.0 };
GLfloat P3[3] = { 0.4, 0.0, 0.0 };
GLfloat P4[3] = { 0.43, 0.0, 0.0 };
GLfloat P5[3] = { 0.5, 0.0, 0.0 };
GLfloat P6[3] = { 0.6, 0.0, 0.0 };


// angle for rotating planets and moons
float theta = 0;

//array with coordinates 
GLfloat arr[21] = { 0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,-0.1,-0.2,-0.3,-0.4,-0.5,-0.6,-0.7,-0.8,-0.9,-1.0 };
// points for the stars
GLfloat starPoints[21];

// colors for stars
GLfloat starColors[2] = { 0.0, 1.0 };

// number of lines for the sun's corona
const int NUM_LINES = 2000;


// file stream to access directory 
FILE *fileStream;
char fileText[100];

// vertices for enterprise
GLfloat entVertices[1201][3];

// faces for enterprise
int entFaces[1989][3];

// camera position
GLfloat cameraPosition[] = { 0.0,0.0, 1.0 };

// difference added at each frame
GLfloat interpDiff = 0.0005;

// determine direction to move the camera in
GLint moveUp, moveDown, moveRight, moveLeft, moveForward, moveBackward = 0;

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


Function:		getRandomFloat


Description:	Generates a random float between min and max


*************************************************************************/
float getRandomFloat(float min, float max)
{
	float scale = rand() / (float)RAND_MAX; /* [0, 1.0] */
	return min + scale * (max - min);      /* [min, max] */
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

	// clear any previous transformations
	glLoadIdentity();

	// set the perspective 
	gluPerspective(45, (float)windowWidth / (float)windowHeight, 0.1, 20);

	// set the shade model
	glShadeModel(GL_SMOOTH);

	// enable smooth line drawing
	glEnable(GL_LINE_SMOOTH);

	// enable depth testing
	glEnable(GL_DEPTH_TEST);

	// load identity matrix
	glLoadIdentity();

	// change to model-view to move objects and camera
	glMatrixMode(GL_MODELVIEW);
	
	//glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);

	// assign random numbers to star points
	for (int i = 0; i < 200; i++)
	{
		starPoints[i] = arr[getRandomNumber(0,20)];
	}
	readEnterpriseFile();

}

/************************************************************************


Function:		readEnterpriseFile


Description:	 Reads the vertices and faces from enterprise.txt file


*************************************************************************/
void readEnterpriseFile()
{
	fileStream = fopen("enterprise.txt", "r");

	GLfloat x, y, z;
	int xFace, yFace, zFace;
	int i = 0, j = 0;
	while (fgets(fileText, sizeof fileText, fileStream))
	{
		if (fileText[0] == 'v')
		{
			if (sscanf(fileText, "v %f %f %f", &x, &y, &z) != 3) { break; }
			//printf("%f %f %f\n", x, y ,z);
			entVertices[i][0] = x;
			entVertices[i][1] = y;
			entVertices[i][2] = z;

			i++;
		}
		else if (fileText[0] == 'f')
		{
			if (sscanf(fileText, "f %d %d %d", &xFace, &yFace, &zFace) != 3) { break; }
			//printf("%d %d %d\n", xFace, yFace ,zFace);
			entFaces[j][0] = xFace;
			entFaces[j][1] = yFace;
			entFaces[j][2] = zFace;

			j++;
		}

	}
	fclose(fileStream);
}

/************************************************************************


Function:		myDisplay


Description:	The display function


*************************************************************************/

void myDisplay()
{
	// clear the screen 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	
	glLoadIdentity();

	// set the camera position
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],
		0, 0, 0,
		0, 1, 0);

	glLoadIdentity();
	//initialize quad
	GLUquadric *quad;
	quad = gluNewQuadric();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	// draw the sun
	glColor3f(1.0, 1.0, -1.0);
	glTranslatef(0.0, 0.0, 0.0);
	gluSphere(quad, 0.2, 100, 20);

	//load identitiy matrix to reset 
	glLoadIdentity();

	//draw the sun's corona if c is pressed
	if (showCorona)
	{
		drawSunCorona();
	}

	glPopMatrix();


	//draw planets and their moons
	drawPlanetsAndMoons(quad);


	glLoadIdentity();

	// draw stars if s is pressed
	if (showStars)
	{
		drawStars();
	}


	glPushMatrix();

	// draw the enterprise
	drawEnterprise();

	glPopMatrix();

	// switch to the other buffer
	glutSwapBuffers();
}

/************************************************************************


Function:		drawEnterprise


Description:	Draws the enterprise from the entVertices and entFaces 


*************************************************************************/
void drawEnterprise()
{
	glLoadIdentity();
	glScalef(0.4, 0.4, 0.4);
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glTranslatef(cameraPosition[0]-0.4, cameraPosition[1]-0.4, cameraPosition[2]);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 1989; i++)
	{
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(entVertices[entFaces[i][0]][0], entVertices[entFaces[i][0]][1], entVertices[entFaces[i][0]][2]);
		glVertex3f(entVertices[entFaces[i][1]][0], entVertices[entFaces[i][1]][1], entVertices[entFaces[i][1]][2]);
		glVertex3f(entVertices[entFaces[i][2]][0], entVertices[entFaces[i][2]][1], entVertices[entFaces[i][2]][2]);
	}
	glEnd();
}


/************************************************************************


Function:		drawSunCorona


Description:	Draws the sun's corona


*************************************************************************/
void drawSunCorona()
{
	//enable blending for transparency
	glEnable(GL_BLEND);

	//set the blending mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTranslatef(0.0, 0.0, 0.0);
	for (int i = 0; i < NUM_LINES; i++)
	{
		glRotatef(getRandomFloat(0, 2 * PI), 0.0, 0.0, 1.0);
		
		glBegin(GL_LINES);

		glColor4f(1.0, 1.0, 0.0, 0.0);
		glVertex2f(-0.25, 0.0);
		glColor4f(1.0, 1.0, -1.0, 1.0);
		glVertex2f(-0.2, 0.0);

		glEnd();
	}
}

/************************************************************************


Function:		drawStars


Description:	Draws 100 stars with randomly changing colors


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
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glRotatef(theta * 10, 0.0, 1.0, 0.0);
	glTranslatef(P1[0], P1[1], P1[2]);
	gluSphere(quad, 0.05, 100, 20);
	glPopMatrix();

	// moon
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glRotatef(theta * 10, 0.0, 1.0, 0.0);
	glTranslatef(P1[0] + 0.06, P1[1] - 0.02, P1[2]);
	gluSphere(quad, 0.01, 100, 20);
	glPopMatrix();

	// planet
	glPushMatrix();
	glColor3f(1.0, 0.5, 0.0);
	glRotatef(theta * 10, 0.0, 1.0, 0.0);
	glTranslatef(P2[0], P2[1], P2[2]);
	gluSphere(quad, 0.04, 100, 20);
	glPopMatrix();

	// planet
	glPushMatrix();
	glColor3f(0.5, 1.0, 0.5);
	glRotatef(theta * 10, 0.0, 1.0, 0.0);
	glTranslatef(P3[0], P3[1], P3[2]);
	gluSphere(quad, 0.03, 100, 20);
	glPopMatrix();

	// moon
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glRotatef(theta * 10, 0.0, 1.0, 0.0);
	glTranslatef(P3[0] + 0.06, P3[1] + 0.04, P3[2]);
	gluSphere(quad, 0.01, 100, 20);
	glPopMatrix();

	// planet
	glPushMatrix();
	glColor3f(0.0, 1.0, 0.0);
	glRotatef(theta * 20, 0.0, 1.0, 0.0);
	glTranslatef(P4[0], P4[1], P4[2]);
	gluSphere(quad, 0.05, 100, 20);
	glPopMatrix();

	// moon
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glRotatef(theta * 20, 0.0, 1.0, 0.0);
	glTranslatef(P4[0] + 0.03, P4[1] - 0.02, P4[2]);
	gluSphere(quad, 0.01, 100, 20);
	glPopMatrix();

	// planet
	glPushMatrix();
	glColor3f(0.0, 0.0, 1.0);
	glRotatef(theta * 40, 0.0, 1.0, 0.0);
	glTranslatef(P5[0], P5[1], P5[2]);
	gluSphere(quad, 0.065, 100, 20);
	glPopMatrix();

	// moon
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glRotatef(theta * 40, 0.0, 1.0, 0.0);
	glTranslatef(P5[0] + 0.04, P5[1] - 0.06, P5[2]);
	gluSphere(quad, 0.01, 100, 20);
	glPopMatrix();

	// planet
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.5);
	glRotatef(theta * 10, 0.0, 1.0, 0.0);
	glTranslatef(P6[0], P6[1], P6[2]);
	gluSphere(quad, 0.06, 100, 20);
	glPopMatrix();
}


/************************************************************************


Function:		myIdle


Description:	 Handles idle functionality 


*************************************************************************/
void myIdle()
{	
	//increase angle
	theta += 0.01; 

	determineMovement();

	// force glut to redraw display
	glutPostRedisplay();
}

/************************************************************************


Function:		determineMovement


Description:	 Determines movement based on key presses


*************************************************************************/
void determineMovement()
{
	// map camera movement to keys
	if (moveRight)
	{
		cameraPosition[0] += interpDiff;
	}
	if (moveLeft)
	{
		cameraPosition[0] -= interpDiff;
	}
	if (moveUp)
	{
		cameraPosition[1] += interpDiff;
	}
	if (moveDown)
	{
		cameraPosition[1] -= interpDiff;
	}
	if (moveForward)
	{
		cameraPosition[2] -= interpDiff;
	}
	if (moveBackward)
	{
		cameraPosition[2] += interpDiff;
	}
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

	//show or hide corona if 'c' is pressed
	case('c'):
		if (showCorona)
		{
			showCorona = 0;
		}
		else
		{
			showCorona = 1;
		}
		break;
	}
}


/************************************************************************


Function:		specialKeys


Description:	Handles special key press functionality


*************************************************************************/
void specialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		moveRight = 1;
		break;
	case GLUT_KEY_LEFT:
		moveLeft = 1;
		break;
	case GLUT_KEY_UP:
		moveUp = 1;
		break;
	case GLUT_KEY_DOWN:
		moveDown = 1;
		break;
	case GLUT_KEY_PAGE_UP:
		moveForward = 1;
		break;
	case GLUT_KEY_PAGE_DOWN:
		moveBackward = 1;
		break;
	}

}

/************************************************************************


Function:		myKeyUp


Description:	Handles key release functionality


*************************************************************************/
void myKeyUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		moveRight = 0;
		break;
	case GLUT_KEY_LEFT:
		moveLeft = 0;
		break;
	case GLUT_KEY_UP:
		moveUp = 0;
		break;
	case GLUT_KEY_DOWN:
		moveDown = 0;
		break;
	case GLUT_KEY_PAGE_UP:
		moveForward = 0;
		break;
	case GLUT_KEY_PAGE_DOWN:
		moveBackward = 0;
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

	// when key has been released
	glutSpecialUpFunc(myKeyUp);

	// special keys function
	glutSpecialFunc(specialKeys);

	// initialize the rendering context
	initializeGL();

	// go into perpetual loop
	glutMainLoop();



}