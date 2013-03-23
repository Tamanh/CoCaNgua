﻿/* Module      : MainFile.cpp
 * Author      : 
 * Email       : 
 * Course      : Computer Graphics
 *
 * Description : 
 *
 *
 * Date        : 
 *
 * History:
 * Revision      Date          Changed By
 * --------      ----------    ----------
 * 01.00         ?????          ???
 * First release.
 *
 */

/* -- INCLUDE FILES ------------------------------------------------------ */
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>

#include <iostream>

#include "Model.h"


using namespace std;

// GLUT CALLBACK functions
void displayCB();
void reshapeCB(int w, int h);
void timerCB(int millisec);
void keyboardCB(unsigned char key, int x, int y);
void mouseCB(int button, int stat, int x, int y);
void mouseMotionCB(int x, int y);

void initGL();
int  initGLUT(int argc, char **argv);
bool initSharedMem();
void initLights();

void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
void drawString(const char *str, int x, int y, float color[4], void *font);
void drawString3D(const char *str, float pos[3], float color[4], void *font);
void DrawCircle(float cx, float cy, float r, int num_segments, unsigned int border = 0);

/* -- CONSTANT ----------------------------------------------------------- */
const int   SCREEN_WIDTH    = 800;
const int   SCREEN_HEIGHT   = 600;
const float CAMERA_DISTANCE = 10.0f;
const int   TEXT_WIDTH      = 8;
const int   TEXT_HEIGHT     = 13;
const float DELTA_TIME      = 33;

/* -- DATA STRUCTURES ---------------------------------------------------- */


/* -- GLOBAL VARIABLES --------------------------------------------------- */

GLPoint3f           eyePoint(5.0, 20.0, 30.0);
GLPoint3f           lookAtPoint(0.0, 1.0, 0.0);

Model               g_model;


void *font = GLUT_BITMAP_8_BY_13;
int screenWidth;
int screenHeight;
bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float cameraDistance;

/* -- LOCAL VARIABLES ---------------------------------------------------- */


///////////////////////////////////////////////////////////////////////////////
// initialize global variables
///////////////////////////////////////////////////////////////////////////////
bool initSharedMem()
{
  screenWidth = SCREEN_WIDTH;
  screenHeight = SCREEN_HEIGHT;

  mouseLeftDown = mouseRightDown = mouseMiddleDown = false;
  mouseX = mouseY = 0;

  cameraAngleX = cameraAngleY = 0.0f;
  cameraDistance = CAMERA_DISTANCE;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
// initialize GLUT for windowing
///////////////////////////////////////////////////////////////////////////////
int initGLUT(int argc, char **argv)
{
  // GLUT stuff for windowing
  // initialization openGL window.
  // it is called before any other GLUT routine
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);   // display mode

  glutInitWindowSize(screenWidth, screenHeight);  // window size

  glutInitWindowPosition(100, 100);               // window location

  // finally, create a window with openGL context
  // Window will not displayed until glutMainLoop() is called
  // it returns a unique ID
  int handle = glutCreateWindow("Assignment - 08. Co Ca Ngua");     // param is the title of window

  // register GLUT callback functions
  glutDisplayFunc(displayCB);
  glutTimerFunc(DELTA_TIME, timerCB, 33);             // redraw only every given millisec
  glutReshapeFunc(reshapeCB);
  glutKeyboardFunc(keyboardCB);
  glutMouseFunc(mouseCB);
  glutMotionFunc(mouseMotionCB);

  return handle;
}

///////////////////////////////////////////////////////////////////////////////
// initialize OpenGL
// disable unused features
///////////////////////////////////////////////////////////////////////////////
void initGL()
{
  glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

  // enable /disable features
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_CULL_FACE);

  glShadeModel(GL_SMOOTH);

  // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  //glEnable(GL_COLOR_MATERIAL);

  glClearColor(0.3, 0.3, 0.3, 1);                   // background color
  glClearStencil(0);                          // clear stencil buffer
  glClearDepth(1.0f);                         // 0 is near, 1 is far
  glDepthFunc(GL_LEQUAL);

  initLights();
}

