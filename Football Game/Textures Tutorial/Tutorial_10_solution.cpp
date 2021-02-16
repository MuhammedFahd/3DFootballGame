//mac osx includes
//#include <iostream>
//#include <stdio.h>
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
//#include <OpenGL/glext.h>
//#include <GLUT/glut.h>

// OpenGL toolkit for PC
#include <math.h>						
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>	//Needed for console output (debugging)
#include <gl/freeglut.h>
#include <iostream>

#ifdef WIN32
#include "gltools.h"
#include <windows.h>		// Must have for Windows platform builds
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
#include <gl\glu.h>			// OpenGL Utilities
#include "glm.h"
#endif
// (c) Philip Trwoga projection tutorial - week 11 -


//foward declare functions
void resetPerspectiveProjection();
void drawAngleWithScaledText(GLfloat scale);
void setOrthographicProjection();
void ChangeSize(int w, int h);
void RenderScene(void);
void TimerFunc(int value);

#define IMAGE1      0
#define IMAGE2      1
#define IMAGE3      2
#define IMAGE4      3
#define TEXTURE_BALL 4    //football map
#define TEXTURE_COUNT 5
GLuint  textures[TEXTURE_COUNT];

const char* textureFiles[TEXTURE_COUNT] = { "pingu.tga", "horse.tga","floor.tga","stone.tga","FootballCompleteMap.tga" };

//they are to do with the image format and size
GLint iWidth, iHeight, iComponents;
GLenum eFormat;
// this is a pointer to memory where the image bytes will be held 
GLbyte* pBytes0;

//camera
GLfloat cameraX = 0.0;
GLfloat cameraY = 100.0;
GLfloat cameraZ = 500.0;

GLint Wwidth;
GLint Wheight;
GLfloat teapotRotation = 0.0;
//used for the framerate
GLint frame = 0, timeTot = 0, timebase = 0;
char frameRate[12];

void drawTexturedQuad(int image);

//bar
GLfloat lineXpos = 20;

//this function pops back to the last projection
void resetPerspectiveProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


//used for drawing text
void displayText(GLfloat x, GLfloat y, GLint r, GLint g, GLint b, const char* string, bool stroke) {
	GLint j = strlen(string);

	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	for (GLint i = 0; i < j; i++)
	{
		if (stroke)
		{
			glutStrokeCharacter(GLUT_STROKE_ROMAN, string[i]);
		}
		else
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
		}

	}
}

char teapotRotationString[12];

void drawAngleWithScaledText(GLfloat scale)
{
	glPushMatrix();

	//draw the text offset from the box
	glTranslatef(500.0, 120.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	sprintf_s(teapotRotationString, "Angle:%3.0f", teapotRotation);

	//flip
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glScalef(scale, scale, scale);
	//if stroke = true then use the tranlate above to move the text
	//if stroke = false then use the first two values of displayText

	displayText(500, 100, 0, 1, 0, teapotRotationString, true);
	glPopMatrix();
}

void drawFrameRate()
{
	//this draws the framerate on the screen - useful for testing animation
	frame++;
	timeTot = glutGet(GLUT_ELAPSED_TIME);
	if (timeTot - timebase > 1000)
	{
		sprintf_s(frameRate, "FPS: %4.2f", frame * 1000.0 / (timeTot - timebase));

		timebase = timeTot;
		frame = 0;
	}

	//set the text to white
	glColor3f(1.0f, 1.0f, 1.0f);

	//this stop the text being affected by the current tranformation by reseting it
	glLoadIdentity();
	displayText(30, 30, 1, 0, 0, frameRate, false);
}


//sets up the orthographics projection for 2D overlay
void setOrthographicProjection() {
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save the previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, Wwidth, 0, Wheight);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -Wheight, 0);
	//set for drawing again
	glMatrixMode(GL_MODELVIEW);
}

// This function does any needed initialization on the rendering
// context.
void SetupRC()
{
	//textures

	GLuint texture;
	// allocate a texture name
	glGenTextures(1, &texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// photoshop is a good converter to targa (TGA)
	//the gltLoadTGA method is found in gltools.cpp and is orignally from the OpenGL SuperBible book
	//there are quite a few ways of loading images
	// Load textures in a for loop
	glGenTextures(TEXTURE_COUNT, textures);
	//this puts the texture into OpenGL texture memory
 //   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); - not defined so probably need to update GLEW - not needed here so ignore
	for (int iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++)
	{
		// Bind to next texture object
		glBindTexture(GL_TEXTURE_2D, textures[iLoop]);

		// Load texture data, set filter and wrap modes
		//note that gltLoadTGA is in the glm.cpp file and not a built-in openGL function
		pBytes0 = gltLoadTGA(textureFiles[iLoop], &iWidth, &iHeight,
			&iComponents, &eFormat);

		glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes0);

		//set up texture parameters

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		//try these too
	   // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
		// glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		free(pBytes0);
	}

	//enable textures
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);	// Hidden surface removal    
	glFrontFace(GL_CCW);// Counter clock-wise polygons face out
 //	glEnable(GL_CULL_FACE);		// Do not calculate inside

