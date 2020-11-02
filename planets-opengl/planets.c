
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
GLfloat P1[3] = { -0.6, 0.0, 0.5 };
GLfloat P2[3] = { -0.6, 0.0, 0.3 };
GLfloat P3[3] = { 0.4, 0.0, 0.9 };
GLfloat P4[3] = { 0.43, 0.0, 0.2 };
GLfloat P5[3] = { 0.5, 0.0, 0.7 };
GLfloat P6[3] = { 0.6, 0.0, 0.4 };


// angle for rotating planets and moons
float theta = 0;

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

// vertices for voyager
GLfloat voyVertices[4409][3];

// faces for voyager
int voyFaces[6623][3];

// vertices for camel
GLfloat camVertices[2443][3];

// faces for camel
int camFaces[4884][3];

// vertices for klingon
GLfloat kliVertices[1610][3];

// faces for klingon
int kliFaces[3191][3];

// camera position
GLfloat cameraPosition[] = { 0.0,0.0, 1.5 };

// difference added at each frame
GLfloat interpDiff = 0.0005;

// determine direction to move the camera in
GLint moveUp, moveDown, moveRight, moveLeft, moveForward, moveBackward = 0;

// determine which ship to show
GLint showEnt, showVoyager, showKlingon;

// positions for enterprise and camel
GLfloat entXPos, entYPos, entZPos, camXPos, camYPos, camZPos;

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

	// set enterprise to show by default
	showEnt = 1;

	// assign random numbers to star points
	for (int i = 0; i < 700; i++)
	{
		starPoints[i] = getRandomFloat(-2.0,2.0);
	}
	readEnterpriseFile();

	readVoyagerFile();

	readCamelFile();

	readKlingonFile();
}

/************************************************************************


Function:		readKlingonFile


Description:	 Reads the vertices and faces from klingon.txt file


*************************************************************************/
void readKlingonFile()
{
	fileStream = fopen("klingon.txt", "r");

	GLfloat x, y, z;
	int xFace, yFace, zFace;
	int i = 0, j = 0;
	while (fgets(fileText, sizeof fileText, fileStream))
	{
		if (fileText[0] == 'v')
		{
			if (sscanf(fileText, "v %f %f %f", &x, &y, &z) != 3) { break; }
			kliVertices[i][0] = x;
			kliVertices[i][1] = y;
			kliVertices[i][2] = z;
			i++;
		}
		else if (fileText[0] == 'f')
		{
			if (sscanf(fileText, "f %d %d %d", &xFace, &yFace, &zFace) != 3) { break; }
			kliFaces[j][0] = xFace;
			kliFaces[j][1] = yFace;
			kliFaces[j][2] = zFace;
			j++;
		}
	}
}

/************************************************************************


Function:		readCamelFile


Description:	 Reads the vertices and faces from camel.txt file


*************************************************************************/
void readCamelFile()
{
	fileStream = fopen("camel.txt", "r");
	GLfloat x, y, z;
	int xFace, yFace, zFace;
	int i = 0, j = 0;
	while (fgets(fileText, sizeof fileText, fileStream))
	{
		if (fileText[0] == 'v')
		{
			if (sscanf(fileText, "v %f %f %f", &x, &y, &z) != 3) { break; }
			camVertices[i][0] = x;
			camVertices[i][1] = y;
			camVertices[i][2] = z;
			i++;
		}
		else if (fileText[0] == 'f')
		{
			if (sscanf(fileText, "f %d %d %d", &xFace, &yFace, &zFace) != 3) { break; }
			camFaces[j][0] = xFace;
			camFaces[j][1] = yFace;
			camFaces[j][2] = zFace;
			j++;
		}
	}
}

/************************************************************************


Function:		readVoyagerFile


Description:	 Reads the vertices and faces from voyager.txt file


*************************************************************************/
void readVoyagerFile()
{
	fileStream = fopen("voyager.txt", "r");
	GLfloat x, y, z;
	int xFace, yFace, zFace;
	int i = 0, j = 0;
	while (fgets(fileText, sizeof fileText, fileStream))
	{
		if (fileText[0] == 'v')
		{
			if (sscanf(fileText, "v %f %f %f", &x, &y, &z) != 3) { break; }
			//printf("%f %f %f\n", x, y ,z);
			voyVertices[i][0] = x;
			voyVertices[i][1] = y;
			voyVertices[i][2] = z;
			i++;
		}
		else if (fileText[0] == 'f')
		{
			if (sscanf(fileText, "f %d %d %d", &xFace, &yFace, &zFace) != 3) { break; }
			voyFaces[j][0] = xFace;
			voyFaces[j][1] = yFace;
			voyFaces[j][2] = zFace;
			j++;
		}
	}
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


	// set to projection mode 
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	// set the camera position
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],
		cameraPosition[0], cameraPosition[1], cameraPosition[2]-100,
		0, 1, 0);

	//initialize quad
	GLUquadric *quad;
	quad = gluNewQuadric();

	if (showEnt)
	{
		// draw the enterprise and camel
		drawEnterprise();

		drawCamel();
	}

	if (showVoyager)
	{
		// draw the voyager
		drawVoyager();
	}

	if (showKlingon)
	{
		// show the klingon
		drawKlingon();
	}
	


	glTranslatef(-cameraPosition[0], -cameraPosition[1], -cameraPosition[2]);


	glPushMatrix();

	 //draw the sun
	glColor3f(1.0, 1.0, -1.0);
	glTranslatef(0.0, 0.0, 0.0);
	gluSphere(quad, 0.2, 100, 20);

	glPopMatrix();

	glPushMatrix();
	glBegin(GL_LINE_LOOP);
	for (float i = 0; i < 2 * PI; i += 0.01)
	{
		float x = 0.5 * cos(i) + 0.0;
		float y = P1[1];
		float z = 0.5 * sin(i) + P1[2];
		glVertex3f(x, y, z);
	}
	glEnd();
	glPopMatrix();

	//draw planets and their moons
	drawPlanetsAndMoons(quad);


	// draw stars if s is pressed
	if (showStars)
	{
		drawStars();
	}


	//draw the sun's corona if c is pressed
	if (showCorona)
	{
		drawSunCorona();
	}

	// switch to the other buffer
	glutSwapBuffers();
}

