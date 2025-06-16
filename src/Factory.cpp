//  ========================================================================
//  COSC363: Computer Graphics 
//  FILE NAME: Factory.cpp
//  See Project.pdf for details
//
//  Program displays the model of a Factory (mesh data from Factory.off,Factory.off)
//  Use left/right arrow keys to rotate the scene
//  Use up/down arrow keys to move camera up/down
//  ========================================================================
 
#include <iostream>
#include <fstream>
#include <climits>
#include <cmath>
#include <list>

#include <GL/freeglut.h>
#include <vector>
#include <GL/glut.h>
#include "loadBMP.h"
#include "loadTGA.h"

//------------------------------- Globals ------------------------------------
float angle = 13., cam_hgt = 20., eye_x = -3., eye_z = 10., look_x = 0., look_z = -2.;
GLuint txId[10];
#define GL_CLAMP_TO_EDGE 0x812F   

int tick = 0;		//Timer counter

struct particle {	//A particle 
    int t;			//Life time  (0 - 200)
    float col;		//Color  (0 - 1)
    float size;		//Size   (5 - 25)
    float pos[3];	//Position
    float vel[3];	//Velocity
};

list<particle> parList1;  // For the first chimney
list<particle> parList2;  // For the second chimney
list<particle> parList3;  // For the third chimney

GLenum polygonMode = GL_FILL;

//--------------------------------------------------------------------------

