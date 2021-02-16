//Windows includes - DO NOT EDIT AS LOTS OF INTERDEPENDENCE

/*
Name: M.Fahd Fazal
UOW ID: w1789949
IIT ID: 2019656
youtube link: https://youtu.be/hg6_jiPEA4k
*/



#include <math.h>						
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>	//Needed for console output (debugging)
#include <gl/freeglut.h>
#include <iostream>
#include <vector>
using namespace std;

#ifdef WIN32
#include "gltools.h"
#include <windows.h>		// Must have for Windows platform builds
#include <gl\gl.h>			// Microsoft OpenGL headers (version 1.1 by themselves)
#include <gl\glu.h>			// OpenGL Utilities
#include "glm.h"
#endif

//foward declare functions
void resetPerspectiveProjection();
void drawScoreText(GLfloat scale);
void setOrthographicProjection();
void ChangeSize(int w, int h);
void RenderScene(void);
void TimerFunc(int value);

//  Created by Philip Trwoga on 09/11/2012. Modified on 20/1/16 to include the many VS2015 changes
//  Copyright (c) 2012 __Philip Trwoga__. All rights reserved.
// DO NOT DISTRIBUTE WITHOUT THE AUTHOR'S PERMISSION


//note that this needs gltools.h and gltools.cpp to be included in the shared/library directory

//can define any number of textures here - we just have 2 images
//these are just integers so you can use them as such

#define PI 3.14159265
#define IMAGE1      0
#define IMAGE2      1
#define IMAGE3      2
#define IMAGE4      3
#define TEXTURE_BALL 4    //football map
#define IMAGE_BLUE 5 
#define IMAGE_GREEN 6 
#define IMAGE_RED 7 
#define IMAGE_PALM      11 
#define IMAGE_YELLOW_FLOWER     12
#define IMAGE_ORANGE_FLOWER     13 
#define TEXTURE_COUNT 14
GLuint  textures[TEXTURE_COUNT];

//below is simply a character array to hold the file names
//note that you may need to replace the below with the full directory root depending on where you put your image files
//if you put them where the exe is then you just need the name as below - THESE IMAGES  ARE IN THE DEBUG FOLDER, YOU CAN ADD ANY NEW ONES THERE 
const char *textureFiles[TEXTURE_COUNT] = {"stormydays_large.tga", "horse.tga","grass_diff.tga","brick_texture_lo_res.tga","FootballCompleteMap.tga","targetBlue.tga","targetGreen.tga","targetRed.tga","fillBarHorizontal.tga","fillBarVerticalR.tga","targetDull.tga","palmBranchA.tga","yellowFlowerFinal.tga","orangeFlowerFinal5.tga"};


//for lighting if you want to experiment with these
GLfloat mKa[4] = {0.11f,0.06f,0.11f,1.0f}; //ambient
GLfloat mKd[4] = {0.43f,0.47f,0.54f,1.0f}; //diffuse
GLfloat mKs[4] = {1.0f,1.0f,1.0f,1.0f}; //specular
GLfloat mKe[4] = {0.5f,0.5f,0.0f,1.0f}; //emission

//spot position and direction
GLfloat	 lightPos[] = { 0.0, 100.0, 300.0, 0.0f };
GLfloat  spotDir[] = { 50.0, 25.0, 0.0 };

GLfloat	 lightPos2[] = { 50.0, 100.0, 350.0, 0.0f };
GLfloat  spotDir2[] = { 50.0, 15.0, 0.0 };

GLfloat	 lightPos3[] = { -50.0, 100.0, 300.0, 0.0f };
GLfloat  spotDir3[] = { 50.0, 15.0, 0.0 };


