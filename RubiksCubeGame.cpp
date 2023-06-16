//
//  finalProject.cpp
//  Graphics
//
//  Created by Samson Batula on 4/19/23.
//
//  Creator: Samson Batula
/******************************************
*
* Official Name:  Samsondeen Batula
*
* Call me: Samson
*
* E-mail:  sobatula@syr.edu
*
* Assignment:  Final Project (RUBIKS CUBE SIMULATOR)
*
* Environment/Compiler:  Xcode 14.1
*
* Date submitted:  April 30, 2023
*
* References:  list any references you used or write NONE
  Spotlights Youtube Video: https://www.youtube.com/watch?v=MAJqiDll0a8
 Open GL basic lighting documentation: https://learnopengl.com/Lighting/Basic-Lighting
*
* Interactions:
 
 DEFAULT KEYS:
 1 : Start the game (changes scenary from launch mode to game mode)
 2 : Celebration (fireworks) appear once this button is clicked.
 3: Closes the game
 L and l (lowercase L) : turns spotlight on and off. (it starts with all spotlights being on)

 Special KEYS:
 ↑ : moves the world up to view under the the cube
 ↓ : moves the world down to view the top of the cube
 →: moves the world right to view the right side of the cube
 ←: moves the world left to view the left side of the cube

 MOUSE CONTROL:
 
 LEFT CLICKS:
 CLICKING START ON VIEWPORT 1 (LEFT SIDE)  the game (changes scenary from launch mode to game mode)
 CLICKING SOLVED ON VIEWPORT 1 (LEFT SIDE) Celebration (fireworks) appear once this button is clicked.
 CLICKING EXIT ON VIEWPORT 1 (LEFT SIDE) Closes the game
 
 RIGHT CLICKS:
 MENU:
 Start
 Solved
 Load Screen
 Quit Game
 

*
*******************************************/

#define MAX_NUM_PARTICLES 10000
#define INITIAL_NUM_PARTICLES 2000
#define INITIAL_POINT_SIZE 1.0
#define INITIAL_SPEED 1.0

typedef int boolean;
#define TRUE 1
#define FALSE 0

#include <stdio.h>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

using namespace std;

void myIdle();
void collision(int);
float forces(int, int);

// Globals
static int width, height; // Size of the OpenGL window.
static int z1 = -1;
static int selection = 0;
static bool lighting = true; // lighting on/off
static bool displaylight = true; // display light on/off

// rotation
static bool frontrotate = false; // true for animation
static bool backrotate = false; // true for animation
static bool leftrotate = false; // true for animation
static bool rightrotate = false; // true for animation
static bool toprotate = false; // true for animation
static bool bottomrotate = false; // true for animation

static int rotatefaceY = 0;
static int rotatefaceZ = 0;
static int rotatefaceX = 0;

// cube variables
static int rotateX = 0; // rotate cubes pov in the x axis
static int rotateY = 0; // rotate cubes pov in the y axis
static int angle = 0;

// textures
static unsigned int texture[1];
static int id = 0;

/* particle struct */
typedef struct particle
{
     int color;
     float position[3];
     float velocity[3];
     float mass;
} particle;

particle particles[MAX_NUM_PARTICLES]; /* particle system */

/* initial state of particle system */

static int present_time;
static int last_time;
static int num_particles = INITIAL_NUM_PARTICLES;
static float point_size = INITIAL_POINT_SIZE;
static float speed = INITIAL_SPEED;
static bool gravity = FALSE; /* gravity off */
static bool elastic = FALSE; /* restitution off */
static bool repulsion = FALSE; /* repulsion off */
static float coef = 1.0; /* perfectly elastic collisions */
static float d2[MAX_NUM_PARTICLES][MAX_NUM_PARTICLES]; /* array for interparticle distances */

GLsizei wh = 800, ww = 400; /* initial window size */

