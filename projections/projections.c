/*
 *  Projections - HW 4
 *  Edward Zhu
 *  CSCI 4229
 *
 *  Code inspired and modified from ex9 from moodle.
 *  Modified HW3 - 3Dobjects code but added First person view.
 *
 *  I created a neighborhood of houses located around a corner.
 *
 *
 *  Time it took to complete: 5 hours
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int mode=0;       //  Projection mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=60;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=12.0;   //  Size of world
int new_angle = 0;
double moveX = 0.0;
double moveZ = 0.0;
double y_pos = 0.0;

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415927/180*(th))
#define Sin(th) sin(3.1415927/180*(th))

/*
 *  Output information onto the screen for the user to interact
 *   with the animation
 */
 void output( int text_pos, char *string)
{
  glColor3f( 255, 255, 255 );
  glWindowPos2i(5 , text_pos);
  int len, i;
  len = (int)strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
  }
}

/*
 *  Draw a house
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void house(double x, double y, double z,
          double dx, double dy, double dz,
          double th){
  
  const double roof_height = 2;
  const double roof_width = 1.25;
  const double root_length = 1.1;
  const double roof_bottom = 0.75;

  glPushMatrix();
  
  // Translations
  glTranslated(x, y, z);
  glRotated(th, 0, 1, 0);
  glScaled(dx, dy, dz);

  /*
  * Main Cube of the house
  */
  glBegin(GL_QUADS);
  //  Front
  glColor3f(0.4,0.4,0.4);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  //  Back
  glColor3f(0.4,0.4,0.4);
  glVertex3f(+1,-1,-1);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,+1,-1);
  glVertex3f(+1,+1,-1);
  //  Right
  glColor3f(0.5,0.5,0.5);
  glVertex3f(+1,-1,+1);
  glVertex3f(+1,-1,-1);
  glVertex3f(+1,+1,-1);
  glVertex3f(+1,+1,+1);
  //  Left
  glColor3f(0.5,0.5,0.5);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,-1,+1);
  glVertex3f(-1,+1,+1);
  glVertex3f(-1,+1,-1);
  //  Top
  glColor3f(0,0,0);
  glVertex3f(-1,+1,+1);
  glVertex3f(+1,+1,+1);
  glVertex3f(+1,+1,-1);
  glVertex3f(-1,+1,-1);
  //  Bottom
  glColor3f(0,0,0);
  glVertex3f(-1,-1,-1);
  glVertex3f(+1,-1,-1);
  glVertex3f(+1,-1,+1);
  glVertex3f(-1,-1,+1);
  glEnd();

  /*
  * Draw the door in front that protrudes a tiny bit
  */
  glBegin(GL_QUADS);
  glColor3ub(100,50,0);
  //  Front
  glVertex3f(+.2,-1,1.01);
  glVertex3f(-.2,-1,1.01);
  glVertex3f(-.2,0,1.01);
  glVertex3f(+.2,0,1.01);
    //  Right
  glVertex3f(+.2,0,1.01);
  glVertex3f(+.2,-1,1.01);
  glVertex3f(+.2,-1,1.0);
  glVertex3f(+.2,0,1.0);
  //  Left
  glVertex3f(-.2,0,1.01);
  glVertex3f(-.2,-1,1.01);
  glVertex3f(-.2,-1,1.0);
  glVertex3f(-.2,0,1.0);
  glEnd();
  //  Top
  glVertex3f(+.2,0,1.01);
  glVertex3f(+.2,0,1.0);
  glVertex3f(-.2,0,1.0);
  glVertex3f(-.2,0,1.01);

  /* 
  * Draw out the chimney on the right side of ever house.
  */ 
  glBegin(GL_QUADS);
    // Front
  glColor3ub(100,0,0);
  glVertex3f(+.5,+1,+.25);
  glVertex3f(+.5,+2,+.25);
  glVertex3f(+1,+2,+.25);
  glVertex3f(+1,+1,+.25);
  // Back
  glColor3ub(100,0,0);
  glVertex3f(+.5,+1,-.25);
  glVertex3f(+.5,+2,-.25);
  glVertex3f(+1,+2,-.25);
  glVertex3f(+1,+1,-.25);
  // Left
  glColor3ub(60,0,0);
  glVertex3f(+.5,+1,+.25);
  glVertex3f(+.5,+1,-.25);
  glVertex3f(+.5,+2,-.25);
  glVertex3f(+.5,+2,+.25);
  // Right
  glColor3ub(60,0,0);
  glVertex3f(+1,+1,+.25);
  glVertex3f(+1,+1,-.25);
  glVertex3f(+1,+2,-.25);
  glVertex3f(+1,+2,+.25);
  // Top
  glColor3ub(10,10,10);
  glVertex3f(+.5,+2,+.25);
  glVertex3f(+.5,+2,-.25);
  glVertex3f(+1,+2,-.25);
  glVertex3f(+1,+2,+.25);
  glEnd();

  /* 
  * Draw out the roof using GL_triangles for the front
  * and back, then back to quads for the sides and bottom
  */ 
  glBegin(GL_TRIANGLES);
  // Front
  glColor3ub(30,30,30);
  glVertex3f(+roof_width,+roof_bottom,+root_length);
  glVertex3f(-roof_width,+roof_bottom,+root_length);
  glVertex3f(0,+roof_height,+root_length);
  // Back
  glVertex3f(+roof_width,+roof_bottom,-root_length);
  glVertex3f(-roof_width,+roof_bottom,-root_length);
  glVertex3f(0,+roof_height,-root_length);
  glEnd();
  // Sides of Roof
  glBegin(GL_QUADS);
  // Bottom
  glColor3ub(0,0,0);
  glVertex3f(+roof_width,+roof_bottom,-root_length);
  glVertex3f(+roof_width,+roof_bottom,+root_length);
  glVertex3f(-roof_width,+roof_bottom,+root_length);
  glVertex3f(-roof_width,+roof_bottom,-root_length);
  // Right side
  glColor3ub(50,50,50);
  glVertex3f(+roof_width,+roof_bottom,-root_length);
  glVertex3f(+roof_width,+roof_bottom,+root_length);
  glVertex3f(0,+roof_height,+root_length);
  glVertex3f(0,+roof_height,-root_length);
  // Left Side
  glVertex3f(-roof_width,+roof_bottom,-root_length);
  glVertex3f(-roof_width,+roof_bottom,+root_length);
  glVertex3f(0,+roof_height,+root_length);
  glVertex3f(0,+roof_height,-root_length);  
  glEnd();

  glPopMatrix();
}