///////////////////////////////////////////////////////////////////////////////
// initialize lights
///////////////////////////////////////////////////////////////////////////////
void initLights()
{
  // set up light colors (ambient, diffuse, specular)
  GLfloat lightKa[] = {.2f, .2f, .2f, 1.0f};  // ambient light
  GLfloat lightKd[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
  GLfloat lightKs[] = {1, 1, 1, 1};           // specular light
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

  // position the light
  float lightPos[4] = {0, 100, 0, 1}; // positional light
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration
}

/* ----------------------------------------------------------------------- */
/* Function    : void myInit( void )
 *
 * Description : Initialize OpenGL and the window where things will be
 *               drawn.
 *
 * Parameters  : void
 *
 * Returns     : void
 */

void initModel( void )  {

  g_model.loadModel("Models/a.obj");
  //g_model.normalize();
  
}

///////////////////////////////////////////////////////////////////////////////
// set the projection matrix as perspective
///////////////////////////////////////////////////////////////////////////////
void toPerspective()
{
  // set viewport to be the entire window
  glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

  // set perspective viewing frustum
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0f, (float)(screenWidth)/screenHeight, 1.0f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip

  // switch to modelview matrix in order to set scene
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

///////////////////////////////////////////////////////////////////////////////
// set camera position and lookat direction
///////////////////////////////////////////////////////////////////////////////
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(posX, posY, posZ, targetX, targetY, targetZ, 0, 1, 0); // eye(x,y,z), focal(x,y,z), up(x,y,z)
}

///////////////////////////////////////////////////////////////////////////////
// write 2d text using GLUT
// The projection matrix must be set to orthogonal before call this function.
///////////////////////////////////////////////////////////////////////////////
void drawString(const char *str, int x, int y, float color[4], void *font)
{
  glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
  glDisable(GL_LIGHTING);     // need to disable lighting for proper text color
  glDisable(GL_TEXTURE_2D);

  glColor4fv(color);          // set text color
  glRasterPos2i(x, y);        // place text position

  // loop all characters in the string
  while(*str)
  {
    glutBitmapCharacter(font, *str);
    ++str;
  }

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  glPopAttrib();
}



///////////////////////////////////////////////////////////////////////////////
// draw a string in 3D space
///////////////////////////////////////////////////////////////////////////////
void drawString3D(const char *str, float pos[3], float color[4], void *font)
{
  glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
  glDisable(GL_LIGHTING);     // need to disable lighting for proper text color
  glDisable(GL_TEXTURE_2D);

  glColor4fv(color);          // set text color
  glRasterPos3fv(pos);        // place text position

  // loop all characters in the string
  while(*str)
  {
    glutBitmapCharacter(font, *str);
    ++str;
  }

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  glPopAttrib();
}

void DrawCircle(float cx, float cy, float r, int num_segments, unsigned int border) 
{ 
  glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
  glDisable(GL_LIGHTING);     // need to disable lighting for proper text color
  glDisable(GL_TEXTURE_2D);

  float theta = 2 * 3.1415926 / float(num_segments); 
  float c = cosf(theta);//precalculate the sine and cosine
  float s = sinf(theta);
  float t;

  float x = r;//we start at angle = 0 
  float y = 0; 

  glBegin(GL_TRIANGLE_FAN); 
  for(int ii = 0; ii < num_segments; ii++) 
  { 
    glVertex3f(x + cx, 0, y + cy);//output vertex 

    //apply the rotation matrix
    t = y;
    y = c * y - s * x;
    x = s * t + c * x;
  } 
  glEnd(); 

  glLineWidth(border);
  glColor3f(1, 1, 1);
  glBegin(GL_LINE_LOOP);
  for (int ii = 0; ii < num_segments; ii++)
  {
    glVertex3f(x + cx, 0, y + cy);//output vertex 

    //apply the rotation matrix
    t = y;
    y = c * y - s * x;
    x = s * t + c * x;
  }
  glEnd();
  glLineWidth(1);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  glPopAttrib();
}

/* ----------------------------------------------------------------------- */
/* Function    : void myDisplay( void )
 *
 * Description : This function gets called everytime the window needs to
 *               be redrawn.
 *
 * Parameters  : void
 *
 * Returns     : void
 */

void displayCB( void )  {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // Save current matrix state
  glPushMatrix();
  glTranslatef(0, 0, -cameraDistance);
  glRotatef(cameraAngleX, 1, 0, 0);   // pitch
  glRotatef(cameraAngleY, 0, 1, 0);   // heading

  glBegin(GL_LINES);
    glColor3f(0, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(10, 0, 0);

    glVertex3f(0, 0, 0);
    glVertex3f(0, 10, 0);

    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 10);
  glEnd();

  glBegin(GL_LINES);
  glColor3f(.1, .1, .1);
  for (int i = -100; i < 100; i+=2)
  {
    glVertex3f(100, 0, i);
    glVertex3f(-100, 0, i);

    glVertex3f(i, 0, 100);
    glVertex3f(i, 0, -100);
  }
  glEnd();

  /************************************************************************/
  /*  Draw center plus                                                    */
  /************************************************************************/
  for (int i = 1; i <= 4; i++)
  {
    glColor3f(1, 0, 0);     // red
    DrawCircle(0, 4*i, 1.5, 100, 3);
    glColor3f(1, 1, 0);     // yellow
    DrawCircle(4*i, 0, 1.5, 100, 3);
    glColor3f(0, 1, 0);     // green
    DrawCircle(0, -4*i, 1.5, 100, 3);
    glColor3f(0, 0, 1);     // blue
    DrawCircle(-4*i, 0, 1.5, 100, 3);
  }

  /************************************************************************/
  /* Draw the road                                                        */
  /************************************************************************/
  

  glDisable(GL_COLOR_MATERIAL);
  //g_model.drawModel();
  glEnable(GL_COLOR_MATERIAL);

  float pos[3] = {0.0f, 5.0f, 0};
  float color[4] = {1,1,1,1};
  drawString3D("Chess board", pos, color, font);

  //glDisable(GL_TEXTURE_2D);

  glPopMatrix();
  glutSwapBuffers();
  //glutPostRedisplay();
  
  g_model.setAnchorPoint(glp3f(0, -0.5, 0));
  //g_model.setPosition(glp3f(10, 10, 0));

  //cout << g_model.getWidth() << ' ' << g_model.getHeight() << ' ' << g_model.getLength() << ' ' << g_model.getRadius();
}

void reshapeCB(int width, int height) {
  screenWidth = width;
  screenHeight = height;
  toPerspective();
  setCamera(eyePoint.x, eyePoint.y, eyePoint.z, lookAtPoint.x, lookAtPoint.y, lookAtPoint.z);
}

/*
 * Callback function for mouse event
 */
void mouseCB(int button, int state, int x, int y) {

  mouseX = x;
  mouseY = y;

  if(button == GLUT_LEFT_BUTTON)
  {
    if(state == GLUT_DOWN)
    {
      mouseLeftDown = true;
    }
    else if(state == GLUT_UP)
      mouseLeftDown = false;
  }

  else if(button == GLUT_RIGHT_BUTTON)
  {
    if(state == GLUT_DOWN)
    {
      mouseRightDown = true;
    }
    else if(state == GLUT_UP)
      mouseRightDown = false;
  }

  else if(button == GLUT_MIDDLE_BUTTON)
  {
    if(state == GLUT_DOWN)
    {
      mouseMiddleDown = true;
    }
    else if(state == GLUT_UP)
      mouseMiddleDown = false;
  }
}

/*
 * Callback function for animation motion event
 */
void mouseMotionCB(int x, int y) {

  if(mouseLeftDown)
  {
    cameraAngleY += (x - mouseX);
    cameraAngleX += (y - mouseY);
    mouseX = x;
    mouseY = y;

    if (cameraAngleX < -eyePoint.y)
    {
      cameraAngleX = -eyePoint.y;
    }

    if (cameraAngleX > 60)
    {
      cameraAngleX = 60;
    }
  }
  if(mouseRightDown)
  {
    cameraDistance -= (y - mouseY) * 0.2f;
    mouseY = y;
  }
}

void keyboardCB(unsigned char key,int x,int y)
{
  switch(key){
  case 's':
    SOIL_save_screenshot(
      "screenshot.bmp",
      SOIL_SAVE_TYPE_BMP,
      0, 0, screenWidth, screenHeight
    );
    break;
  default:
    break;
  }
}

void timerCB(int value) {
  
  glutPostRedisplay(); //Redraw scene

  glutTimerFunc(DELTA_TIME, timerCB, 0); //Call update in 5 milliseconds
}


/* ----------------------------------------------------------------------- */
/* Function    : int main( int argc, char** argv )
 *
 * Description : This is the main function. It sets up the rendering
 *               context, and then reacts to user events.
 *
 * Parameters  : int argc     : Number of command-line arguments.
 *               char *argv[] : Array of command-line arguments.
 *
 * Returns     : int : Return code to pass to the shell.
 */

int main( int argc, char *argv[] )  {
  // init global vars
  initSharedMem();

  // init GLUT and GL
  initGLUT(argc, argv);
  initGL();

  // Initialize some things.
  initModel( );

  //glutTimerFunc(DELTA_TIME, timerCB, 0);

  // Now that we have set everything up, loop responding to events.
  glutMainLoop( );
}



/* ----------------------------------------------------------------------- */