GLfloat colors[8][3]={{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0},{0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {1.0, 0.0, 1.0}, {1.0, 1.0, 0.0},
    {1.0, 1.0, 1.0}};

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{
   char *c;
   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}
  
// Routine to draw a stroke character string.
void writeStrokeString(void *font, char *string)
{
   char *c;
   for (c = string; *c != '\0'; c++) glutStrokeCharacter(font, *c);
}

void TitleText (void) {
    glPushMatrix();
    
    glColor3f(0.0, 1.0, 0.0);
    glRasterPos3f(-28.0, 25.0, -30.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, "R");
    
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos3f(-26, 25.0, -30.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, "U");
    
    glColor3f(0.0, 0.0, 1.0);
    glRasterPos3f(-23.7, 25.0, -30.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, "B");
    
    glColor3f(1.0, 0.5, .0);
    glRasterPos3f(-21.9, 25.0, -30.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, "I");
    
    glColor3f(1.0, 1.0, .0);
    glRasterPos3f(-20.8, 25.0, -30.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, "K");
    
    glColor3f(1.0, 0.0, 0);
    glRasterPos3f(-18.6, 25.0, -30.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, "S");
    
    glColor3f(0.0, 0.8, 1.0);
    glRasterPos3f(-15, 25.0, -30.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, "CUBE");
    
    glColor3f(1.0, 0.5, 0);
    glRasterPos3f(-6, 25.0, -30.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, "SILMULATOR!");

    glPopMatrix();
    
}

void startText (void){
    glPushMatrix();
    
    glColor3f(0.0, 1.0, 0.0);
    glRasterPos3f(-28.0, 15.0, -30.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, "START");
    
    glPopMatrix();
    
}

void SolvedText (void){
    glPushMatrix();
    
    glColor3f(1.0, 0.5, 0.5);
    glRasterPos3f(-28.0, 5.0, -30.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, "SOLVED");
    
    glPopMatrix();
    
}

void exitText (void){
    glPushMatrix();
    
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos3f(-28.0, -5.0, -30.0);
    writeBitmapString(GLUT_BITMAP_HELVETICA_18, "EXIT");
    
    glPopMatrix();
    
}

// Struct of bitmap file
struct BitMapFile
{
    int sizeX;
    int sizeY;
    unsigned char* data;
};

// Routine to read a bitmap file.
// Works only for uncompressed bmp files of 24-bit color.
BitMapFile *getBMPData(string filename)
{
   BitMapFile *bmp = new BitMapFile;
   unsigned int size, offset, headerSize;
  
   // Read input file name.
   ifstream infile(filename.c_str(), ios::binary);
 
   // Get the starting point of the image data.
   infile.seekg(10);
   infile.read((char *) &offset, 4);
   
   // Get the header size of the bitmap.
   infile.read((char *) &headerSize,4);

   // Get width and height values in the bitmap header.
   infile.seekg(18);
   infile.read( (char *) &bmp->sizeX, 4);
   infile.read( (char *) &bmp->sizeY, 4);

   // Allocate buffer for the image.
   size = bmp->sizeX * bmp->sizeY * 24;
   bmp->data = new unsigned char[size];

   // Read bitmap data.
   infile.seekg(offset);
   infile.read((char *) bmp->data , size);
   
   // Reverse color from bgr to rgb.
   int temp;
   for (int i = 0; i < size; i += 3)
   {
      temp = bmp->data[i];
      bmp->data[i] = bmp->data[i+2];
      bmp->data[i+2] = temp;
   }

   return bmp;
}

// Load external textures.
void loadExternalTextures()
{
    // Local storage for bmp image data.
    BitMapFile *image[1];
    
    // Load the texture.
    image[0] = getBMPData("Textures/sulogo.bmp");
    // Activate texture index texture[0].
    glBindTexture(GL_TEXTURE_2D, texture[0]);

    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Specify an image as the texture to be bound with the currently active texture index.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
    
   
}

void logo(){
glEnable(GL_TEXTURE_2D);

// Activate a texture.
glBindTexture(GL_TEXTURE_2D, texture[0]);
glBegin(GL_POLYGON);
glTexCoord2f(0.0, 0.0); glVertex3f(-1, 3.55, -1.0);
glTexCoord2f(30.0, 0.0); glVertex3f(-1, 3.55, 1.0);
glTexCoord2f(30.0, 30.0); glVertex3f(1, 3.55, 1.0);
glTexCoord2f(0.0, 30.0); glVertex3f(1, 3.55, -1.0);
glEnd();
    
glDisable(GL_TEXTURE_2D);
}

/* ***************************************************************************************** */
// drawing the cube
void left_top_front (void){
    glPushMatrix();

     glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(-1.5, 3.5, 3.5);
      glVertex3f(-3.5, 3.5, 3.5);
      glVertex3f(-3.5, 1.5, 3.5);
      glVertex3f(-1.5, 1.5, 3.5);
    glEnd();

   glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(-1.5, 3.5, 1.5);
    glVertex3f(-1.5, 1.5, 1.5);
    glVertex3f(-3.5, 1.5, 1.5);
    glVertex3f(-3.5, 3.5, 1.5);
  glEnd();
      
     glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(-3.5, 3.5, 3.5);
      glVertex3f(-3.5, 3.5, 1.5);
      glVertex3f(-3.5, 1.5, 1.5);
      glVertex3f(-3.5, 1.5, 3.5);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(-1.5, 3.5, 3.5);
      glVertex3f(-1.5, 1.5, 3.5);
      glVertex3f(-1.5, 1.5, 1.5);
      glVertex3f(-1.5, 3.5, 1.5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(-3.5, 3.5, 1.5);
      glVertex3f(-3.5, 3.5, 3.5);
      glVertex3f(-1.5, 3.5, 3.5);
      glVertex3f(-1.5, 3.5, 1.5);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(-3.5, 1.5, 1.5);
        glVertex3f(-1.5, 1.5, 1.5);
        glVertex3f(-1.5, 1.5, 3.5);
        glVertex3f(-3.5, 1.5, 3.5);
      glEnd();

    glPopMatrix();
}

void left_top_mid (void){
    glPushMatrix();

     glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(-1.5, 3.5, 1);
      glVertex3f(-3.5, 3.5, 1);
      glVertex3f(-3.5, 1.5, 1);
      glVertex3f(-1.5, 1.5, 1);
    glEnd();

   glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(-1.5, 3.5, -1);
    glVertex3f(-1.5, 1.5, -1);
    glVertex3f(-3.5, 1.5, -1);
    glVertex3f(-3.5, 3.5, -1);
  glEnd();
      
     glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(-3.5, 3.5, 1);
      glVertex3f(-3.5, 3.5, -1);
      glVertex3f(-3.5, 1.5, -1);
      glVertex3f(-3.5, 1.5, 1);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(-1.5, 3.5, 1);
      glVertex3f(-1.5, 1.5, 1);
      glVertex3f(-1.5, 1.5, -1);
      glVertex3f(-1.5, 3.5, -1);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(-3.5, 3.5, -1);
      glVertex3f(-3.5, 3.5, 1);
      glVertex3f(-1.5, 3.5, 1);
      glVertex3f(-1.5, 3.5, -1);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(-3.5, 1.5, -1);
        glVertex3f(-1.5, 1.5, -1);
        glVertex3f(-1.5, 1.5, 1);
        glVertex3f(-3.5, 1.5, 1);
      glEnd();

    glPopMatrix();
}

void left_top_back (void){
    glPushMatrix();

    glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(-1.5, 3.5, -1.5);
      glVertex3f(-3.5, 3.5, -1.5);
      glVertex3f(-3.5, 1.5, -1.5);
      glVertex3f(-1.5, 1.5, -1.5);
    glEnd();

  glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(-1.5, 3.5, -3.5);
    glVertex3f(-1.5, 1.5, -3.5);
    glVertex3f(-3.5, 1.5, -3.5);
    glVertex3f(-3.5, 3.5, -3.5);
  glEnd();
      
    glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(-3.5, 3.5, -1.5);
      glVertex3f(-3.5, 3.5, -3.5);
      glVertex3f(-3.5, 1.5, -3.5);
      glVertex3f(-3.5, 1.5, -1.5);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(-1.5, 3.5, -1.5);
      glVertex3f(-1.5, 1.5, -1.5);
      glVertex3f(-1.5, 1.5, -3.5);
      glVertex3f(-1.5, 3.5, -3.5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(-3.5, 3.5, -3.5);
      glVertex3f(-3.5, 3.5, -1.5);
      glVertex3f(-1.5, 3.5, -1.5);
      glVertex3f(-1.5, 3.5, -3.5);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(-3.5, 1.5, -3.5);
        glVertex3f(-1.5, 1.5, -3.5);
        glVertex3f(-1.5, 1.5, -1.5);
        glVertex3f(-3.5, 1.5, -1.5);
      glEnd();

    glPopMatrix();
}

void left_mid_front (void){
    glPushMatrix();

     glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(-1.5, 1, 3.5);
      glVertex3f(-3.5, 1, 3.5);
      glVertex3f(-3.5, -1, 3.5);
      glVertex3f(-1.5, -1, 3.5);
    glEnd();

   glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(-1.5, 1, 1.5);
    glVertex3f(-1.5, -1, 1.5);
    glVertex3f(-3.5, -1, 1.5);
    glVertex3f(-3.5, 1, 1.5);
  glEnd();
      
     glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(-3.5, 1, 3.5);
      glVertex3f(-3.5, 1, 1.5);
      glVertex3f(-3.5, -1, 1.5);
      glVertex3f(-3.5, -1, 3.5);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(-1.5, 1, 3.5);
      glVertex3f(-1.5, -1, 3.5);
      glVertex3f(-1.5, -1, 1.5);
      glVertex3f(-1.5, 1, 1.5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(-3.5, 1, 1.5);
      glVertex3f(-3.5, 1, 3.5);
      glVertex3f(-1.5, 1, 3.5);
      glVertex3f(-1.5, 1, 1.5);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(-3.5, -1, 1.5);
        glVertex3f(-1.5, -1, 1.5);
        glVertex3f(-1.5, -1, 3.5);
        glVertex3f(-3.5, -1, 3.5);
      glEnd();

    glPopMatrix();
}

void left_mid_mid (void){
    glPushMatrix();

     glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(-1.5, 1, 1);
      glVertex3f(-3.5, 1, 1);
      glVertex3f(-3.5, -1, 1);
      glVertex3f(-1.5, -1, 1);
    glEnd();

   glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(-1.5, 1, -1);
    glVertex3f(-1.5, -1, -1);
    glVertex3f(-3.5, -1, -1);
    glVertex3f(-3.5, 1, -1);
  glEnd();
      
     glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(-3.5, 1, 1);
      glVertex3f(-3.5, 1, -1);
      glVertex3f(-3.5, -1, -1);
      glVertex3f(-3.5, -1, 1);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(-1.5, 1, 1);
      glVertex3f(-1.5, -1, 1);
      glVertex3f(-1.5, -1, -1);
      glVertex3f(-1.5, 1, -1);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(-3.5, 1, -1);
      glVertex3f(-3.5, 1, 1);
      glVertex3f(-1.5, 1, 1);
      glVertex3f(-1.5, 1, -1);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(-3.5, -1, -1);
        glVertex3f(-1.5, -1, -1);
        glVertex3f(-1.5, -1, 1);
        glVertex3f(-3.5, -1, 1);
      glEnd();

    glPopMatrix();
}

void left_mid_back (void){
    glPushMatrix();

    glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(-1.5, 1, -1.5);
      glVertex3f(-3.5, 1, -1.5);
      glVertex3f(-3.5, -1, -1.5);
      glVertex3f(-1.5, -1, -1.5);
    glEnd();

  glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(-1.5, 1, -3.5);
    glVertex3f(-1.5, -1, -3.5);
    glVertex3f(-3.5, -1, -3.5);
    glVertex3f(-3.5, 1, -3.5);
  glEnd();
      
    glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(-3.5, 1, -1.5);
      glVertex3f(-3.5, 1, -3.5);
      glVertex3f(-3.5, -1, -3.5);
      glVertex3f(-3.5, -1, -1.5);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(-1.5, 1, -1.5);
      glVertex3f(-1.5, -1, -1.5);
      glVertex3f(-1.5, -1, -3.5);
      glVertex3f(-1.5, 1, -3.5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(-3.5, 1, -3.5);
      glVertex3f(-3.5, 1, -1.5);
      glVertex3f(-1.5, 1, -1.5);
      glVertex3f(-1.5, 1, -3.5);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(-3.5, -1, -3.5);
        glVertex3f(-1.5, -1, -3.5);
        glVertex3f(-1.5, -1, -1.5);
        glVertex3f(-3.5, -1, -1.5);
      glEnd();

    glPopMatrix();
}
//  ***

void left_bot_front (void){
    glPushMatrix();

     glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(-1.5, -1.5, 3.5);
      glVertex3f(-3.5, -1.5, 3.5);
      glVertex3f(-3.5, -3.5, 3.5);
      glVertex3f(-1.5, -3.5, 3.5);
    glEnd();

   glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(-1.5, -1.5, 1.5);
    glVertex3f(-1.5, -3.5, 1.5);
    glVertex3f(-3.5, -3.5, 1.5);
    glVertex3f(-3.5, -1.5, 1.5);
  glEnd();
      
     glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(-3.5, -1.5, 3.5);
      glVertex3f(-3.5, -1.5, 1.5);
      glVertex3f(-3.5, -3.5, 1.5);
      glVertex3f(-3.5, -3.5, 3.5);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(-1.5, -1.5, 3.5);
      glVertex3f(-1.5, -3.5, 3.5);
      glVertex3f(-1.5, -3.5, 1.5);
      glVertex3f(-1.5, -1.5, 1.5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(-3.5, -1.5, 1.5);
      glVertex3f(-3.5, -1.5, 3.5);
      glVertex3f(-1.5, -1.5, 3.5);
      glVertex3f(-1.5, -1.5, 1.5);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(-3.5, -3.5, 1.5);
        glVertex3f(-1.5, -3.5, 1.5);
        glVertex3f(-1.5, -3.5, 3.5);
        glVertex3f(-3.5, -3.5, 3.5);
      glEnd();

    glPopMatrix();
}

void left_bot_mid (void){
    glPushMatrix();

     glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(-1.5, -1.5, 1);
      glVertex3f(-3.5, -1.5, 1);
      glVertex3f(-3.5, -3.5, 1);
      glVertex3f(-1.5, -3.5, 1);
    glEnd();

   glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(-1.5, -1.5, -1);
    glVertex3f(-1.5, -3.5, -1);
    glVertex3f(-3.5, -3.5, -1);
    glVertex3f(-3.5, -1.5, -1);
  glEnd();
      
     glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(-3.5, -1.5, 1);
      glVertex3f(-3.5, -1.5, -1);
      glVertex3f(-3.5, -3.5, -1);
      glVertex3f(-3.5, -3.5, 1);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(-1.5, -1.5, 1);
      glVertex3f(-1.5, -3.5, 1);
      glVertex3f(-1.5, -3.5, -1);
      glVertex3f(-1.5, -1.5, -1);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(-3.5, -1.5, -1);
      glVertex3f(-3.5, -1.5, 1);
      glVertex3f(-1.5, -1.5, 1);
      glVertex3f(-1.5, -1.5, -1);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(-3.5, -3.5, -1);
        glVertex3f(-1.5, -3.5, -1);
        glVertex3f(-1.5, -3.5, 1);
        glVertex3f(-3.5, -3.5, 1);
      glEnd();

    glPopMatrix();
}

void left_bot_back (void){
    glPushMatrix();

    glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(-1.5, -1.5, -1.5);
      glVertex3f(-3.5, -1.5, -1.5);
      glVertex3f(-3.5, -3.5, -1.5);
      glVertex3f(-1.5, -3.5, -1.5);
    glEnd();

  glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(-1.5, -1.5, -3.5);
    glVertex3f(-1.5, -3.5, -3.5);
    glVertex3f(-3.5, -3.5, -3.5);
    glVertex3f(-3.5, -1.5, -3.5);
  glEnd();
      
    glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(-3.5, -1.5, -1.5);
      glVertex3f(-3.5, -1.5, -3.5);
      glVertex3f(-3.5, -3.5, -3.5);
      glVertex3f(-3.5, -3.5, -1.5);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(-1.5, -1.5, -1.5);
      glVertex3f(-1.5, -3.5, -1.5);
      glVertex3f(-1.5, -3.5, -3.5);
      glVertex3f(-1.5, -1.5, -3.5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(-3.5, -1.5, -3.5);
      glVertex3f(-3.5, -1.5, -1.5);
      glVertex3f(-1.5, -1.5, -1.5);
      glVertex3f(-1.5, -1.5, -3.5);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(-3.5, -3.5, -3.5);
        glVertex3f(-1.5, -3.5, -3.5);
        glVertex3f(-1.5, -3.5, -1.5);
        glVertex3f(-3.5, -3.5, -1.5);
      glEnd();

    glPopMatrix();
}

void mid_top_front (void){
    glPushMatrix();

     glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(1, 3.5, 3.5);
      glVertex3f(-1, 3.5, 3.5);
      glVertex3f(-1, 1.5, 3.5);
      glVertex3f(1, 1.5, 3.5);
    glEnd();

   glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(1, 3.5, 1.5);
    glVertex3f(1, 1.5, 1.5);
    glVertex3f(-1, 1.5, 1.5);
    glVertex3f(-1, 3.5, 1.5);
  glEnd();
      
     glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(-1, 3.5, 3.5);
      glVertex3f(-1, 3.5, 1.5);
      glVertex3f(-1, 1.5, 1.5);
      glVertex3f(-1, 1.5, 3.5);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(1, 3.5, 3.5);
      glVertex3f(1, 1.5, 3.5);
      glVertex3f(1, 1.5, 1.5);
      glVertex3f(1, 3.5, 1.5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(-1, 3.5, 1.5);
      glVertex3f(-1, 3.5, 3.5);
      glVertex3f(1, 3.5, 3.5);
      glVertex3f(1, 3.5, 1.5);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(-1, 1.5, 1.5);
        glVertex3f(1, 1.5, 1.5);
        glVertex3f(1, 1.5, 3.5);
        glVertex3f(-1, 1.5, 3.5);
      glEnd();

    glPopMatrix();
}

void mid_top_mid (void){
    
    glPushMatrix();

     glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(1, 3.5, 1);
      glVertex3f(-1, 3.5, 1);
      glVertex3f(-1, 1.5, 1);
      glVertex3f(1, 1.5, 1);
    glEnd();

   glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(1, 3.5, -1);
    glVertex3f(1, 1.5, -1);
    glVertex3f(-1, 1.5, -1);
    glVertex3f(-1, 3.5, -1);
  glEnd();
      
     glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(-1, 3.5, 1);
      glVertex3f(-1, 3.5, -1);
      glVertex3f(-1, 1.5, -1);
      glVertex3f(-1, 1.5, 1);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(1, 3.5, 1);
      glVertex3f(1, 1.5, 1);
      glVertex3f(1, 1.5, -1);
      glVertex3f(1, 3.5, -1);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(-1, 3.5, -1);
      glVertex3f(-1, 3.5, 1);
      glVertex3f(1, 3.5, 1);
      glVertex3f(1, 3.5, -1);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(-1, 1.5, -1);
        glVertex3f(1, 1.5, -1);
        glVertex3f(1, 1.5, 1);
        glVertex3f(-1, 1.5, 1);
      glEnd();

    glPopMatrix();

}

void mid_top_back (void){
    glPushMatrix();

    glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(1, 3.5, -1.5);
      glVertex3f(-1, 3.5, -1.5);
      glVertex3f(-1, 1.5, -1.5);
      glVertex3f(1, 1.5, -1.5);
    glEnd();

  glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(1, 3.5, -3.5);
    glVertex3f(1, 1.5, -3.5);
    glVertex3f(-1, 1.5, -3.5);
    glVertex3f(-1, 3.5, -3.5);
  glEnd();
      
    glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(-1, 3.5, -1.5);
      glVertex3f(-1, 3.5, -3.5);
      glVertex3f(-1, 1.5, -3.5);
      glVertex3f(-1, 1.5, -1.5);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(1, 3.5, -1.5);
      glVertex3f(1, 1.5, -1.5);
      glVertex3f(1, 1.5, -3.5);
      glVertex3f(1, 3.5, -3.5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(-1, 3.5, -3.5);
      glVertex3f(-1, 3.5, -1.5);
      glVertex3f(1, 3.5, -1.5);
      glVertex3f(1, 3.5, -3.5);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(-1, 1.5, -3.5);
        glVertex3f(1, 1.5, -3.5);
        glVertex3f(1, 1.5, -1.5);
        glVertex3f(-1, 1.5, -1.5);
      glEnd();

    glPopMatrix();
}

void mid_front_mid (void){
    glPushMatrix();

     glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(1, 1, 3.5);
      glVertex3f(-1, 1, 3.5);
      glVertex3f(-1, -1, 3.5);
      glVertex3f(1, -1, 3.5);
    glEnd();

   glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(1, 1, 1.5);
    glVertex3f(1, -1, 1.5);
    glVertex3f(-1, -1, 1.5);
    glVertex3f(-1, 1, 1.5);
  glEnd();
      
     glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(-1, 1, 3.5);
      glVertex3f(-1, 1, 1.5);
      glVertex3f(-1, -1, 1.5);
      glVertex3f(-1, -1, 3.5);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(1, 1, 3.5);
      glVertex3f(1, -1, 3.5);
      glVertex3f(1, -1, 1.5);
      glVertex3f(1, 1, 1.5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(-1, 1, 1.5);
      glVertex3f(-1, 1, 3.5);
      glVertex3f(1, 1, 3.5);
      glVertex3f(1, 1, 1.5);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(-1, -1, 1.5);
        glVertex3f(1, -1, 1.5);
        glVertex3f(1, -1, 3.5);
        glVertex3f(-1, -1, 3.5);
      glEnd();

    glPopMatrix();
    
}

void mid_mid_mid(void)
{
  glPushMatrix();

   glColor3f(1.0f, 0.0f, 0.0f); // red
  glBegin(GL_QUADS);  // front
    glNormal3f(0.0, 0.0, 1.0);  // face normal
    glVertex3f(1, 1, 1);
    glVertex3f(-1, 1, 1);
    glVertex3f(-1, -1, 1);
    glVertex3f(1, -1, 1);
  glEnd();

 glColor3f(1.0f, 0.5f, 0.0f); // orange
glBegin(GL_QUADS);  // back
  glNormal3f(0.0, 0.0, -1.0);  // face normal
  glVertex3f(1, 1, -1);
  glVertex3f(1, -1, -1);
  glVertex3f(-1, -1, -1);
  glVertex3f(-1, 1, -1);
glEnd();
    
   glColor3f(0.0f, 1.0f, 0.0f); // blue
  glBegin(GL_QUADS);  // left
    glNormal3f(-1.0, 0.0, 0.0);  // face normal
    glVertex3f(-1, 1, 1);
    glVertex3f(-1, 1, -1);
    glVertex3f(-1, -1, -1);
    glVertex3f(-1, -1, 1);
  glEnd();
  
    glColor3f(0.0f, 0.0f, 1.0f); // green
  glBegin(GL_QUADS);  // right
    glNormal3f(1.0, 0.0, 0.0);  // face normal
    glVertex3f(1, 1, 1);
    glVertex3f(1, -1, 1);
    glVertex3f(1, -1, -1);
    glVertex3f(1, 1, -1);
  glEnd();

  glColor3f(1.0f, 1.0f, 1.0f); // white
  glBegin(GL_QUADS);  // top
    glNormal3f(0.0, 1.0, 0.0);  // face normal
    glVertex3f(-1, 1, -1);
    glVertex3f(-1, 1, 1);
    glVertex3f(1, 1, 1);
    glVertex3f(1, 1, -1);
  glEnd();
    
    glColor3f(1.0f, 1.0f, 0.0f); // yellow
    glBegin(GL_QUADS);  // bottom
      glNormal3f(0.0, -1.0, 0.0);  // face normal
      glVertex3f(-1, -1, -1);
      glVertex3f(1, -1, -1);
      glVertex3f(1, -1, 1);
      glVertex3f(-1, -1, 1);
    glEnd();

  glPopMatrix();

}

void mid_mid_back (void){
    glPushMatrix();

    glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(1, 1, -1.5);
      glVertex3f(-1, 1, -1.5);
      glVertex3f(-1, -1, -1.5);
      glVertex3f(1, -1, -1.5);
    glEnd();

  glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(1, 1, -3.5);
    glVertex3f(1, -1, -3.5);
    glVertex3f(-1, -1, -3.5);
    glVertex3f(-1, 1, -3.5);
  glEnd();
      
    glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(-1, 1, -1.5);
      glVertex3f(-1, 1, -3.5);
      glVertex3f(-1, -1, -3.5);
      glVertex3f(-1, -1, -1.5);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(1, 1, -1.5);
      glVertex3f(1, -1, -1.5);
      glVertex3f(1, -1, -3.5);
      glVertex3f(1, 1, -3.5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(-1, 1, -3.5);
      glVertex3f(-1, 1, -1.5);
      glVertex3f(1, 1, -1.5);
      glVertex3f(1, 1, -3.5);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(-1, -1, -3.5);
        glVertex3f(1, -1, -3.5);
        glVertex3f(1, -1, -1.5);
        glVertex3f(-1, -1, -1.5);
      glEnd();

    glPopMatrix();
}

void mid_bot_front (void){
    glPushMatrix();

     glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(1, -1.5, 3.5);
      glVertex3f(-1, -1.5, 3.5);
      glVertex3f(-1, -3.5, 3.5);
      glVertex3f(1, -3.5, 3.5);
    glEnd();

   glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(1, -1.5, 1.5);
    glVertex3f(1, -3.5, 1.5);
    glVertex3f(-1, -3.5, 1.5);
    glVertex3f(-1, -1.5, 1.5);
  glEnd();
      
     glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(-1, -1.5, 3.5);
      glVertex3f(-1, -1.5, 1.5);
      glVertex3f(-1, -3.5, 1.5);
      glVertex3f(-1, -3.5, 3.5);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(1, -1.5, 3.5);
      glVertex3f(1, -3.5, 3.5);
      glVertex3f(1, -3.5, 1.5);
      glVertex3f(1, -1.5, 1.5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(-1, -1.5, 1.5);
      glVertex3f(-1, -1.5, 3.5);
      glVertex3f(1, -1.5, 3.5);
      glVertex3f(1, -1.5, 1.5);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(-1, -3.5, 1.5);
        glVertex3f(1, -3.5, 1.5);
        glVertex3f(1, -3.5, 3.5);
        glVertex3f(-1, -3.5, 3.5);
      glEnd();

    glPopMatrix();
}

void mid_bot_mid (void){
    glPushMatrix();

     glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(1, -1.5, 1);
      glVertex3f(-1, -1.5, 1);
      glVertex3f(-1, -3.5, 1);
      glVertex3f(1, -3.5, 1);
    glEnd();

   glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(1, -1.5, -1);
    glVertex3f(1, -3.5, -1);
    glVertex3f(-1, -3.5, -1);
    glVertex3f(-1, -1.5, -1);
  glEnd();
      
     glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(-1, -1.5, 1);
      glVertex3f(-1, -1.5, -1);
      glVertex3f(-1, -3.5, -1);
      glVertex3f(-1, -3.5, 1);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(1, -1.5, 1);
      glVertex3f(1, -3.5, 1);
      glVertex3f(1, -3.5, -1);
      glVertex3f(1, -1.5, -1);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(-1, -1.5, -1);
      glVertex3f(-1, -1.5, 1);
      glVertex3f(1, -1.5, 1);
      glVertex3f(1, -1.5, -1);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(-1, -3.5, -1);
        glVertex3f(1, -3.5, -1);
        glVertex3f(1, -3.5, 1);
        glVertex3f(-1, -3.5, 1);
      glEnd();

    glPopMatrix();
}

void mid_bot_back (void){
    glPushMatrix();

    glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(1, -1.5, -1.5);
      glVertex3f(-1, -1.5, -1.5);
      glVertex3f(-1, -3.5, -1.5);
      glVertex3f(1, -3.5, -1.5);
    glEnd();

  glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(1, -1.5, -3.5);
    glVertex3f(1, -3.5, -3.5);
    glVertex3f(-1, -3.5, -3.5);
    glVertex3f(-1, -1.5, -3.5);
  glEnd();
      
    glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(-1, -1.5, -1.5);
      glVertex3f(-1, -1.5, -3.5);
      glVertex3f(-1, -3.5, -3.5);
      glVertex3f(-1, -3.5, -1.5);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(1, -1.5, -1.5);
      glVertex3f(1, -3.5, -1.5);
      glVertex3f(1, -3.5, -3.5);
      glVertex3f(1, -1.5, -3.5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(-1, -1.5, -3.5);
      glVertex3f(-1, -1.5, -1.5);
      glVertex3f(1, -1.5, -1.5);
      glVertex3f(1, -1.5, -3.5);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(-1, -3.5, -3.5);
        glVertex3f(1, -3.5, -3.5);
        glVertex3f(1, -3.5, -1.5);
        glVertex3f(-1, -3.5, -1.5);
      glEnd();

    glPopMatrix();
    
}

void right_top_front (void){
    glPushMatrix();

     glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(3.5, 3.5, 3.5);
      glVertex3f(1.5, 3.5, 3.5);
      glVertex3f(1.5, 1.5, 3.5);
      glVertex3f(3.5, 1.5, 3.5);
    glEnd();

   glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(3.5, 3.5, 1.5);
    glVertex3f(3.5, 1.5, 1.5);
    glVertex3f(1.5, 1.5, 1.5);
    glVertex3f(1.5, 3.5, 1.5);
  glEnd();
      
     glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(1.5, 3.5, 3.5);
      glVertex3f(1.5, 3.5, 1.5);
      glVertex3f(1.5, 1.5, 1.5);
      glVertex3f(1.5, 1.5, 3.5);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(3.5, 3.5, 3.5);
      glVertex3f(3.5, 1.5, 3.5);
      glVertex3f(3.5, 1.5, 1.5);
      glVertex3f(3.5, 3.5, 1.5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(1.5, 3.5, 1.5);
      glVertex3f(1.5, 3.5, 3.5);
      glVertex3f(3.5, 3.5, 3.5);
      glVertex3f(3.5, 3.5, 1.5);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(1.5, 1.5, 1.5);
        glVertex3f(3.5, 1.5, 1.5);
        glVertex3f(3.5, 1.5, 3.5);
        glVertex3f(1.5, 1.5, 3.5);
      glEnd();

    glPopMatrix();
}

void right_top_mid (void){
    glPushMatrix();

     glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(3.5, 3.5, 1);
      glVertex3f(1.5, 3.5, 1);
      glVertex3f(1.5, 1.5, 1);
      glVertex3f(3.5, 1.5, 1);
    glEnd();

   glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(3.5, 3.5, -1);
    glVertex3f(3.5, 1.5, -1);
    glVertex3f(1.5, 1.5, -1);
    glVertex3f(1.5, 3.5, -1);
  glEnd();
      
     glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(1.5, 3.5, 1);
      glVertex3f(1.5, 3.5, -1);
      glVertex3f(1.5, 1.5, -1);
      glVertex3f(1.5, 1.5, 1);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(3.5, 3.5, 1);
      glVertex3f(3.5, 1.5, 1);
      glVertex3f(3.5, 1.5, -1);
      glVertex3f(3.5, 3.5, -1);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(1.5, 3.5, -1);
      glVertex3f(1.5, 3.5, 1);
      glVertex3f(3.5, 3.5, 1);
      glVertex3f(3.5, 3.5, -1);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(1.5, 1.5, -1);
        glVertex3f(3.5, 1.5, -1);
        glVertex3f(3.5, 1.5, 1);
        glVertex3f(1.5, 1.5, 1);
      glEnd();

    glPopMatrix();
}

void right_top_back (void){
    glPushMatrix();

    glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(3.5, 3.5, -1.5);
      glVertex3f(1.5, 3.5, -1.5);
      glVertex3f(1.5, 1.5, -1.5);
      glVertex3f(3.5, 1.5, -1.5);
    glEnd();

  glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(3.5, 3.5, -3.5);
    glVertex3f(3.5, 1.5, -3.5);
    glVertex3f(1.5, 1.5, -3.5);
    glVertex3f(1.5, 3.5, -3.5);
  glEnd();
      
    glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(1.5, 3.5, -1.5);
      glVertex3f(1.5, 3.5, -3.5);
      glVertex3f(1.5, 1.5, -3.5);
      glVertex3f(1.5, 1.5, -1.5);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(3.5, 3.5, -1.5);
      glVertex3f(3.5, 1.5, -1.5);
      glVertex3f(3.5, 1.5, -3.5);
      glVertex3f(3.5, 3.5, -3.5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(1.5, 3.5, -3.5);
      glVertex3f(1.5, 3.5, -1.5);
      glVertex3f(3.5, 3.5, -1.5);
      glVertex3f(3.5, 3.5, -3.5);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(1.5, 1.5, -3.5);
        glVertex3f(3.5, 1.5, -3.5);
        glVertex3f(3.5, 1.5, -1.5);
        glVertex3f(1.5, 1.5, -1.5);
      glEnd();

    glPopMatrix();
}

void right_mid_front (void){
    glPushMatrix();

     glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(3.5, 1, 3.5);
      glVertex3f(1.5, 1, 3.5);
      glVertex3f(1.5, -1, 3.5);
      glVertex3f(3.5, -1, 3.5);
    glEnd();

   glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(3.5, 1, 1.5);
    glVertex3f(3.5, -1, 1.5);
    glVertex3f(1.5, -1, 1.5);
    glVertex3f(1.5, 1, 1.5);
  glEnd();
      
     glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(1.5, 1, 3.5);
      glVertex3f(1.5, 1, 1.5);
      glVertex3f(1.5, -1, 1.5);
      glVertex3f(1.5, -1, 3.5);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(3.5, 1, 3.5);
      glVertex3f(3.5, -1, 3.5);
      glVertex3f(3.5, -1, 1.5);
      glVertex3f(3.5, 1, 1.5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(1.5, 1, 1.5);
      glVertex3f(1.5, 1, 3.5);
      glVertex3f(3.5, 1, 3.5);
      glVertex3f(3.5, 1, 1.5);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(1.5, -1, 1.5);
        glVertex3f(3.5, -1, 1.5);
        glVertex3f(3.5, -1, 3.5);
        glVertex3f(1.5, -1, 3.5);
      glEnd();

    glPopMatrix();
}

void right_mid_mid (void){
    glPushMatrix();

     glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(3.5, 1, 1);
      glVertex3f(1.5, 1, 1);
      glVertex3f(1.5, -1, 1);
      glVertex3f(3.5, -1, 1);
    glEnd();

   glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(3.5, 1, -1);
    glVertex3f(3.5, -1, -1);
    glVertex3f(1.5, -1, -1);
    glVertex3f(1.5, 1, -1);
  glEnd();
      
     glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(1.5, 1, 1);
      glVertex3f(1.5, 1, -1);
      glVertex3f(1.5, -1, -1);
      glVertex3f(1.5, -1, 1);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(3.5, 1, 1);
      glVertex3f(3.5, -1, 1);
      glVertex3f(3.5, -1, -1);
      glVertex3f(3.5, 1, -1);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(1.5, 1, -1);
      glVertex3f(1.5, 1, 1);
      glVertex3f(3.5, 1, 1);
      glVertex3f(3.5, 1, -1);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(1.5, -1, -1);
        glVertex3f(3.5, -1, -1);
        glVertex3f(3.5, -1, 1);
        glVertex3f(1.5, -1, 1);
      glEnd();

    glPopMatrix();
}

void right_mid_back (void){
    glPushMatrix();

    glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(3.5, 1, -1.5);
      glVertex3f(1.5, 1, -1.5);
      glVertex3f(1.5, -1, -1.5);
      glVertex3f(3.5, -1, -1.5);
    glEnd();

  glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(3.5, 1, -3.5);
    glVertex3f(3.5, -1, -3.5);
    glVertex3f(1.5, -1, -3.5);
    glVertex3f(1.5, 1, -3.5);
  glEnd();
      
    glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(1.5, 1, -1.5);
      glVertex3f(1.5, 1, -3.5);
      glVertex3f(1.5, -1, -3.5);
      glVertex3f(1.5, -1, -1.5);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(3.5, 1, -1.5);
      glVertex3f(3.5, -1, -1.5);
      glVertex3f(3.5, -1, -3.5);
      glVertex3f(3.5, 1, -3.5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(1.5, 1, -3.5);
      glVertex3f(1.5, 1, -1.5);
      glVertex3f(3.5, 1, -1.5);
      glVertex3f(3.5, 1, -3.5);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(1.5, -1, -3.5);
        glVertex3f(3.5, -1, -3.5);
        glVertex3f(3.5, -1, -1.5);
        glVertex3f(1.5, -1, -1.5);
      glEnd();

    glPopMatrix();
}

void right_bot_front (void){
    glPushMatrix();

     glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(3.5, -1.5, 3.5);
      glVertex3f(1.5, -1.5, 3.5);
      glVertex3f(1.5, -3.5, 3.5);
      glVertex3f(3.5, -3.5, 3.5);
    glEnd();

   glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(3.5, -1.5, 1.5);
    glVertex3f(3.5, -3.5, 1.5);
    glVertex3f(1.5, -3.5, 1.5);
    glVertex3f(1.5, -1.5, 1.5);
  glEnd();
      
     glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(1.5, -1.5, 3.5);
      glVertex3f(1.5, -1.5, 1.5);
      glVertex3f(1.5, -3.5, 1.5);
      glVertex3f(1.5, -3.5, 3.5);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(3.5, -1.5, 3.5);
      glVertex3f(3.5, -3.5, 3.5);
      glVertex3f(3.5, -3.5, 1.5);
      glVertex3f(3.5, -1.5, 1.5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(1.5, -1.5, 1.5);
      glVertex3f(1.5, -1.5, 3.5);
      glVertex3f(3.5, -1.5, 3.5);
      glVertex3f(3.5, -1.5, 1.5);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(1.5, -3.5, 1.5);
        glVertex3f(3.5, -3.5, 1.5);
        glVertex3f(3.5, -3.5, 3.5);
        glVertex3f(1.5, -3.5, 3.5);
      glEnd();

    glPopMatrix();
}

void right_bot_mid (void){
    glPushMatrix();

     glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(3.5, -1.5, 1);
      glVertex3f(1.5, -1.5, 1);
      glVertex3f(1.5, -3.5, 1);
      glVertex3f(3.5, -3.5, 1);
    glEnd();

   glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(3.5, -1.5, -1);
    glVertex3f(3.5, -3.5, -1);
    glVertex3f(1.5, -3.5, -1);
    glVertex3f(1.5, -1.5, -1);
  glEnd();
      
     glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(1.5, -1.5, 1);
      glVertex3f(1.5, -1.5, -1);
      glVertex3f(1.5, -3.5, -1);
      glVertex3f(1.5, -3.5, 1);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(3.5, -1.5, 1);
      glVertex3f(3.5, -3.5, 1);
      glVertex3f(3.5, -3.5, -1);
      glVertex3f(3.5, -1.5, -1);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(1.5, -1.5, -1);
      glVertex3f(1.5, -1.5, 1);
      glVertex3f(3.5, -1.5, 1);
      glVertex3f(3.5, -1.5, -1);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(1.5, -3.5, -1);
        glVertex3f(3.5, -3.5, -1);
        glVertex3f(3.5, -3.5, 1);
        glVertex3f(1.5, -3.5, 1);
      glEnd();

    glPopMatrix();
}

void right_bot_back (void){
    glPushMatrix();

    glColor3f(1.0f, 0.0f, 0.0f); // red
    glBegin(GL_QUADS);  // front
      glNormal3f(0.0, 0.0, 1.0);  // face normal
      glVertex3f(3.5, -1.5, -1.5);
      glVertex3f(1.5, -1.5, -1.5);
      glVertex3f(1.5, -3.5, -1.5);
      glVertex3f(3.5, -3.5, -1.5);
    glEnd();

  glColor3f(1.0f, 0.5f, 0.0f); // orange
  glBegin(GL_QUADS);  // back
    glNormal3f(0.0, 0.0, -1.0);  // face normal
    glVertex3f(3.5, -1.5, -3.5);
    glVertex3f(3.5, -3.5, -3.5);
    glVertex3f(1.5, -3.5, -3.5);
    glVertex3f(1.5, -1.5, -3.5);
  glEnd();
      
    glColor3f(0.0f, 1.0f, 0.0f); // blue
    glBegin(GL_QUADS);  // left
      glNormal3f(-1.0, 0.0, 0.0);  // face normal
      glVertex3f(1.5, -1.5, -1.5);
      glVertex3f(1.5, -1.5, -3.5);
      glVertex3f(1.5, -3.5, -3.5);
      glVertex3f(1.5, -3.5, -1.5);
    glEnd();
    
      glColor3f(0.0f, 0.0f, 1.0f); // green
    glBegin(GL_QUADS);  // right
      glNormal3f(1.0, 0.0, 0.0);  // face normal
      glVertex3f(3.5, -1.5, -1.5);
      glVertex3f(3.5, -3.5, -1.5);
      glVertex3f(3.5, -3.5, -3.5);
      glVertex3f(3.5, -1.5, -3.5);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f); // white
    glBegin(GL_QUADS);  // top
      glNormal3f(0.0, 1.0, 0.0);  // face normal
      glVertex3f(1.5, -1.5, -3.5);
      glVertex3f(1.5, -1.5, -1.5);
      glVertex3f(3.5, -1.5, -1.5);
      glVertex3f(3.5, -1.5, -3.5);
    glEnd();
      
      glColor3f(1.0f, 1.0f, 0.0f); // yellow
      glBegin(GL_QUADS);  // bottom
        glNormal3f(0.0, -1.0, 0.0);  // face normal
        glVertex3f(1.5, -3.5, -3.5);
        glVertex3f(3.5, -3.5, -3.5);
        glVertex3f(3.5, -3.5, -1.5);
        glVertex3f(1.5, -3.5, -1.5);
      glEnd();

    glPopMatrix();
}


/* ***************************************************************************************** */

void myIdle()
{
    int i, j, k;
    float dt;
    present_time = glutGet(GLUT_ELAPSED_TIME);
    dt = 0.001*(present_time -  last_time);
    for(i=0; i<num_particles; i++)
    {
       for(j=0; j<3; j++)
       {
           particles[i].position[j]+=dt*particles[i].velocity[j];
           particles[i].velocity[j]+=dt*forces(i,j)/particles[i].mass;
       }
        collision(i);
    }
    if(repulsion) for(i=0;i<num_particles;i++) for(k=0;k<i;k++)
    {
            d2[i][k] = 0.0;
            for(j=0;j<3;j++) d2[i][k]+= (particles[i].position[j]-
              particles[k].position[j])*(particles[i].position[j]-
              particles[k].position[j]);
            d2[k][i]=d2[i][k];
    }
    last_time = present_time;
    glutPostRedisplay();
}

float forces(int i, int j)
{
   int k;
   float force = 0.0;
   if(gravity&&j==1) force = -1.0; /* simple gravity */
   return(force);
}

void collision(int n)

/* tests for collisions against cube and reflect particles if necessary */

{
     int i;
     for (i=0; i<3; i++)
     {
           if(particles[n].position[i]>=35.0)
           {
                particles[n].velocity[i] = -coef*particles[n].velocity[i];
                particles[n].position[i] = 1.0-coef*(particles[n].position[i]-1.0);
           }
        
           if(particles[n].position[i]<=10.0)
           {
//                particles[n].velocity[i] = -coef*particles[n].velocity[i];
//                particles[n].position[i] = -1.0-coef*(particles[n].position[i]+1.0);
           }
     }
}

// Initialization routine.
void setup(void)
{
    
    int  i, j;
/* set up particles with random locations and velocities */

    for(i=0; i<num_particles; i++)
    {
        particles[i].color = i%8;
        if(particles[i].color==1) //red particles have more mass
          particles[i].mass = 1.0;
        else
          particles[i].mass = 1.0;
        
        for(j=0; j<3; j++)
        {
            particles[i].position[j] = 20 *((float) rand()/RAND_MAX)-1.0;
            particles[i].velocity[j] = speed*5.0*((float) rand()/RAND_MAX)-1.0;
        }
    }
    glPointSize(point_size);
    
   glEnable(GL_DEPTH_TEST);
   glClearColor (0.0, 0.0, 0.0, 0.0);
    
    // Texture setting
    // Create texture index array.
    glGenTextures(1, texture);
    
    // Load external texture and generate and load procedural texture.
    loadExternalTextures();

    
    // Turn on OpenGL texturing.
    glEnable(GL_TEXTURE_2D);
    
    // Specify how texture values combine with current surface color values.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

}

void rubikslighting(void) {
    // light property vectors
    if (selection == 0){
        //LOADING SCREEN
        float displaytop[]= { 1.0, 0.5, 1.0, 1.0 }; //white light
        float displaybottom[]= { 1.0, 0.5, 0, 1.0 }; //white light
        float displayback[]= { 1.0, 0.5, 0, 1.0 }; //white light
        
        float lightDif[]= { 1.0, 1.0, 1.0, 1.0 };
        float lightSpec[]= { 1.0, 1.0, 1.0, 1.0 };
        
        // LIGHT POSITIONS
        float lightPos1[]= { 0.0, -15.5, 0.0, 1.0 }; // positioned on bottom
        float lightPos2[]= { 0.0, 0.0, -10.0, 1.0 }; // positioned on the back
        float lightPos3[]= { 0.0, 15.5, 0, 1.0 }; // positioned on the top for loadup menu

        float spotDirect[] = { 0.0, -1.0, 0.0, 1.0}; // direct downwards
        float spotDirect2[] = { 0.0, 0.0, 1.0, 1.0}; // direct towards the cube from orange side (towards me or the screen)
        float spotDirect3[] = { 0.0, 1.0, 0.0, 1.0}; // direct upwards
        
        float spotCutoff = 35.0;
        
        // ambient light
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, displaytop);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, displayback);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, displaybottom);

        // light params
        glLightfv(GL_LIGHT0, GL_AMBIENT, displaytop);
        glLightfv(GL_LIGHT0, GL_AMBIENT, displayback);
        glLightfv(GL_LIGHT0, GL_AMBIENT, displaybottom);
        
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDif);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpec);
        
        // LIGHT POSITIONS and DIRECTION
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos1);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos2);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos3);

        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirect);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirect2);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirect3);

        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotCutoff);
        
        // materials
        float specularity[]={ 1.0,1.0,1.0,1.0 }; // reflectional lighting basically
        float matSpec = 100; // the opacity or strength of the lighting being reflected off the object.. if set to 0, some colors wont show. SET to anything higher than 50 for full effect

        glMaterialfv(GL_FRONT,GL_SPECULAR, specularity); // reflection
        glMateriali(GL_FRONT,GL_SHININESS, matSpec); // brightness/concentration

    }
    

    glEnable(GL_DEPTH_TEST);
    
    // enable changing material color
    glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    
    if (selection == 1 || selection == 2){
        // cone (base of light)
        glColor3f(.98, .5, .45);
        glPushMatrix();
        glTranslated(0, 12, 0);
        glRotatef(-90, 0, 0, 0);
        glutSolidCone(5, 5, 360, 360);
        glPopMatrix();
        
        
        if(lighting){
            
            // light made from sphere
            glColor3f(1, 1, 1);
            glPushMatrix();
            glTranslated(0, 14, 0);
            glutSolidSphere(2, 360, 360);
            glPopMatrix();
            
            // enable lighting
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            
            //spot light when solving cube
            float lightAmb[]= { 1.0, 0.5, 0.0, 1.0 }; //yellow light
            float lightDif[]= { 1.0, 1.0, 1.0, 1.0 };
            float lightSpec[]= { 1.0, 1.0, 1.0, 1.0 };
            float lightPos0[]= { 0.0, 10.0, 0.0, 1.0 }; // positioned on top for in game spotlight
            float spotDirect[] = { 0.0, -1.0, 0.0, 1.0}; // direct downwards
            float spotCutoff = 35.0;
            
            // materials
            float specularity[]={ 1.0,1.0,1.0,1.0 }; // reflectional lighting basically
            float matSpec = 100; // the opacity or strength of the lighting being reflected off the object.. if set to 0, some colors wont show. SET to anything higher than 50 for full effect
            
            glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmb);
            glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
            glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
            
            glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDirect);
            glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spotCutoff);
            glMaterialfv(GL_FRONT,GL_SPECULAR, specularity); // reflection
            glMateriali(GL_FRONT,GL_SHININESS, matSpec); // brightness/concentration
        }
        else{
            glDisable(GL_LIGHT0);
        }
    }
    
}

void rubiksDisplay(void) {
    glDisable(GL_LIGHT0);
     // back piece
    glColor3f(0.2,0.2,0.2);
    glPushMatrix();
    glTranslatef(0, 0, -10);
    glScalef(3, 12, 1);
    glutSolidCube(3);
    glPopMatrix();
    
    //top piece
    glPushMatrix();
    glTranslatef(0, 16.5, 0);
    glScalef(3, 1, 7);
    glutSolidCube(3);
    glPopMatrix();
    
    //bottom piece
    glPushMatrix();
    glTranslatef(0, -16.5, 0);
    glScalef(3, 1, 7);
    glutSolidCube(3);
    glPopMatrix();
    glEnable(GL_LIGHT0);
    
    // spotlight pieces
    // back piece
   glColor3f(1.0,1.0,1.0);
   glPushMatrix();
   glTranslatef(0, .5, -9);
   glScalef(3, 12, 1);
   glutSolidCube(2);
   glPopMatrix();
    
    //top piece
    glPushMatrix();
    glTranslatef(0, 15.5, 0);
    glScalef(3, 1, 7);
    glutSolidCube(2);
    glPopMatrix();
    
    //bottom piece
    glPushMatrix();
    glTranslatef(0, -15.5, 0);
    glScalef(3, 1, 7);
    glutSolidCube(2);
    glPopMatrix();
    
    glEnable(GL_LIGHT0);
    
}

void rubikscube(void){
    /* ******************************** */
     //MIDDLE COLUMN
     
     mid_top_front();
     mid_top_mid();
     mid_top_back();

     mid_front_mid();
     mid_mid_mid();
     mid_mid_back();
     
     mid_bot_front();
     mid_bot_mid();
     mid_bot_back();

     
     /* ******************************** */
     // LEFT COLUMN
     
     left_top_front();
     left_top_mid();
     left_top_back();

     left_mid_front();
     left_mid_mid();
     left_mid_back();

     left_bot_front();
     left_bot_mid();
     left_bot_back();
     
     
     
     /* ******************************** */
     // RIGHT COLUMN
     
     right_top_front();
     right_top_mid();
     right_top_back();

     right_mid_front();
     right_mid_mid();
     right_mid_back();

     right_bot_front();
     right_bot_mid();
     right_bot_back();
}

// Drawing routine.
void drawScene(void)
{
    
    int i, j;
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Begin left viewport.
   glViewport (0, 0, width/2.0,  height);
   glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    
    TitleText();
    startText();
    SolvedText();
    exitText();
    
    
   // Fixed camera.
   gluLookAt(0.0, 10.0, 20.0,
             0.0, 0.0, 0.0,
             0.0, 1.0, 0.0);
   
    
   // End left viewport.
// *****************************************************************************************************************
    
   // Begin right viewport.
   glViewport(width/2.0, 0, width/2.0, height);
   glLoadIdentity();
    
   // Draw a vertical line on the left of the viewport to separate the two viewports
   glColor3f(1.0, 1.0, 1.0);
   glLineWidth(2.0);
   glBegin(GL_LINES);
      glVertex3f(-5.0, -5.0, -5.0);
      glVertex3f(-5.0, 5.0, -5.0);
   glEnd();
   glLineWidth(1.0);


     // Fixed camera.
    gluLookAt(0.0, 10.0, 20.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    
    glRotatef(rotateX, 1.0, 0.0, 0.0); // rotate in y axis
    glRotatef(rotateY, 0.0, 1.0, 0.0); // rotate in x axis
    
    
    // launch CASE
    if (selection == 0){
         rubiksDisplay();
         rubikslighting();
        
        glPushMatrix();
        glRotatef(angle, 1, 1, 0);
         rubikscube();
         logo();
        glPopMatrix();
        angle += 1;
         
    }
    
    
    // START CASE - radomizer and remove the rubiks cube display
    if (selection == 1){
        rubikslighting();
        rubikscube();
        logo();
        
    }
    else{
        
    }
    
    // solved CASE
    if (selection == 2){
        glBegin(GL_POINTS); /* render all particles */
        for(i=0; i<num_particles; i++)
        {
           glColor3fv(colors[particles[i].color]);
           glVertex3fv(particles[i].position);
        }
        glEnd();
        
        rubikslighting();
        logo();
        rubikscube();
        
    }
    else {
        
    }
    
   // End right viewport.
   glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport (0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 250.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Pass the size of the OpenGL window.
   width = w;
   height = h;
}

// Mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
    int xWorld, yWorld;
    xWorld = x * 1.0/5;
    yWorld = (500-y) * 1.0/5;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        glRasterPos3f(-28.0, 15.0, -30.0);

        if (yWorld>=75 && yWorld<=85 && xWorld>=5 && xWorld<=15)
        {
            cout<< "Start button pressed" << endl;
            selection = 1;
            
        }
        else if(yWorld>=60 && yWorld<=70 && xWorld>=5 && xWorld<=15)
        {
            cout<< "Solved button pressed" << endl;
            selection = 2;
            gravity = !gravity;
            setup();
        }
        else if(yWorld>=50 && yWorld<=59 && xWorld>=5 && xWorld<=15)
        {
            exit(0);
            cout<< "EXIT Button Pressed" << endl;
        }
    }


    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    }
   
   glutPostRedisplay();
}
// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key)
   {
      case 27:
         exit(0);
         break;
       case '1': // To start game
           selection = 1;
           glutPostRedisplay();
           break;
       case '2': // To celebrate solving the cube
           selection = 2;
           gravity = !gravity;
           setup();
           glutPostRedisplay();
           break;
       case '3': // to Exit game
           exit(0);
           glutPostRedisplay();
           break;
       case 'w': // Rotate front face vertically (Over the X axis)
           frontrotate = !frontrotate;
           glutPostRedisplay();
           break;
       case 'o':
           selection == 0;
           displaylight = !displaylight;
           rubikslighting();
           glutPostRedisplay();
           break;
       case 'l':
       case 'L':
           lighting = !lighting;
           cout << "lighting enabled" << endl;
           rubikslighting();
           break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
    int increment = 5;

    switch(key){

        case GLUT_KEY_UP: // up
            rotateX = (rotateX - increment) % 360;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN: //down
            rotateX = (rotateX + increment) % 360;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT: // right
            rotateY = (rotateY - increment) % 360;
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT: //left
            rotateY = (rotateY + increment) % 360;
            glutPostRedisplay();
            break;
    }//end switch
    
    glutPostRedisplay();
}

void main_menu(int index)
{
    switch(index)
    {
        case(1):
        {
            // start game
            selection = 1;
            break;
        }
        case(2):
        {
            selection = 2;
            gravity = !gravity;
            setup();
            break;
        }
        case(3):
        {
            selection = 0;
            break;
        case(5):
            {
                exit(0);
                break;
            }
        }
    }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
    
   cout << "Interaction:" << endl;
   cout << " 1 : Start the game (changes scenary from launch mode to game mode)" << endl
        << " 2 : Celebration (fireworks) appear once this button is clicked." << endl
        << "  3: Closes the game " << endl
        << " L and l (lowercase L) : turns spotlight on and off. (it starts with all spotlights being on) " << endl
        << " ↑ : moves the world up to view under the the cube " << endl
        << " ↓ : moves the world down to view the top of the cube " << endl
        << " →: moves the world right to view the right side of the cube " << endl
        << " ←: moves the world left to view the left side of the cube " << endl
        << "  CLICKING START ON VIEWPORT 1 (LEFT SIDE)  the game (changes scenary from launch mode to game mode)" << endl
        << "  CLICKING SOLVED ON VIEWPORT 1 (LEFT SIDE) Celebration (fireworks) appear once this button is clicked" << endl
        << "  CLICKING EXIT ON VIEWPORT 1 (LEFT SIDE) Closes the game" << endl
        << " (RIGHT CLICK FOR MENU:) " << endl
        << "  Start" << endl
        << " Solved" << endl
        << "  Load Screen" << endl
        << " Quit Game" << endl;
}

// Main routine.
int main(int argc, char **argv)
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(800, 400);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("RUBIKS CUBE SIMULATOR.cpp");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   // Register the mouse callback function.
   glutMouseFunc(mouseControl);
    
    glutCreateMenu(main_menu);
    glutAddMenuEntry("START", 1);
    glutAddMenuEntry("SOLVED", 2);
    glutAddMenuEntry("LOAD SCREEN", 3);
    glutAddMenuEntry("QUIT GAME",5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    glutIdleFunc(myIdle);
    
    // glutIdleFunc(drawScene);

   glutMainLoop();

   return 0;
}