/************************************************************************


Function:		drawKlingon


Description:	Draws the klingon from the kliVertices and kliFaces


*************************************************************************/
void drawKlingon()
{
	glPushMatrix();
	glScalef(0.1, 0.1, 0.1);
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glTranslatef(cameraPosition[0] - 0.4, cameraPosition[1] - 0.7, cameraPosition[2] + 2.0);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 3191; i++)
	{
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(kliVertices[kliFaces[i][0]][0], kliVertices[kliFaces[i][0]][1], kliVertices[kliFaces[i][0]][2]);
		glVertex3f(kliVertices[kliFaces[i][1]][0], kliVertices[kliFaces[i][1]][1], kliVertices[kliFaces[i][1]][2]);
		glVertex3f(kliVertices[kliFaces[i][2]][0], kliVertices[kliFaces[i][2]][1], kliVertices[kliFaces[i][2]][2]);
	}
	glEnd();
	glPopMatrix();
}

/************************************************************************


Function:		drawCamel


Description:	Draws the camel from the camVertices and camFaces


*************************************************************************/
void drawCamel()
{
	glPushMatrix();
	glScalef(0.1, 0.1, 0.1);
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glTranslatef(camXPos, camYPos, camZPos);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 4884; i++)
	{
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(camVertices[camFaces[i][0]][0], camVertices[camFaces[i][0]][1], camVertices[camFaces[i][0]][2]);
		glVertex3f(camVertices[camFaces[i][1]][0], camVertices[camFaces[i][1]][1], camVertices[camFaces[i][1]][2]);
		glVertex3f(camVertices[camFaces[i][2]][0], camVertices[camFaces[i][2]][1], camVertices[camFaces[i][2]][2]);
	}
	glEnd();
	glPopMatrix();
}

/************************************************************************


Function:		drawVoyager


Description:	Draws the voyager from the voyVertices and voyFaces


*************************************************************************/
void drawVoyager()
{
	glPushMatrix();
	glScalef(0.4, 0.4, 0.4);
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glTranslatef(cameraPosition[0] + 0.2 , cameraPosition[1] + 0.4 , cameraPosition[2]);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 4409; i++)
	{
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(voyVertices[voyFaces[i][0]][0], voyVertices[voyFaces[i][0]][1], voyVertices[voyFaces[i][0]][2]);
		glVertex3f(voyVertices[voyFaces[i][1]][0], voyVertices[voyFaces[i][1]][1], voyVertices[voyFaces[i][1]][2]);
		glVertex3f(voyVertices[voyFaces[i][2]][0], voyVertices[voyFaces[i][2]][1], voyVertices[voyFaces[i][2]][2]);
	}
	glEnd();
	glPopMatrix();
}


void drawOrbits(float xRadius, float yRadius)
{
	glBegin(GL_LINES);
	for (float i = 0; i < 2 * PI; i += 0.01)
	{
		float x = xRadius * cos(i) + 0.0;
		float y = 0.0;
		float z = yRadius * sin(i) + 0.0;
		glVertex3f(x, y, z);
	}
	glEnd();
}

/************************************************************************


Function:		drawEnterprise


Description:	Draws the enterprise from the entVertices and entFaces 


*************************************************************************/
void drawEnterprise()
{
	glPushMatrix();
	entXPos = cameraPosition[0] - 0.4;
	entYPos = cameraPosition[1] - 0.8;
	entZPos = cameraPosition[2] + 0.2;
	glScalef(0.15, 0.15, 0.15);
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glTranslatef(entXPos, entYPos, entZPos);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 1989; i++)
	{
		glColor3f(1989/1989, 1989 /1989, 1989 /1989);
		glVertex3f(entVertices[entFaces[i][0]][0], entVertices[entFaces[i][0]][1], entVertices[entFaces[i][0]][2]);
		glColor3f(1989 / 1989, 1989 / 1989, 1989 / 1989);
		glVertex3f(entVertices[entFaces[i][1]][0], entVertices[entFaces[i][1]][1], entVertices[entFaces[i][1]][2]);
		glColor3f(1989 / 1989, 1989 / 1989, 1989 / 1989);
		glVertex3f(entVertices[entFaces[i][2]][0], entVertices[entFaces[i][2]][1], entVertices[entFaces[i][2]][2]);
	}
	glEnd();
	glPopMatrix();
	camXPos = entXPos;
	camYPos = entYPos;
	camZPos = entZPos;
	glPushMatrix();
	drawCamel();
	glPopMatrix();
	//glTranslatef(cameraPosition[0] - 0.4, cameraPosition[1] - 0.4, cameraPosition[2]);
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
	glPushMatrix();
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
	glPopMatrix();
}