// Useful lighting colour values
GLfloat  whiteLightBright[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat  redLight[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat  greenLight[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat  blueLight[] = { 0.0f, 1.0f, 1.0f, 1.0f };
GLfloat  whiteLightLessBright[] = { 0.8f, 0.8f, 0.8f, 1.0f };


//we need these for the texture loader
//they are to do with the image format and size
GLint iWidth, iHeight, iComponents;
GLenum eFormat;
// this is a pointer to memory where the image bytes will be held 
GLbyte *pBytes0;

GLint Wwidth=600.0;
GLint Wheight=600.0;


//camera
GLfloat cameraX = 0.0;
GLfloat cameraY = 80.0;
GLfloat cameraZ = 500.0;
GLfloat camDirX = 0.00;
GLfloat camDirY = 0.00;

//variables for movement of the ball in respective direction
GLfloat xMove = 0.0;
GLfloat yMove = 0.0;
GLfloat zMove = 0.0;

//angle variables to decide the aim of the ball
GLfloat angleX = 90.0;
GLfloat angleY = 0.0;
GLfloat rotationAngle = 0.0; //rotation angle of the ball
GLfloat score = 0.0;
GLfloat rightSliderMove = 540.0;  //movement of the rightslider
GLfloat leftSliderMove = 40.0;    //movement of the 
int attempts = 0;  //no of attempts 

//variables related to the texture of the hoops
GLint blue1 = 5;
GLint blue2 = 5;
GLint green1 = 6;
GLint green2 = 6;
GLint red1 = 7;
GLint red2 = 7;
GLint red3 = 7;
GLint red4 = 7;
GLint dull = 10;
//arrays to store the center cordinates of the square blocks 
//index:0,1 is for blue hoops
//index:2,3 is for green hoops
//index:4,5,6,7 for red hoops
vector<float> xCordinates = {60.0,-100.0,175.0,-275.0,-320.0,-140.0,180.0,280.0};
vector<float> yCordinates = {80.0,30.0,35.0,75.0,10.0,100.0,100.0,60.0};



bool repeatOn = false;
bool repeatWallOn = false;
bool moveCamera = false;
bool shootBall = false;


// here we declare an array of vertices for the cube
GLfloat cubeVertexData[108] =
{
    //object vertex coordinates for cube made from triangles
    // Data layout for each line below is:
    // positionX, positionY, positionZ
    //wind counter-clockwise

    400.0f, -400.0f, -400.0f,
    400.0f, 400.0f, -400.0f,
    400.0f, -400.0f, 400.0f,
    400.0f, -400.0f, 400.0f,
    400.0f, 400.0f, -400.0f,
    400.0f, 400.0f, 400.0f,

    400.0f, 400.0f, -400.0f,
    -400.0f, 400.0f, -400.0f,
    400.0f, 400.0f, 400.0f,
    400.0f, 400.0f, 400.0f,
    -400.0f, 400.0f, -400.0f,
    -400.0f, 400.0f, 400.0f,

    -400.0f, 400.0f, -400.0f,
    -400.0f, -400.0f, -400.0f,
    -400.0f, 400.0f, 400.0f,
    -400.0f, 400.0f, 400.0f,
    -400.0f, -400.0f, -400.0f,
    -400.0f, -400.0f, 400.0f,
    //4
    -400.0f, -400.0f, -400.0f,
    400.0f, -400.0f, -400.0f,
    -400.0f, -400.0f, 400.0f,
    -400.0f, -400.0f, 400.0f,
    400.0f, -400.0f, -400.0f,
    400.0f, -400.0f, 400.0f,
    //5
    400.0f, 400.0f, 400.0f,
    -400.0f, 400.0f, 400.0f,
    400.0f, -400.0f, 400.0f,
    400.0f, -400.0f, 400.0f,
    -400.0f, 400.0f, 400.0f,
    -400.0f, -400.0f, 400.0f,
    //6
    400.0f, -400.0f, -400.0f,
    -400.0f, -400.0f, -400.0f,
    400.0f, 400.0f, -400.0f,
    400.0f, 400.0f, -400.0f,
    -400.0f, -400.0f, -400.0f,
    -400.0f, 400.0f, -400.0f

};

//the texture coordinates - work will skybox texture of this shape +--

GLfloat textureCoordsSkyBox[72] = {
    //face 1
    0.75,0.33,      //    0,1,
    0.75,0.67,     //    1,1,
    0.5,0.33,     //    0,0,
    0.5,0.33,     //    0,0,
    0.75,0.67,    //    1,0,
    0.5,0.67,   //    1,1,

    //face 2
    0.5,1.0, //    1,1,
    0.25,1, //    0,1,
    0.5,0.67, //    1,0,
    0.5,0.67, //    1,0,
    0.25,1.0, //    0,1,
    0.25,0.67, //    1,1,
    //face 3
    0,0.67,//    1,1,
    0,0.33,//    0,1,
    0.25,0.67,//    1,0,
    0.25,0.67,//    1,0,
    0,0.33,//    0,1,
    0.25,0.33,//    0,0,
    //face 4
    0.25,0.0,//    0,1,
    0.5,0.0,//    1,1,
    0.25,0.33,//    0,0,
    0.25,0.33,//    0,0,
    0.5,0.0,//    1,1,
    0.5,0.33,//    0,0,
    //face 5
    0.5,0.67,//    1,0,
    0.25,0.67,//    0,0,
    0.5,0.33,//    1,1,
    0.5,0.33,//    1,1,
    0.25,0.67,//    0,0,
    0.25,0.33,//    0,1,
    //face 6
    0.75,0.33,//    1,1,
    1.0,0.33,//    0,1,
    0.75,0.67,//    1,0,
    0.75,0.67,//    1,0,
    1.0,0.33,//    0,1,
    1.0,0.67//    0,0


};

//the lighting normals - all facing out from each face
GLfloat gCubeVertexdataNormals[108] =
{
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,

    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,

    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f

};


//end of intialisation 



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

//this function pops back to the last projection
void resetPerspectiveProjection()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

//method to restart the game where all the variables will be set to initial values
void restartGame() {
    shootBall = false;
    blue1 = 5;
    blue2 = 5;
    green1 = 6;
    green2 = 6;
    red1 = 7;
    red2 = 7;
    red3 = 7;
    red4 = 7;

    cameraX = 0.0;
    cameraY = 80.0;
    cameraZ = 500.0;
    camDirX = 0.00;
    camDirY = 0.00;
    xMove = 0.0;
    yMove = 0.0;
    zMove = 0.0;
    angleX = 90.0;
    angleY = 0.0;
    rotationAngle = 0.0;
    score = 0.0;
    rightSliderMove = 540.0;
    leftSliderMove = 40.0;

    attempts = 0;
    glutPostRedisplay();

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

char scoreString[12];

//method to draw the score in the screen
void drawScoreText(GLfloat scale)
{
    glPushMatrix();

    //draw the text offset from the box
    glTranslatef(455.0, 50.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    sprintf_s(scoreString, "Score:%3.0f", score);

    //flip
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glScalef(scale, scale,scale);
    //if stroke = true then use the tranlate above to move the text
    //if stroke = false then use the first two values of displayText

    displayText(455, 50, 0, 0, 0, scoreString, true);
    glPopMatrix();
}



void drawBlueHoop(int image)
{
    //add some lighting normals for each vertex
    //draw the texture on the front
    glEnable(GL_TEXTURE_2D);
   // glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
    glColor3f(0.8, 0.8, 0.8);
    glEnable( GL_TEXTURE_2D );
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[image]);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0,0.0);
    glVertex3f(-30.0, 0.0,60.0);
    glTexCoord3f(1.0,0.0,0.0);
    glVertex3f(30.0, 0.0,60.0);
    glTexCoord2f(1.0,1.0);
    glVertex3f(30.0, 60.0,60.0);
    glTexCoord2f(0.0,1.0);
    glVertex3f(-30.0, 60.0,60.0);
    glEnd();
    glDisable( GL_TEXTURE_2D );
}

void drawGreenHoop(int image)
{
    //add some lighting normals for each vertex
    //draw the texture on the front
    glEnable(GL_TEXTURE_2D);
    // glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
    glColor3f(0.8, 0.8, 0.8);
    glEnable(GL_TEXTURE_2D);
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[image]);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-35.0, 0.0, 70.0);
    glTexCoord3f(1.0, 0.0, 0.0);
    glVertex3f(35.0, 0.0, 70.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(35.0, 70.0, 70.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-35.0, 70.0, 70.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void drawRedHoop(int image)
{
    //add some lighting normals for each vertex
    //draw the texture on the front
    glEnable(GL_TEXTURE_2D);
    // glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
    glColor3f(0.8, 0.8, 0.8);
    glEnable(GL_TEXTURE_2D);
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[image]);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-20.0, 0.0, 40.0);
    glTexCoord3f(1.0, 0.0, 0.0);
    glVertex3f(20.0, 0.0, 40.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(20.0, 40.0, 40.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-20.0, 40.0, 40.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

//method to draw the mark at the ball position
void drawBallPositionHoop(int image)
{
    //add some lighting normals for each vertex
    //draw the texture on the front
    glEnable(GL_TEXTURE_2D);
    // glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
    glColor3f(0.8, 0.8, 0.8);
    glEnable(GL_TEXTURE_2D);
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[image]);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-10.0, 0.0, 20.0);
    glTexCoord3f(1.0, 0.0, 0.0);
    glVertex3f(10.0, 0.0, 20.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(10.0, 20.0, 20.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-10.0, 20.0, 20.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

//method to draw the left slider
void drawHorizontalFillBar(int image)
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
    glVertex3f(-20.0, 0.0, 5.0);
    glTexCoord3f(1.0, 0.0, 0.0);
    glVertex3f(20.0, 0.0, 5.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(20.0, 5.0, 5.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-20.0, 5.0, 5.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

//method to draw the right slider
void drawVerticalFillBar(int image)
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
    glVertex3f(-5.0, 0.0, 30.0);
    glTexCoord3f(1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 30.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.0, 30.0, 30.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-5.0, 30.0, 30.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void palmBranch(int image)
{
    //add some lighting normals for each vertex
    //draw the texture on the front
    glEnable(GL_TEXTURE_2D);
    // glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
    glColor3f(0.8, 0.8, 0.8);
    glEnable(GL_TEXTURE_2D);
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[image]);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-20.0, 0.0, 100.0);
    glTexCoord3f(1.0, 0.0, 0.0);
    glVertex3f(20.0, 0.0, 100.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(20.0, 100.0, 100.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-20.0, 100.0, 100.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void flowers(int image)
{
    //add some lighting normals for each vertex
    //draw the texture on the front
    glEnable(GL_TEXTURE_2D);
    // glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
    glColor3f(0.8, 0.8, 0.8);
    glEnable(GL_TEXTURE_2D);
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[image]);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-20.0, 0.0, 50.0);
    glTexCoord3f(1.0, 0.0, 0.0);
    glVertex3f(20.0, 0.0, 50.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(20.0, 50.0, 50.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-20.0, 50.0, 50.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}



//method to draw the grass floor
void drawTexturedSurface(int image)
{
	//add some lighting normals for each vertex
	//draw the texture on the front
	//draw face up and then rotated
	glEnable(GL_TEXTURE_2D);
	// glFrontFace(GL_CW); //use glFrontFace(GL_CW) to texture the other side - not needed here as we set this elsewhere
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	//bind the texture
	glBindTexture(GL_TEXTURE_2D, textures[image]);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(-4.0, -4.0);//repeated texture
	glVertex3f(-800.0, -800.0, 0.0);
	glTexCoord2f(4.0, -4.0);
	glVertex3f(800.0, -800.0, 0.0);
	glTexCoord2f(4.0, 4.0);
	glVertex3f(800.0, 800.0, 0.0);
	glTexCoord2f(-4.0, 4.0);
	glVertex3f(-800.0, 800.0, 0.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

//method to draw the sky
void drawSkyBox(int image) {
    glFrontFace(GL_CW); //texture the inside
    

    //bind your texture here
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[image]);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, gCubeVertexdataNormals);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, cubeVertexData);
   //glTexCoordPointer(2, GL_FLOAT, 0, textureCoords);
    glTexCoordPointer(2, GL_FLOAT, 0, textureCoordsSkyBox);
    // draw a cube - type - start number - number of vertices to draw (so 3 for single triangle)
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDisable(GL_TEXTURE_2D);
    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


//method to draw the walls
void drawBackWall(int image)
{
	//add some lighting normals for each vertex
	//draw the texture on the front
	//draw face up and then rotated
    glEnable(GL_TEXTURE_2D);
    glFrontFace(GL_CW);
    glColor3f(0.8, 0.8, 0.8);
    glEnable(GL_TEXTURE_2D);
    //bind the texture 
    glBindTexture(GL_TEXTURE_2D, textures[image]);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-350, 0.0, 0.0);//dL
    glTexCoord3f(1.0, 0.0, 0.0);
    glVertex3f(350, 0.0, 0.0);//dR
    glTexCoord2f(1.0, 1.0);
    glVertex3f(350, 200.0, 0.0);//ur
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-350, 200.0, 0.0);//uL
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

//---------This is used for the football ball-------//
GLUquadricObj *quadricFootball;
void drawFootBall(GLfloat x, GLfloat y, GLfloat z, GLfloat r) 
{
	glPushMatrix();
	glFrontFace(GL_CCW);
	glTranslatef(x, y, z);
	// Create and texture the ball
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_BALL]);
	// glDisable(GL_LIGHTING);
	glColor3f(0.5, 0.5, 0.5);
	quadricFootball = gluNewQuadric();
	gluQuadricDrawStyle(quadricFootball, GLU_FILL);
	gluQuadricNormals(quadricFootball, GLU_SMOOTH);
	gluQuadricOrientation(quadricFootball, GLU_OUTSIDE);
	gluQuadricTexture(quadricFootball, GL_TRUE);
	gluSphere(quadricFootball, r, 50, 35);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

//method to check the collision between the ball and the target hoops
void checkCollision(float x, float y) {

    //loop through all the target hoop and checks whether there is a collision
    //if there is a collision, then increase the score accordingly
    //change the texture of the collided target hoop
    for (int i = 0; i < 8; i++) {
        if ((x >= xCordinates[i] && x <= (xCordinates[i] + 40.0))) {
            if ((y >= yCordinates[i] && y <= (yCordinates[i] + 40.0))) {
                cout << "Its a hit\n";
                cout << "You have hit a target\n";
                switch (i) {
                case 0:
                    score = score + 20.0;
                    blue1 = dull;
                    break;
                case 1:
                    score = score + 20.0;
                    blue2 = dull;
                    break;
                case 2:
                    score = score + 10.0;
                    green1 = dull;
                    break;
                case 3:
                    score = score + 10.0;
                    green2 = dull;
                    break;
                case 4:
                    score = score + 30.0;
                    red1 = dull;
                    break;
                case 5:
                    score = score + 30.0;
                    red2 = dull;
                    break;
                case 6:
                    score = score + 30.0;
                    red3 = dull;
                    break;
                case 7:
                    score = score + 30.0;
                    red4 = dull;
                    break;
                }
                
            }
        }
    }
  
}


//method to perform keyboard functions 
void myKeyboardFunction(unsigned char key, int x, int y) {
    if (key == 'z' || key == ' ') {
        if (attempts >= 10) {
            cout << "Game Over\n";
            cout << "Restarting the game......\n";
            restartGame();  //calls the method to restart the game once all attempts are completed
        }
        else {
            shootBall = !shootBall;
            attempts = attempts + 1;
            glutPostRedisplay();
        }
    }else if(key=='f'){
        cout << "Camera mode is changed\n";
        moveCamera = !moveCamera;
    }
}

//method which handles arrow key events
void specialKey(int key,int x,int y) {
    //changes the aim according to the direction user selected 
    switch (key) {
    case GLUT_KEY_UP:
        cout<< "Up\n";
        angleY = angleY+1.0;
        rightSliderMove = rightSliderMove - 2.0;
        glutPostRedisplay();
        break;
    case GLUT_KEY_DOWN:
        cout << "Down\n";
        angleY = angleY - 1.0;
        rightSliderMove = rightSliderMove + 2.0;
        glutPostRedisplay();
        break;
    case GLUT_KEY_LEFT:
        cout << "Left\n";
        angleX = angleX + 2.0;
        leftSliderMove = leftSliderMove - 1.0;
        glutPostRedisplay();
        break;
    case GLUT_KEY_RIGHT:
        cout << "Right\n";
        angleX = angleX - 2.0;
        leftSliderMove = leftSliderMove + 1.0;
        glutPostRedisplay();
        break;
    }
}


// Called to draw scene
void RenderScene(void)
{
 
    // Clear the window with current clearing colour
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// view the scene
	gluLookAt(cameraX, cameraY, cameraZ,//eye
		camDirX, camDirY, 0.00,//centre
		0.00, 1.00, 0.00);//up

    


 	glPushMatrix();

	
    //grass floor
	glPushMatrix();
	glTranslatef(0.0, 0.0, -400.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	//drawTexturedSurfaceNoTiling(IMAGE3);
	drawTexturedSurface(IMAGE3);
	glPopMatrix();

	//back wall
	glPushMatrix();
	glTranslatef(0.0, 0.0, -350.0);
	//drawTexturedSurfaceNoTiling(IMAGE4);
	drawBackWall(IMAGE4);
	
	glPopMatrix();

    //left side wall
	glPushMatrix();
	glTranslatef(-350.0, 0.0, 0.0);
	glRotatef(-90,0.0,1.0,0.0);
	drawBackWall(IMAGE4);

	glPopMatrix();

    //sky box
    glPushMatrix();
    glTranslatef(0.0, 550.0,0.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    drawSkyBox(IMAGE1);

    glPopMatrix();

    //right side wall
	glPushMatrix();
	glTranslatef(350.0, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawBackWall(IMAGE4);

	glPopMatrix();

    //right side wall trees
    glPushMatrix();
    glTranslatef(150.0, 18.0, 0.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    palmBranch(IMAGE_PALM);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(150.0, 18.0, 60.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    palmBranch(IMAGE_PALM);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(150.0, 18.0, 45.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    palmBranch(IMAGE_PALM);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(150.0, 18.0, 150.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    palmBranch(IMAGE_PALM);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(150.0, 18.0, 200.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    palmBranch(IMAGE_PALM);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(150.0, 18.0, 220.0);
    glRotatef(90.0, 0.0, 1.0, 0.0);
    palmBranch(IMAGE_PALM);
    glPopMatrix();




  
    //left side wall trees
    glPushMatrix();
    glTranslatef(-150.0, 18.0, 0.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    palmBranch(IMAGE_PALM);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-150.0, 18.0, 60.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    palmBranch(IMAGE_PALM);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-150.0, 18.0, 45.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    palmBranch(IMAGE_PALM);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-150.0, 18.0, 150.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    palmBranch(IMAGE_PALM);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-150.0, 18.0, 200.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    palmBranch(IMAGE_PALM);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-150.0, 18.0, 220.0);
    glRotatef(-90.0, 0.0, 1.0, 0.0);
    palmBranch(IMAGE_PALM);
    glPopMatrix();

    //all the flowers
    glPushMatrix();
    glTranslatef(0.0, 0.0, -350.0);
    flowers(IMAGE_ORANGE_FLOWER);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(30.0, 0.0, -350.0);
    flowers(IMAGE_YELLOW_FLOWER);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(60.0, 0.0, -350.0);
    flowers(IMAGE_ORANGE_FLOWER);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(100.0, 0.0, -350.0);
    flowers(IMAGE_ORANGE_FLOWER);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(130.0, 0.0, -350.0);
    flowers(IMAGE_YELLOW_FLOWER);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(260.0, 0.0, -350.0);
    flowers(IMAGE_ORANGE_FLOWER);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(290.0, 0.0, -350.0);
    flowers(IMAGE_YELLOW_FLOWER);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-130.0, 0.0, -350.0);
    flowers(IMAGE_YELLOW_FLOWER);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-160.0, 0.0, -350.0);
    flowers(IMAGE_ORANGE_FLOWER);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-190.0, 0.0, -350.0);
    flowers(IMAGE_YELLOW_FLOWER);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-220.0, 0.0, -350.0);
    flowers(IMAGE_ORANGE_FLOWER);
    glPopMatrix();


    //target hoops
    glPushMatrix();
    glTranslatef(80.0, 70.0,-350.0);
    drawBlueHoop(blue1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-80.0, 20.0, -350.0);
    drawBlueHoop(blue2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(200.0, 20.0, -350.0);
    drawGreenHoop(green1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-250.0, 60.0, -350.0);
    drawGreenHoop(green2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-300.0, 10.0, -350.0);
    drawRedHoop(red1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-120.0, 100.0, -350.0);
    drawRedHoop(red2);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(200.0, 100.0, -350.0);
    drawRedHoop(red3);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(300.0, 60.0, -350.0);
    drawRedHoop(red4);
    glPopMatrix();

  

    glPushMatrix();
    glLoadIdentity();
    setOrthographicProjection();
    
    //bar in the right slider
    glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    glTranslatef(560.0,rightSliderMove,0.0);
    glRectf(10.0, 0.0, 23.0, 35.0);
    glPopMatrix();

    //bar in the left slider
    glPushMatrix();
    glColor3f(1.0, 0.5, 0.0);
    glTranslatef(leftSliderMove, 15.0, 0.0);
    glRectf(10.0, 0.0, 30.0, 21.0);
    glPopMatrix();

    //score
    glPushMatrix();
    drawScoreText(0.2);
    glPopMatrix();

    glPopMatrix();
    resetPerspectiveProjection();

    //left slider
    glPushMatrix();
    glTranslatef(-110.0, 120.0, 350.0);
    //glRotatef(1.0, 0.0, 0.0, -1.0);
    drawHorizontalFillBar(8);
    glPopMatrix();

    //right slider
    glPushMatrix();
    glTranslatef(105.0, 5.0, 370.0);
    glRotatef(-10.0,1.0,0.0,0.0);
    drawVerticalFillBar(9);
    glPopMatrix();

    //football
    glPushMatrix();
    glTranslatef(xMove, yMove, zMove);
    glTranslatef(0.0, 25.0, 350.0);
    glRotatef(rotationAngle,-1.0,0.0,0.0);
    glTranslatef(0.0, -25.0, -350.0);
    //glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);
	drawFootBall(0.0, 25.0, 350.0, 15.0);
    glPopMatrix();

    //mark hoop at football position
    glPushMatrix();
    glTranslatef(0.0, 30.0, 340.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    drawBallPositionHoop(dull);
    glPopMatrix();


    

	glutSwapBuffers();
}



// This function does any needed initialization on the rendering
// context.
void SetupRC()
{
    //textures

    GLuint texture;
    // allocate a texture name
    glGenTextures( 1, &texture );
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	// photoshop is a good converter to targa (TGA)
	//the gltLoadTGA method is found in gltools.cpp and is orignally from the OpenGL SuperBible book
	//there are quite a few ways of loading images
    // Load textures in a for loop
    glGenTextures(TEXTURE_COUNT, textures);
    //this puts the texture into OpenGL texture memory
 //   glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); - not defined so probably need to update GLEW - not needed here so ignore
    for(int iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++)
    {
        // Bind to next texture object
        glBindTexture(GL_TEXTURE_2D, textures[iLoop]);
        
        // Load texture data, set filter and wrap modes
        //note that gltLoadTGA is in the glm.cpp file and not a built-in openGL function
        pBytes0 = gltLoadTGA(textureFiles[iLoop],&iWidth, &iHeight,
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
	glEnable(GL_LIGHTING);
	glEnable(GL_POINT_SMOOTH);
	// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLightBright);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLightBright);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, mKs);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0f);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, whiteLightBright);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);
	glLightfv(GL_LIGHT1, GL_SPECULAR, mKs);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0f);
    //glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir2);
    glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT2, GL_DIFFUSE, whiteLightBright);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos3);
	glLightfv(GL_LIGHT2, GL_SPECULAR, mKs);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 60.0f);
    //glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDir3);
	glEnable(GL_LIGHT2);

	// Enable colour tracking
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity
	// with a high shine
	glMaterialfv(GL_FRONT, GL_SPECULAR, mKs);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	// Black blue background clear colour
	glClearColor(0.0f, 0.0f, 0.03f, 1.0f);
}


void TimerFunc(int value)
{
	
    //shoot the ball when z or spacebar is pressed
    if (shootBall)
    {
        zMove = zMove - 10.0;
        xMove = -zMove*cos(angleX* PI / 180);
        yMove = -zMove*sin(angleY* PI / 180);
        rotationAngle = rotationAngle + 10.0;

        //calculating the current position of the ball
        float ballX = 0.0 + xMove;
        float ballY = 25.0 + yMove;

        //moves the camera if f is pressed
        if (moveCamera) {
            cameraZ = cameraZ - 5.0;
        }

        //when the ball hits the wall ball comes back to the intial position
        //all the variables related to the motion of the ball is set to initial values
        if (zMove < -600.0) {
            zMove = 0.0;
            xMove = 0.0;
            yMove = 0.0;
            rotationAngle = 0.0;
            rightSliderMove = 540.0;
            leftSliderMove = 40.0;
            angleX = 90.0;
            angleY = 0.0;
            checkCollision(ballX,ballY);   //calls the method to check for collision
            shootBall = !shootBall;
            cameraZ = 500.0;

        }

    }

    glutPostRedisplay();
    glutTimerFunc(25, TimerFunc, 1);
}

void ProcessMenu(int choice) {

	switch (choice)
	{
	case 1:
		repeatOn = !repeatOn;
		break;
	case 2:
		repeatWallOn = !repeatWallOn;
		break;
	case 3:
		moveCamera = !moveCamera;
		if (!moveCamera) {
			cameraX = 0.0;
			cameraY = 100.0;
			cameraZ = 500.0;
		}
		break;

	default:
		break;
	}

	glutPostRedisplay();
}

void ChangeSize(int w, int h)
{
    GLfloat fAspect;
    
    // Prevent a divide by zero
    if(h == 0)
        h = 1;
    
    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);
    
    // Calculate aspect ratio of the window
    fAspect = (GLfloat)w/(GLfloat)h;
    
    // Set the perspective coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // field of view of 45 degrees, near and far planes 1.0 and 1000
    //that znear and zfar should typically have a ratio of 1000:1 to make sorting out z depth easier for the GPU
    gluPerspective(55.0f, fAspect, 1.0, 1000.0);
    
	// Modelview matrix reset
    glMatrixMode(GL_MODELVIEW);
}

void displayMenu() {
    cout << "...........WELCOME TO THE FOOTBALL GAME...........\n\n\n";
    cout << "....Instructions....\n\n";
    cout << "- You have 10 attempts to score maximum points\n";
    cout << "- Press z or spacebar to shoot the ball\n";
    cout << "- Use arrow keys to aim and aim sliders are there for assist\n";
    cout << "- Press f to toggle between the camera mode\n\n";
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);// a 16:9 ratio
	glutCreateWindow("Textures Tutorial");
    glutKeyboardFunc(myKeyboardFunction);
    glutSpecialFunc(specialKey);
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
    glutTimerFunc(25, TimerFunc, 1);
	// Create the Menu (right click on window)
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Toggle repeated texture", 1);
	glutAddMenuEntry("Toggle repeated wall", 2);
	glutAddMenuEntry("Toggle camera movement", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	SetupRC();
    displayMenu();
	glutMainLoop();
	return 0;
}