void loadTexture() {
    glGenTextures(10, txId);   //Get 2 texture IDs 

    glBindTexture(GL_TEXTURE_2D, txId[0]);  
   loadBMP("rusty.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, txId[1]); 
    loadBMP("green.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, txId[8]);
    loadBMP("white.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glBindTexture(GL_TEXTURE_2D, txId[9]);
    loadBMP("brick.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
 
	////////////////////BOX TEXTURES////////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[2]);	
    loadTGA("Back.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, txId[3]);	
    loadTGA("Front.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[4]);	
    loadTGA("Left.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[5]);	
    loadTGA("Right.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[6]);	
    loadTGA("Up.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    ////////////////////BOX TEXTURES////////////////////////
  

    glGenTextures(1, &txId[7]);
    glBindTexture(GL_TEXTURE_2D, txId[7]);
    loadBMP("Glow.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}



void walls() {
    glEnable(GL_TEXTURE_2D);
    glColor3f(1., 1., 1.);
    /////////////////// BACK FACE////////////////////
    glBindTexture(GL_TEXTURE_2D, txId[2]); // Bind the texture for the back face
    glBegin(GL_QUADS);
    glTexCoord2f(0., 0.); glVertex3f(30., 30., -35.);  // Top-left
    glTexCoord2f(0., 1.); glVertex3f(30., 0., -35.);   // Bottom-left
    glTexCoord2f(1., 1.); glVertex3f(-30., 0., -35.);  // Bottom-right
    glTexCoord2f(1., 0.); glVertex3f(-30., 30., -35.); // Top-right 
    glEnd();

    ///////////////// FRONT FACE///////////
    glBindTexture(GL_TEXTURE_2D, txId[3]); // Bind the texture for the front 
    glBegin(GL_QUADS);
    glTexCoord2f(0., 0.); glVertex3f(-30., 30., 20.);  // Top-left
    glTexCoord2f(0., 1.); glVertex3f(-30., 0., 20.);   // Bottom-left
    glTexCoord2f(1., 1.); glVertex3f(30., 0., 20.);    // Bottom-right
    glTexCoord2f(1., 0.); glVertex3f(30., 30., 20.);   // Top-right
    glEnd();

    ////////////////// LEFT FACE/////////////////
    glBindTexture(GL_TEXTURE_2D, txId[4]); // Bind the texture for the left 
    glBegin(GL_QUADS);
    glTexCoord2f(0., 0.); glVertex3f(-30., 30., -35.); // Top-left
    glTexCoord2f(0., 1.); glVertex3f(-30., 0., -35.);  // Bottom-left
    glTexCoord2f(1., 1.); glVertex3f(-30., 0., 20.);   // Bottom-right
    glTexCoord2f(1., 0.); glVertex3f(-30., 30., 20.);  // Top-right
    glEnd();

    /////////////////// RIGHT FACE////////////////
    glBindTexture(GL_TEXTURE_2D, txId[5]); // Bind the texture for the right 
    glBegin(GL_QUADS);
    glTexCoord2f(0., 0.); glVertex3f(30., 30., 20.);   // Top-left 
    glTexCoord2f(0., 1.); glVertex3f(30., 0., 20.);    // Bottom-left 
    glTexCoord2f(1., 1.); glVertex3f(30., 0., -35.);   // Bottom-right 
    glTexCoord2f(1., 0.); glVertex3f(30., 30., -35.);  // Top-right 
    glEnd();

    //////////////////// TOP FACE//////////////
    glBindTexture(GL_TEXTURE_2D, txId[6]); // Bind the texture for the top 
    glBegin(GL_QUADS);
    glTexCoord2f(0., 1.); glVertex3f(-30., 30., -35.); // Bottom-left
    glTexCoord2f(1., 1.); glVertex3f(30., 30., -35.);  // Bottom-right
    glTexCoord2f(1., 0.); glVertex3f(30., 30., 20.);   // Top-right
    glTexCoord2f(0., 0.); glVertex3f(-30., 30., 20.);  // Top-left
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void floor() {
    bool flag = false;
    glBegin(GL_QUADS);
    glNormal3f(0., 1., 0.);
    for (int x = -100; x <= 100; x += 1) {
        for (int z = -100; z <= 100; z += 1) {
            if (flag)  glColor3f(0.96, 0.96, 0.86);
            else 	  glColor3f(0.5, 0.5, 0.5);
            glVertex3f(x, -0.1, z);
            glVertex3f(x, -0.1, z + 1.);
            glVertex3f(x + 1., -0.1, z + 1.);
            glVertex3f(x + 1., -0.1, z);
            flag = !flag;
        }
    }
    glEnd();
}


//--------------------------- Draws entity-----------------------------------------

void drawRustyDoor() {
    glPushMatrix();
    glTranslatef(15., 5., -9.7);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[0]);
    glBegin(GL_QUADS); // Front 
    glTexCoord2f(0., 0.); glVertex3f(-2., -5., 0.5);
    glTexCoord2f(1., 0.); glVertex3f(2., -5., 0.5);
    glTexCoord2f(1., 1.); glVertex3f(2., 5., 0.5);
    glTexCoord2f(0., 1.); glVertex3f(-2., 5., 0.5);
    glEnd();
    glBegin(GL_QUADS); // Back
    glTexCoord2f(0., 0.); glVertex3f(-2., -5., -0.5);
    glTexCoord2f(1., 0.); glVertex3f(2., -5., -0.5);
    glTexCoord2f(1., 1.); glVertex3f(2., 5., -0.5);
    glTexCoord2f(0., 1.); glVertex3f(-2., 5., -0.5);
    glEnd();
    glBegin(GL_QUADS); // Left
    glTexCoord2f(0., 0.); glVertex3f(-2., -5., -0.5);
    glTexCoord2f(1., 0.); glVertex3f(-2., -5., 0.5);
    glTexCoord2f(1., 1.); glVertex3f(-2., 5., 0.5);
    glTexCoord2f(0., 1.); glVertex3f(-2., 5., -0.5);
    glEnd();
    glBegin(GL_QUADS); // Right
    glTexCoord2f(0., 0.); glVertex3f(2., -5., -0.5);
    glTexCoord2f(1., 0.); glVertex3f(2., -5., 0.5);
    glTexCoord2f(1., 1.); glVertex3f(2., 5., 0.5);
    glTexCoord2f(0., 1.); glVertex3f(2., 5., -0.5);
    glEnd();
    glBegin(GL_QUADS); // Top 
    glTexCoord2f(0., 0.); glVertex3f(-2., 5., -0.5);
    glTexCoord2f(1., 0.); glVertex3f(2., 5., -0.5);
    glTexCoord2f(1., 1.); glVertex3f(2., 5., 0.5);
    glTexCoord2f(0., 1.); glVertex3f(-2., 5., 0.5);
    glEnd();
    glBegin(GL_QUADS); // Bottom
    glTexCoord2f(0., 0.); glVertex3f(-2., -5., -0.5);
    glTexCoord2f(1., 0.); glVertex3f(2., -5., -0.5);
    glTexCoord2f(1., 1.); glVertex3f(2., -5., 0.5);
    glTexCoord2f(0., 1.); glVertex3f(-2., -5., 0.5);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawRedDoor() {
    glPushMatrix();
    glTranslatef(15., 5., 8.7);
    glScalef(4., 10., 1.);
    glutSolidCube(1.);
    glPopMatrix();
}

void drawGreenDoor() {
    glPushMatrix();
    glTranslatef(-15., 5., 0.);
    glRotatef(90., 0., 1., 0.);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[1]);
    glBegin(GL_QUADS); // Front 
    glTexCoord2f(0., 0.); glVertex3f(-2., -5., 0.5);
    glTexCoord2f(1., 0.); glVertex3f(2., -5., 0.5);
    glTexCoord2f(1., 1.); glVertex3f(2., 5., 0.5);
    glTexCoord2f(0., 1.); glVertex3f(-2., 5., 0.5);
    glEnd();
    glBegin(GL_QUADS); // Back
    glTexCoord2f(0., 0.); glVertex3f(-2., -5., -0.5);
    glTexCoord2f(1., 0.); glVertex3f(2., -5., -0.5);
    glTexCoord2f(1., 1.); glVertex3f(2., 5., -0.5);
    glTexCoord2f(0., 1.); glVertex3f(-2., 5., -0.5);
    glEnd();
    glBegin(GL_QUADS); // Left
    glTexCoord2f(0., 0.); glVertex3f(-2., -5., -0.5);
    glTexCoord2f(1., 0.); glVertex3f(-2., -5., 0.5);
    glTexCoord2f(1., 1.); glVertex3f(-2., 5., 0.5);
    glTexCoord2f(0., 1.); glVertex3f(-2., 5., -0.5);
    glEnd();
    glBegin(GL_QUADS); // Right
    glTexCoord2f(0., 0.); glVertex3f(2., -5., -0.5);
    glTexCoord2f(1., 0.); glVertex3f(2., -5., 0.5);
    glTexCoord2f(1., 1.); glVertex3f(2., 5., 0.5);
    glTexCoord2f(0., 1.); glVertex3f(2., 5., -0.5);
    glEnd();
    glBegin(GL_QUADS); // Top 
    glTexCoord2f(0., 0.); glVertex3f(-2., 5., -0.5);
    glTexCoord2f(1., 0.); glVertex3f(2., 5., -0.5);
    glTexCoord2f(1., 1.); glVertex3f(2., 5., 0.5);
    glTexCoord2f(0., 1.); glVertex3f(-2., 5., 0.5);
    glEnd();
    glBegin(GL_QUADS); // Bottom
    glTexCoord2f(0., 0.); glVertex3f(-2., -5., -0.5);
    glTexCoord2f(1., 0.); glVertex3f(2., -5., -0.5);
    glTexCoord2f(1., 1.); glVertex3f(2., -5., 0.5);
    glTexCoord2f(0., 1.); glVertex3f(-2., -5., 0.5);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void drawInterPointBox() {
    glPushMatrix();
    glTranslatef(14., 2.1, 1.);
    glRotatef(90., 0., 1., 0.);
    glScalef(6., 5., 6.);
    glutSolidCube(1.);
    glPopMatrix();
}



float hammerPosition = 1.5;
bool hammerDown = false;
void drawHammer() {
    glPushMatrix();
    glTranslatef(-5., hammerPosition, 5.);
    glColor3f(0.6, 0.6, 0.6);
    // Vertical part
    glPushMatrix();
    glScalef(1.5, 15.0, 0.5);
    glutSolidCube(1.);
    glPopMatrix();
    // -- PART ,
    glTranslatef(0., 7., -1.);
    glScalef(2.5, 1.5, 3.0);
    glutSolidCube(1.);
    glPopMatrix();
}


void drawSprinkler() {
    glPushMatrix();
    glTranslatef(5., 0., 5.);
    glColor3f(0., 0., 1.);
    // Vertical PART
    glPushMatrix();
    glScalef(0.5, 11., 0.5);
    glutSolidCube(1.);
    glPopMatrix();
    // -- PART
    glTranslatef(0., 5.5, -0.5);
    glScalef(1.5, 0.5, 1.5);
    glutSolidCube(1.);
    glPopMatrix();
}

//--------------------------------------------------------------------------


//---------------------------Conveyor Belt-----------------------------------------

float xpts[13] = { -15.0, -12.5, -10.0, -7.5, -5.0, -2.5, 0.0, 2.5, 5.0, 7.5, 10.0, 12.5, 15.0 };
float zpts[13] = { 0.0, 1.0, 1.8, 2.5, 3.2, 3.8, 4.0, 3.8, 3.2, 2.5, 1.8, 1.0, 0.0 };
//-- Function to compute normal vectors at vertices of a polygonal line ----
void normal(int i) {
    float xdiff1, zdiff1, xdiff2, zdiff2;
    if (i == 0 || i == 12) glNormal3f(-1., 0., 0.);   
    else {
        xdiff1 = xpts[i] - xpts[i - 1];
        zdiff1 = zpts[i] - zpts[i - 1];
        xdiff2 = xpts[i + 1] - xpts[i];
        zdiff2 = zpts[i + 1] - zpts[i];
        glNormal3f(0., -(zdiff1 + zdiff2), (xdiff1 + xdiff2));
    }
}


void drawConveyorBeltBottom() {
    int N = 13;
    float width = 3.0f;
    float height = 3.0f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[8]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
    // Bottom
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < N; i++) {
        normal(i);
        glTexCoord2f(i / (float)(N - 1), 0.);
        glVertex3f(xpts[i], 0.0f, zpts[i] - width / 2.);

        glTexCoord2f(i / (float)(N - 1), 1.);
        glVertex3f(xpts[i], 0.0f, zpts[i] + width / 2.);
    }
    glEnd();

    // Top 
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < N; i++) {
        normal(i);
        glTexCoord2f(i / (float)(N - 1), 0.);
        glVertex3f(xpts[i], height, zpts[i] - width / 2.);

        glTexCoord2f(i / (float)(N - 1), 1.);
        glVertex3f(xpts[i], height, zpts[i] + width / 2.);
    }
    glEnd();

    // Left 
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < N - 1; i++) {
        normal(i);
        glTexCoord2f(i / (float)(N - 1), 0.);
        glVertex3f(xpts[i], 0., zpts[i] - width / 2.);

        glTexCoord2f((i + 1) / (float)(N - 1), 0.);
        glVertex3f(xpts[i + 1], 0., zpts[i + 1] - width / 2.);

        glTexCoord2f(i / (float)(N - 1), 1.);
        glVertex3f(xpts[i], height, zpts[i] - width / 2.);

        glTexCoord2f((i + 1) / (float)(N - 1), 1.);
        glVertex3f(xpts[i + 1], height, zpts[i + 1] - width / 2.);
    }
    glEnd();

    //Right
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < N - 1; i++) {
        normal(i);
        glTexCoord2f(i / (float)(N - 1), 0.);
        glVertex3f(xpts[i], 0., zpts[i] + width / 2.);

        glTexCoord2f((i + 1) / (float)(N - 1), 0.);
        glVertex3f(xpts[i + 1], 0., zpts[i + 1] + width / 2.0f);

        glTexCoord2f(i / (float)(N - 1), 1.);
        glVertex3f(xpts[i], height, zpts[i] + width / 2.);

        glTexCoord2f((i + 1) / (float)(N - 1), 1.);
        glVertex3f(xpts[i + 1], height, zpts[i + 1] + width / 2.);
    }
    glEnd();

    // Y-axies
    for (int i = 0; i < 150; ++i) {
        glPushMatrix();
        glTranslatef(15., 2., -10. + i * 0.125);
        glScalef(2., 2., 0.125);
        glutSolidCube(1.);
        glPopMatrix();
    }


    glDisable(GL_TEXTURE_2D);
}

const int numBricks = 50;
float brick_positions_h[numBricks];
float brick_positions_v[numBricks];
void initializeBricks() {
    for (int i = 0; i < numBricks; ++i) {
        if (i < 29) {
            //x
            brick_positions_h[i] = -15. + i;
        }
        //y
        else {
            brick_positions_v[i] = -9. + (i - 30);
        }
    }
}

// func to transe the positions followed the curve
// used by items1 and belt bricks
float interpolate(float x, const float* xArray, const float* zArray, int size) {
    for (int i = 0; i < size - 1; ++i) {
        if (x >= xArray[i] && x <= xArray[i + 1]) {
            float t = (x - xArray[i]) / (xArray[i + 1] - xArray[i]);
            return zArray[i] + t * (zArray[i + 1] - zArray[i]);
        }
    }
}

void drawConveyorBeltBricks() {
    for (int i = 0; i < numBricks; ++i) {
        if (i < 29) {
            //x
            float zPosition = interpolate(brick_positions_h[i], xpts, zpts, 13);
            glPushMatrix();
            glTranslatef(brick_positions_h[i], 3.5, zPosition);
            glScalef(0.9, 0.2, 2.);
            glColor3f(0.55, 0.27, 0.07);
            glutSolidCube(1.);
            glPopMatrix();
        }
        else {
            //y
            glPushMatrix();
            glTranslatef(15., 3.5, brick_positions_v[i]);
            glScalef(2., 0.2, 0.9);
            glColor3f(0.55, 0.27, 0.07);
            glutSolidCube(1.);
            glPopMatrix();
        }
    }
}
//--------------------------------------------------------------------------


//--------------------------- Draws items-----------------------------------------
enum ItemColor { RED, BLACK, WHITE }; //color state
ItemColor Items1currentColor = RED;
const int numItems1 = 5;
float item_positions1[numItems1];
float item1Speed = 0.025;
ItemColor item1Colors[numItems1];

//x item == Items1
void initializeItems1() {
    //initial positions
    for (int i = 0; i < numItems1; ++i) {
        item_positions1[i] = -40. + i * 5.5;
    }
}

void drawItem1() {
    for (int i = 0; i < numItems1; ++i) {
        if (item_positions1[i] > -15.) {
            float zPosition = interpolate(item_positions1[i], xpts, zpts, 13);
            glPushMatrix();
            glTranslatef(item_positions1[i], 4., zPosition);
            switch (item1Colors[i]) {
            case RED:
                glColor3f(1., 0., 0.); // Red
                break;
            case BLACK:
                glColor3f(0., 0., 0.); // Black
                break;
            case WHITE:
                glColor3f(1., 1., 1.); // White
                break;
            }
            glutSolidSphere(0.5, 20., 20.);
            glPopMatrix();
        }
    }
}

const int numItems2 = 4;
float item_positions2[numItems2];
float item2Speed = 0.018;

//y item == Items2
void initializeItems2() {
    // initial positions
    for (int i = 0; i < numItems2; ++i) {
        item_positions2[i] = -30. + i * 4.7;
    }
}

void drawItem2() {
    glColor3f(0., 1., 1.);
    for (int i = 0; i < numItems2; ++i) {
        if (item_positions2[i] > -9.) {
            glPushMatrix();
            glTranslatef(15., 4., item_positions2[i]); // Move along the z-axis

            if (item_positions2[i] > 0.) {
                glutSolidTeapot(0.5); // change to teapot
            }
            else {
                glutSolidSphere(0.5, 20., 20.);
            }
            glPopMatrix();
        }
    }
}

void drawChimney(float x, float y, float z, float scale) {
    //---------------------init positions--------------------
    const int N = 15;
    const int nSlices = 36; 
    float vx_init[N] = { 2., 2., 2., 2., 1.9, 1.6, 1.5, 1.4, 1.3, 1., 0.95, 0.9 ,0.8, 0.7,0.6}; // x-coordinates of the profile
    float vy_init[N] = { 0., 1., 2., 3., 4., 4.5,5., 5.5, 6., 7., 8., 9., 10., 11., 12. }; // y-coordinates of the profile
    float nx_init[N] = { 0., 0., 0., 0., 0.2, 0.3, 0.3, 0.3, 0.3, 0.5, 0.1, 0.2, 0.2, 0.2, 0.2 };
    float ny_init[N] = { 0., 0., 0., 0., 0.2, 0.3, 0.3, 0.3, 0.3, 0.5, 0.1, 0.2, 0.2, 0.2, 0.2 };
    float vx[N], vy[N], vz[N];	//vertex 
    float wx[N], wy[N], wz[N];
    float nx[N], ny[N], nz[N];	//normal vectors
    float mx[N], my[N], mz[N];
    float angStep = 2. * 3.14 / nSlices; 
    //--------------------Initialize data--------------------
    for (int i = 0; i < N; i++) {	
        vx[i] = vx_init[i];
        vy[i] = vy_init[i];
        vz[i] = 0.;
        nx[i] = nx_init[i];
        ny[i] = ny_init[i];
        nz[i] = 0.;
    }
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[9]);

    //--------------------Draw the chimney in --------------------

    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(scale, scale, scale);

    for (int j = 0; j < nSlices; j++) { 
        for (int i = 0; i < N; i++) {
            wx[i] = cos(angStep) * vx[i] + sin(angStep) * vz[i];
            wy[i] = vy[i];
            wz[i] = -sin(angStep) * vx[i] + cos(angStep) * vz[i];
            mx[i] = cos(angStep) * nx[i] + sin(angStep) * nz[i];
            my[i] = ny[i];
            mz[i] = -sin(angStep) * nx[i] + cos(angStep) * nz[i];
        }
        glBegin(GL_QUAD_STRIP); 

        for (int i = 0; i < N; i++) {


            glNormal3f(nx[i], ny[i], nz[i]);
            glTexCoord2f((float)j / nSlices, (float)i / (N - 1));
            glVertex3f(vx[i], vy[i], vz[i]);

            glNormal3f(mx[i], my[i], mz[i]);
            glTexCoord2f((float)(j + 1) / nSlices, (float)i / (N - 1));
            glVertex3f(wx[i], wy[i], wz[i]);
        }
        glEnd();

        for (int i = 0; i < N; i++) { 
            vx[i] = wx[i]; 
            vy[i] = wy[i]; 
            vz[i] = wz[i];
            nx[i] = mx[i]; 
            ny[i] = my[i]; 
            nz[i] = mz[i];
        }
    }
    glPopMatrix();
}
//-------------------------------------------------------------------------------

//--------------------------- Draws a smoke -----------------------------------------

void drawParticle(float col, float size, float px, float py, float pz) {
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glColor3f(col, col, col);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[7]);

    glPushMatrix();
    glTranslatef(px, py, pz);
    glScalef(size, size, size);

    glBegin(GL_QUADS);
    //xy-plane
    glTexCoord2f(0., 0.);
    glVertex3f(-0.5, -0.5, 0.);
    glTexCoord2f(1., 0.);
    glVertex3f(0.5, -0.5, 0.);
    glTexCoord2f(1., 1.);
    glVertex3f(0.5, 0.5, 0.);
    glTexCoord2f(0., 1.);
    glVertex3f(-0.5, 0.5, 0.);

    //yz-plane
    glTexCoord2f(0., 0.);
    glVertex3f(0., -0.5, -0.5);
    glTexCoord2f(1., 0.);
    glVertex3f(0., 0.5, -0.5);
    glTexCoord2f(1., 1.);
    glVertex3f(0., 0.5, 0.5);
    glTexCoord2f(0., 1.);
    glVertex3f(0., -0.5, 0.5);
    glEnd();
    glPopMatrix();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
}

void newParticle(list<particle>& parList, float x, float y, float z) {
    particle p = { 0 };

    float i = 0.5 * (rand() / (float)RAND_MAX) - 0.25;
    float v = 0.01* (rand() / (float)RAND_MAX) - 0.01;

    p.pos[0] = x + i;
    p.pos[1] = y;  // This point is at the top end of the smoke stack
    p.pos[2] = z + i;

    p.vel[0] = v;
    p.vel[1] = 0.06;
    p.vel[2] = v;

    p.col = 1;
    p.size = 2;

    parList.push_back(p);
}

void updateQueue(list<particle>& parList, float x, float y, float z) {
    const int LIFETIME =200;
    list<particle>::iterator it;
    particle p;
    int tval;
    float delta;

    if (!parList.empty()) {
        p = parList.front();
        if (p.t > LIFETIME) parList.pop_front();
    }

    for (it = parList.begin(); it != parList.end(); it++) {
        tval = it->t;
        it->t = tval + 1;
        delta = (float)tval / (float)LIFETIME;

        it->pos[0] += it->vel[0];
        it->pos[1] += it->vel[1];
        it->pos[2] += it->vel[2];

        it->size = delta * 0.1 +1.5;  
        it->col = 1 - delta;        
    }

    if (tick % 5== 0) newParticle(parList, x, y, z);  
}

//--------------------------------------------------------------------------




//------------------------------Spotlight--------------------------------------------
float spotlightAngle = 0.0; // Angle for spotlight rotation
float spotLight[] = { 15, 10, 3, 1 };
float spotDir[] = { -1,-1,0.3 };

void drawSpotlight() {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE); // Enable texturing for the quadric
    glPushMatrix();
        glTranslatef(15., 5., 0.); 
        glRotatef(spotlightAngle, 0., 1., 0.); 
        glColor3f(1., 1., 0.); 
        gluCylinder(quad, 0.3, 0.3, 1.5, 30., 30.); 
   
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir);
        glLightfv(GL_LIGHT1, GL_POSITION, spotLight);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(15., 5., 0.); // Position the spotlight at the corner of the belt
        glRotatef(90., 1., 0., 0.); // Rotate around the y-axis
        glColor3f(1., 1., 0.); // Set color to yellow for the spotlight
        gluCylinder(quad, 0.2, 0.2, 0.5, 30., 30.); // Draw the cylinder
    glPopMatrix();
    gluDeleteQuadric(quad);
}

//--------------------------------------------------------------------------







//------------------------------timer----------------------------------------
void timer(int value) {
    //brick animation
    for (float& brickposition : brick_positions_h) {
        brickposition += item1Speed;
        if (brickposition > 14.) {
            brickposition = -15.; // Reset to the black door
        }
    }
    for (float& brickposition : brick_positions_v) {
        brickposition += item2Speed;
        if (brickposition > 8.5) {
            brickposition = -9.9; // Resetto the fire door
        }
    }
    //items1 animation 
    for (int i = 0; i < numItems1; ++i) {
        item_positions1[i] += item1Speed;
        if (item_positions1[i] > 13.) {
            item_positions1[i] = -15.; 
            item1Colors[i] = RED;
        }
        if (item_positions1[i] > -6.5 && item_positions1[i] < -5.5) {
            hammerDown = true; // hammer down
        }
        else if (item_positions1[i] > -5. && item_positions1[i] <= 5.) {
            item1Colors[i] = BLACK;
        }
        else if (item_positions1[i] > 5. && item_positions1[i] <= 10.) {
            item1Colors[i] = WHITE;
        }

    }
    //items2 animation
    for (int i = 0; i < numItems2; ++i) {
        item_positions2[i] += item2Speed;
        if (item_positions2[i] > 8.5) {
            item_positions2[i] = -10.5;
        }
    }


    //hammer animation
    if (hammerDown) {
        hammerPosition -= 0.1; // Move down
        if (hammerPosition <= 1.5) {
            hammerDown = false; // Reset 
        }
    }
    else {
        if (hammerPosition <= 1.5) {
            hammerPosition += 0.1; // Move up
        }
    }
    //spotlight animation
    spotlightAngle += 1; 
    if (spotlightAngle >= 360) {
        spotlightAngle -= 360; 
    }

    tick++;
    if (tick == INT_MAX) tick = 0;

    float chimney1Pos[3] = { -8., 10.5, -12. };  
    float chimney2Pos[3] = { 1., 7.5, -15.};  
    float chimney3Pos[3] = { 13., 10.5, -12. };

    updateQueue(parList1, chimney1Pos[0], chimney1Pos[1], chimney1Pos[2]);
    updateQueue(parList2, chimney2Pos[0], chimney2Pos[1], chimney2Pos[2]);
    updateQueue(parList3, chimney3Pos[0], chimney3Pos[1], chimney3Pos[2]);
	
	glutTimerFunc(16, timer, 0); 
    glutPostRedisplay();
}




//--------------------------------------------------------------------------

void display() {
    float lpos[4] = { 10.,20., 5., 1. };	//light's position
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_x, cam_hgt, eye_z, look_x, 6., look_z, 0., 1., 0.);
   
    glRotatef(angle, 0., 1., 0.);

    glLightfv(GL_LIGHT0, GL_POSITION, lpos);

 
    float shadowMat[16] = {
        lpos[1], 0, 0, 0,
        -lpos[0], 0, -lpos[2], -1,
        0, 0, lpos[1], 0,
        0, 0, 0, lpos[1]
    };
 
    glEnable(GL_LIGHTING);

    walls();
    floor();
    
    drawItem1();
    drawItem2();
    drawConveyorBeltBricks();
    drawSprinkler();
    drawHammer();
    drawSpotlight();


    glColor3f(1., 0., 0.);
    drawRedDoor();

    glColor3f(1., 0.5, 0.);
    drawInterPointBox();

    glColor3f(1., 1., 1.);
    drawConveyorBeltBottom();
    drawRustyDoor();
    drawGreenDoor();
    drawChimney(-7., 0., -7., 1);
    drawChimney(0., 0., -10., 0.7);
    drawChimney(7., 0., -5., 1);

    glDisable(GL_LIGHTING);
    glPushMatrix();
    glMultMatrixf(shadowMat);
    glColor4f(0., 0., 0., 1.);
    glColor3f(0., 0., 0.);
    drawChimney(-7., 0., -7., 1);
    drawChimney(0., 0., -10., 0.7);
    drawChimney(7., 0., -7., 1);
    drawInterPointBox();
    drawConveyorBeltBottom();
    drawRedDoor();
    drawRustyDoor();
    drawGreenDoor();
    glPopMatrix();
    glEnable(GL_LIGHTING);
   
  



    list<particle>::iterator it;
    for (it = parList1.begin(); it != parList1.end(); it++) {
        drawParticle(it->col, it->size, it->pos[0], it->pos[1], it->pos[2]);
    }
    for (it = parList2.begin(); it != parList2.end(); it++) {
        drawParticle(it->col, it->size, it->pos[0], it->pos[1], it->pos[2]);
    }
    for (it = parList3.begin(); it != parList3.end(); it++) {
        drawParticle(it->col, it->size, it->pos[0], it->pos[1], it->pos[2]);
    }

    if (polygonMode == GL_LINE) {
        glDisable(GL_LIGHTING);  
    }
    else {
        glEnable(GL_LIGHTING);   
    }

    glFlush();
    glutSwapBuffers();
}

//-- Special key event callback --------------------------------------------
// To enable the use of left and right arrow keys to ro-tate the scene
void keyboard(unsigned char key, int x, int y) {

    float d_x = look_x - eye_x;
    float d_z = look_z - eye_z;
    //Normalize the Direction Vector (divide by length)
  
    ////////////////////////////// polygon mode /////////////////////////////// 
    if (key == 'q' || key == 'Q') { 
        if (polygonMode == GL_FILL) {
            polygonMode = GL_LINE;
        }
        else {
            polygonMode = GL_FILL;
        }
        glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
    }

    /////////////////////////////// Forward /////////////////////////////
    else if (key == 'w' || key == 'W') {

        eye_x += d_x * 0.1;
        eye_z += d_z * 0.1;
        look_x += d_x * 0.1;
        look_z += d_z * 0.1;
    
    }

    //////////////////////////// Backward //////////////////////////////////
    else if (key == 's' || key == 'S') {  

        eye_x -= d_x * 0.1;
        eye_z -= d_z * 0.1;
        look_x -= d_x * 0.1;
        look_z -= d_z * 0.1;
    
    }


    glutPostRedisplay();
}

// Function to handle special key presses
void special(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) {
        angle--;
    }
    else if (key == GLUT_KEY_RIGHT) {
        angle++;
    }
    else if (key == GLUT_KEY_UP) {
        cam_hgt--;
    }
    else if (key == GLUT_KEY_DOWN) {
        cam_hgt++;
    }

	////////////////////////// Limit the camera height ///////////////////////////////
    if (cam_hgt > 25.) {
        cam_hgt = 25.;
    }
    else if (cam_hgt < 10.) {
        cam_hgt = 10.;
    }
    glutPostRedisplay();  
}


GLUquadricObj* q;
void initialize() {
    initializeBricks();
    initializeItems1();
    initializeItems2();


    glClearColor(1., 1., 1., 1.); 	 
    glClearDepth(1.);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);

    loadTexture();

    //light 0
    float white[4] = { 1., 1., 1., 1. };
    float ambient[4] = { 0.6, 0.6, 0.6, 1.0 }; // Ambient light
    float diffuse[4] = { 0.3, 0.3, 0.3, 1.0 };
    float specular[4] = { 0.1, 0.1, 0.1, 1.0 };
   

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 30);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);	
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);	
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.);

    glBlendFunc(GL_ONE,GL_ONE);



    //light 1
    glEnable(GL_LIGHT1);
    float red[4] = { 1.0, 0.0, 0.0, 1.0 }; // Red 
    glLightfv(GL_LIGHT1, GL_DIFFUSE, red);
    glLightfv(GL_LIGHT1, GL_SPECULAR, red);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 2.0);

    
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-5., 5., -5., 5., 5., 1000.);	//Camera Frustum
}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_DEPTH);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Factory");

    initialize();

   
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutTimerFunc(16, timer, 0); 

    glutMainLoop();
    return 0;

}