//    glCullFace(GL_FRONT_AND_BACK);

// Enable lighting

	//glEnable(GL_LIGHT2);


	// Black blue background clear colour
	glClearColor(0.0f, 0.0f, 0.03f, 1.0f);
}



void ChangeSize(int w, int h)
{
	GLfloat fAspect;
	Wwidth = w;
	Wheight = h;

	// Prevent a divide by zero
	if (h == 0)
		h = 1;
	//need this for changing projection

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Calculate aspect ratio of the window
	fAspect = (GLfloat)w / (GLfloat)h;

	// Set the perspective coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// field of view of 45 degrees, near and far planes 1.0 and 625
	gluPerspective(60.0f, fAspect, 1.0, 1600.0);
	// Modelview matrix reset
	glMatrixMode(GL_MODELVIEW);
}



void RenderScene(void)
{
	// Clear the window with current clearing colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// view the scene
	/*gluLookAt(cameraX, cameraY, cameraZ,//eye
		0.00, 0.00, 0.00,//centre
		0.00, 1.00, 0.00);//up*/

	// Clear the window with current clearing colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// Save the matrix state and do the rotations
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(0.0, 0.0, -200.0);

	glLineWidth(1.0);
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	//draw a teapot
	glRotatef(teapotRotation, 0.0f, -1.0f, 0.0f);
	glutWireTeapot(40.0);
	glPopMatrix();

	glLineWidth(1.0);
	//draw the 2D overlay
	glPushMatrix();
	glLoadIdentity();
	setOrthographicProjection();
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2f(20.0, 40.0);
	glVertex2f(100.0, 40.0);
	glEnd();
	drawFrameRate();

	//length of the bar is 180
	//180/360 0.5 
	//So we can increase the length of a line by 0.5 each frame

	glLineWidth(10.0);
	glBegin(GL_LINES);
	glColor3f(0.0, 1.0, 1.0);
	glVertex2f(20.0, 50.0);
	glVertex2f(lineXpos, 50.0);
	glEnd();

	glLineWidth(1.0);
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	//draw a teapot
	glTranslatef(Wwidth - 50.0, 50, 0);
	glRotatef(180, 1.0f, 0.0f, 0.0f);
	//glRotatef(teapotRotation, 0.0f, -1.0f, 0.0f);
	glutWireTeapot(20.0);
	glPopMatrix();

	drawAngleWithScaledText(0.5f);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glTranslatef(100, 40, 0.0);
	//draw a teapot
	glRotatef(0, 1.0f, 0.0f, 0.0f);
	glutWireTeapot(25.0);
	glPopMatrix();
	resetPerspectiveProjection();

	glPushMatrix();
	////rotate the quad slightly
	//glRotatef(15.0, 0.0, 1.0, 0.0);
	drawTexturedQuad(IMAGE1);
	glPopMatrix();


	glutSwapBuffers();
}

void drawTexturedQuad(int image)
{
	//add some lighting normals for each vertex
	//draw the texture on the front
	glEnable(GL_TEXTURE_2D);
	// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor3f(0.8, 0.8, 0.8);
	glEnable(GL_TEXTURE_2D);
	//bind the texture 
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(100.0, -50.0, 0.0);
	glTexCoord3f(1.0, 0.0, 0.0);
	glVertex3f(150.0, -50.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(150.0, 10.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(100.0, 10.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void TimerFunc(int value)
{
	glutTimerFunc(25, TimerFunc, 1);
	glutPostRedisplay();
	teapotRotation++;
	lineXpos = lineXpos + 0.5;
	if (teapotRotation > 360.0f)
	{
		teapotRotation = 0.0f;
		lineXpos = 20.0;
	}
	//printf("teapot rotation is  %f \n", teapotRotation);

}


void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}



int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);// a 16:9 ratio
	glutCreateWindow("2D Overlay");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(25, TimerFunc, 1);
	init();
	SetupRC();
	glutMainLoop();
	return 0;
}