/*   
 *     set the projection to perspective
 */
static void set_proj_perspective()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fov,asp,dim/4,4*dim);
  glMatrixMode(GL_MODELVIEW);
}
/*   
 *     set the projection to perspective
 */
static void set_proj_orthogonal()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
  glMatrixMode(GL_MODELVIEW);
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
    if (mode == 1)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   // First Person mode, we care about the gluLookAt function
   // Instead of looking at a origin point, we need to make movements
   // so when we turn, we rotate both the eye position and the center values
   // Read too many stackover flow threads and blogs to figure this simple math out.
   else if(mode == 2)
   {
      double Ex = 0 + moveX;
      double Ez = +2*dim + moveZ;
      double Cx = Ex-Sin(new_angle);
      double Cz = Ez-Cos(new_angle);
      gluLookAt(Ex,y_pos,Ez , Cx,y_pos,Cz , 0,1,0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   //  Draw our all the houses on the street
   house(-1,0,0 , 1,1,1, 0);
   house(-5,1,0 , 1,2,1, 0);
   house(3,0,3 , 2,1,1 , -90);
   house(3,0,8 , 1,1,1 , -90);
   house(-5,0,8 , 1,1,1 , 180);

  // Draw a huge green groundthat expands a huge space
  glColor3ub(0,35,0);
  glBegin(GL_QUADS);
  glVertex3f(-200,-1,-200);
  glVertex3f(-200,-1,200);
  glVertex3f(200,-1,200);
  glVertex3f(200,-1,-200);
  glEnd();

  // Draw the corner road
  glColor3ub(0,0,0);
  glBegin(GL_QUADS);
  glVertex3f(-20,-0.9,2);
  glVertex3f(-20,-0.9,6);
  glVertex3f(1,-0.9,6);
  glVertex3f(1,-0.9,2);
  glEnd();
  glColor3ub(0,0,0);
  glBegin(GL_QUADS);
  glVertex3f(-3,-0.9,2);
  glVertex3f(-3,-0.9,20);
  glVertex3f(1,-0.9,20);
  glVertex3f(1,-0.9,2);
  glEnd();

   //  Display parameters
   if (mode == 2) {
    output(45, "Projection= First Person View Perspective");
    output(25, "w and s for roaming the up and down y-axis");
   }
   else if (mode == 1) {
    output(25, "Projection= Perspective");
   }
   else if (mode == 0) {
    output(25, "Projection= Orthogonal");
   }
   output(5, "Use ARROW KEYS to move around  --  m to switch modes  --  0 for hard reset");
  
   //  Render the scene and make it visible
   glFlush();
   glGetError();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
  if (mode == 0 || mode == 1)
   {
     //  Right arrow key - increase angle angle 
     if (key == GLUT_KEY_RIGHT)
        th -= 5;
     //  Left arrow key - decrease angle angle 
     else if (key == GLUT_KEY_LEFT)
        th += 5;
     //  Up arrow key - increase elevation angle 
     else if (key == GLUT_KEY_UP)
        ph += 5;
     //  Down arrow key - decrease elevation angle 
     else if (key == GLUT_KEY_DOWN)
        ph -= 5;
     //  Keep angles to +/-360 degrees
     th %= 360;
     ph %= 360;
   } else {
    // If the mode is 2, then put it in first person mode
    // look to the right
    if (key == GLUT_KEY_RIGHT)
       new_angle -= 5;
    //  look to the left
    else if (key == GLUT_KEY_LEFT)
       new_angle += 5;
    //  move forward
    else if (key == GLUT_KEY_UP) 
    {
       moveX -= 0.5*Sin(new_angle);
       moveZ -= 0.5*Cos(new_angle);
    }     
    //  move backwards
    else if (key == GLUT_KEY_DOWN)
    {
       moveX += 0.5*Sin(new_angle);
       moveZ += 0.5*Cos(new_angle);
    }
    //  PageUp key - increase dim
    else if (key == GLUT_KEY_PAGE_UP)
       dim += 0.1;
    //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
       dim -= 0.1;
    //  Keep angles to +/-360 degrees
    new_angle %= 360;
   }
   //  Update projection
   if (!mode)
   {
    set_proj_orthogonal();
   } else {
    set_proj_perspective();
   }
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
    if (ch == 27)
      exit(0);
    //  Switch display mode
    else if (ch == 'm' || ch == 'M')
      mode = (mode+1)%3;
    //  Change field of view angle
    else if (ch == '-')
      fov--;
    else if (ch == '+')
      fov++;
    // change the y position up
    else if (ch == 'w')
      y_pos += 0.1;
    // change the y position up
    else if (ch == 's')
      y_pos -= 0.1;
    // Hard reset
    else if (ch == '0')
    new_angle = moveX = moveZ = y_pos = th = ph = 0;
   //  Reproject
   if (!mode)
   {
    set_proj_orthogonal();
   } else {
    set_proj_perspective();
   }
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   if (!mode)
   {
    set_proj_orthogonal();
   } else {
    set_proj_perspective();
   }
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(700,700);
   glutCreateWindow("Edward Zhu - Projections");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