/************************************************************************


Function:		drawStars


Description:	Draws 100 stars with randomly changing colors


*************************************************************************/
void drawStars()
{
	glPointSize(1.0);
	// draw stars
	glBegin(GL_POINTS);
	for (int i = 0; i < 700; i++)
	{
		glColor3f(starColors[getRandomNumber(0, 1)], starColors[getRandomNumber(0, 1)], starColors[getRandomNumber(0, 1)]);
		glVertex3f(starPoints[i], starPoints[i++], 0.0);
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
	
	//// orbit around the planet
	//glPushMatrix();
	//glColor3f(1.0, 1.0, 1.0);
	//glRotatef(theta * 10, 0.0, 1.0, 0.0);
	//glTranslatef(P1[0], P1[1], P1[2]);
	//drawOrbits(0.05, 0.08);
	//glPopMatrix();



	//// moon
	//glPushMatrix();
	//glColor3f(1.0, 1.0, 1.0);
	//glRotatef(theta * 10, 0.0, 1.0, 0.0);
	//glTranslatef(P1[0] + 0.06, P1[1] - 0.02, P1[2]);
	//gluSphere(quad, 0.01, 100, 20);
	//glPopMatrix();

	//// planet
	//glPushMatrix();
	//glColor3f(1.0, 0.5, 0.0);
	//glRotatef(theta * 10, 0.0, 1.0, 0.0);
	//glTranslatef(P2[0], P2[1], P2[2]);
	//gluSphere(quad, 0.04, 100, 20);
	//glPopMatrix();

	//// planet
	//glPushMatrix();
	//glColor3f(0.5, 1.0, 0.5);
	//glRotatef(theta * 35, 0.0, 1.0, 0.0);
	//glTranslatef(P3[0], P3[1], P3[2]);
	//gluSphere(quad, 0.03, 100, 20);
	//glPopMatrix();

	//// moon
	//glPushMatrix();
	//glColor3f(1.0, 1.0, 1.0);
	//glRotatef(theta * 35, 0.0, 1.0, 0.0);
	//glTranslatef(P3[0] + 0.06, P3[1] + 0.04, P3[2]);
	//gluSphere(quad, 0.01, 100, 20);
	//glPopMatrix();

	//// planet
	//glPushMatrix();
	//glColor3f(0.0, 1.0, 0.0);
	//glRotatef(theta * 20, 0.0, 1.0, 0.0);
	//glTranslatef(P4[0], P4[1], P4[2]);
	//gluSphere(quad, 0.05, 100, 20);
	//glPopMatrix();

	//// moon
	//glPushMatrix();
	//glColor3f(1.0, 1.0, 1.0);
	//glRotatef(theta * 20, 0.0, 1.0, 0.0);
	//glTranslatef(P4[0] + 0.03, P4[1] - 0.02, P4[2]);
	//gluSphere(quad, 0.01, 100, 20);
	//glPopMatrix();

	//// planet
	//glPushMatrix();
	//glColor3f(0.0, 0.0, 1.0);
	//glRotatef(theta * 40, 0.0, 1.0, 0.0);
	//glTranslatef(P5[0], P5[1], P5[2]);
	//gluSphere(quad, 0.065, 100, 20);
	//glPopMatrix();

	//// moon
	//glPushMatrix();
	//glColor3f(1.0, 1.0, 1.0);
	//glRotatef(theta * 40, 0.0, 1.0, 0.0);
	//glTranslatef(P5[0] + 0.04, P5[1] - 0.06, P5[2]);
	//gluSphere(quad, 0.01, 100, 20);
	//glPopMatrix();

	//// planet
	//glPushMatrix();
	//glColor3f(1.0, 0.0, 0.5);
	//glRotatef(theta * 10, 0.0, 1.0, 0.0);
	//glTranslatef(P6[0], P6[1], P6[2]);
	//gluSphere(quad, 0.06, 100, 20);
	//glPopMatrix();
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

	// show the selected vehicle
	case('1'):
		if (showEnt)
		{
			showEnt;
		}
		else
		{
			showEnt = 1;
			showVoyager = showKlingon = 0;
		}
		break;
	case('2'):
		if (showVoyager == 0)
		{
			showVoyager = 1;
			showEnt = showKlingon = 0;
		}
		break;
	case('3'):
		if (showKlingon == 0)
		{
			showKlingon = 1;
			showEnt = showVoyager = 0;
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