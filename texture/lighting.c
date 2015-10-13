/*
 *  Lighting - Edward Zhu
 *
 *  Demonstrates basic lighting with a house. 
 *  Code skeleton taken from and inspired by example 13 from moodle.
 *
 *  Key bindings:
 *  l          Toggles lighting
 *  m          Toggles light movement
 *  < or >     when light movement toggled off, it moves the light source
 *  []         Lower/rise light
 *  3          Toggle light distance (10/2)
 *  p          Toggles ortogonal/perspective projection
 *  + or -     Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 *
 * Other options:
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  1          Toggle smooth/flat shading
 *  2          Toggle local viewer mode
 *  4          Change ball increment
 *  5          Invert bottom normal
 */
#include "CSCIx229.h"

int axes=1;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=60;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=10.0;   //  Size of world
// Light values
int one       =   1;  // Unit value
int distance  =   10;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  50;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   4;  // Elevation of light

/*
 *  Draw a house
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void house(double x, double y, double z,
          double dx, double dy, double dz,
          double th){
  
     //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

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
  glNormal3f( 0, 0, 1);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  //  Back
  glNormal3f( 0, 0,-1);
  glColor3f(0.4,0.4,0.4);
  glVertex3f(+1,-1,-1);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,+1,-1);
  glVertex3f(+1,+1,-1);
  //  Right
  glColor3f(0.5,0.5,0.5);
  glNormal3f(+1, 0, 0);
  glVertex3f(+1,-1,+1);
  glVertex3f(+1,-1,-1);
  glVertex3f(+1,+1,-1);
  glVertex3f(+1,+1,+1);
  //  Left
  glColor3f(0.5,0.5,0.5);
  glNormal3f(-1, 0, 0);
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
  glNormal3f( 0,-1, 0);
  glVertex3f(-1,-1,-1);
  glVertex3f(+1,-1,-1);
  glVertex3f(+1,-1,+1);
  glVertex3f(-1,-1,+1);
  glEnd();

  /*
  * Draw the front door in front that protrudes a tiny bit
  */
  glBegin(GL_QUADS);
  glColor3ub(100,50,0);
  //  Front
  glNormal3f(0,0,+1);
  glVertex3f(+.2,-1,1.1);
  glVertex3f(-.2,-1,1.1);
  glVertex3f(-.2,0,1.1);
  glVertex3f(+.2,0,1.1);
  //  Right
  glNormal3f(+1,0,0);
  glVertex3f(+.2,0,1.1);
  glVertex3f(+.2,-1,1.1);
  glVertex3f(+.2,-1,1.0);
  glVertex3f(+.2,0,1.0);
  //  Left
  glNormal3f(-1,0,0);
  glVertex3f(-.2,0,1.1);
  glVertex3f(-.2,-1,1.1);
  glVertex3f(-.2,-1,1.0);
  glVertex3f(-.2,0,1.0);
  glEnd();
  //  Top
  glVertex3f(+.2,0,1.1);
  glVertex3f(+.2,0,1.0);
  glVertex3f(-.2,0,1.0);
  glVertex3f(-.2,0,1.1);

  /* 
  * Draw out the chimney on the right side of ever house.
  */ 
  glBegin(GL_QUADS);
    // Front
  glColor3ub(100,0,0);
  glNormal3f(0,0,+1);
  glVertex3f(+.5,+1,+.25);
  glVertex3f(+.5,+2,+.25);
  glVertex3f(+1,+2,+.25);
  glVertex3f(+1,+1,+.25);
  // Back
  glColor3ub(100,0,0);
  glNormal3f(0,0,-1);
  glVertex3f(+.5,+1,-.25);
  glVertex3f(+.5,+2,-.25);
  glVertex3f(+1,+2,-.25);
  glVertex3f(+1,+1,-.25);
  // Left
  glColor3ub(60,0,0);
  glNormal3f(-1,0,0);
  glVertex3f(+.5,+1,+.25);
  glVertex3f(+.5,+1,-.25);
  glVertex3f(+.5,+2,-.25);
  glVertex3f(+.5,+2,+.25);
  // Right
  glColor3ub(60,0,0);
  glNormal3f(+1,0,0);
  glVertex3f(+1,+1,+.25);
  glVertex3f(+1,+1,-.25);
  glVertex3f(+1,+2,-.25);
  glVertex3f(+1,+2,+.25);
  // Top
  glColor3ub(10,10,10);
  glNormal3f(0,+1,0);
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
  glNormal3f(0,0,+1);
  glVertex3f(+roof_width,+roof_bottom,+root_length);
  glVertex3f(-roof_width,+roof_bottom,+root_length);
  glVertex3f(0,+roof_height,+root_length);
  // Back
  glNormal3f(0,0,-1);
  glVertex3f(+roof_width,+roof_bottom,-root_length);
  glVertex3f(-roof_width,+roof_bottom,-root_length);
  glVertex3f(0,+roof_height,-root_length);
  glEnd();
  // Sides of Roof
  // two sides are curved, bottom is just flat
  glBegin(GL_QUADS);
  // Bottom
  glColor3ub(0,0,0);
  glNormal3f(0,+1,0);
  glVertex3f(+roof_width,+roof_bottom,-root_length);
  glVertex3f(+roof_width,+roof_bottom,+root_length);
  glVertex3f(-roof_width,+roof_bottom,+root_length);
  glVertex3f(-roof_width,+roof_bottom,-root_length);
  // Right side
  glColor3ub(50,50,50);
  glNormal3f(+1,+1,0);
  glVertex3f(+roof_width,+roof_bottom,-root_length);
  glVertex3f(+roof_width,+roof_bottom,+root_length);
  glVertex3f(0,+roof_height,+root_length);
  glVertex3f(0,+roof_height,-root_length);
  // Left Side
  glNormal3f(-1,-1,0);
  glVertex3f(-roof_width,+roof_bottom,-root_length);
  glVertex3f(-roof_width,+roof_bottom,+root_length);
  glVertex3f(0,+roof_height,+root_length);
  glVertex3f(0,+roof_height,-root_length);  
  glEnd();

  glPopMatrix();
  glDisable(GL_POLYGON_OFFSET_FILL);
}

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  Draw out the scene of the neighboorhood (houses, roads, ground)
 */
static void drawScene(){
  // Make the four houses in the neighborhood
  house(0,0,0 , 1,1,1, 0);
  house(-5,0.5,0 , 1,1.5,1, 0);
  house(3,0,5 , 2,1,1 , -90);
  house(-4,0,6 , 1,1,1 , 180);

  // huge green ground
  glColor3ub(0,35,0);
  glBegin(GL_QUADS);
  glNormal3f(0,+1,0);
  glVertex3f(-200,-1,-200);
  glVertex3f(-200,-1,200);
  glVertex3f(200,-1,200);
  glVertex3f(200,-1,-200);
  glEnd();

  // Make L shaped roads
  glColor3ub(0,0,0);
  glBegin(GL_QUADS);
  glVertex3f(-20,-0.9,2);
  glVertex3f(-20,-0.9,4.5);
  glVertex3f(1,-0.9,4.5);
  glVertex3f(1,-0.9,2);
  glEnd();
  glColor3ub(0,0,0);
  glBegin(GL_QUADS);
  glVertex3f(-2,-0.9,2);
  glVertex3f(-2,-0.9,20);
  glVertex3f(1,-0.9,20);
  glVertex3f(1,-0.9,2);
  glEnd();
}

/*
 *  OpenGL calls this routine to display the scene
 */
void display()
{
   const double len=2.0;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
     glDisable(GL_LIGHTING);

   //  Draw scene
   drawScene();

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
     th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shinyvec[0]);
   }

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(mode?fov:0,asp,dim);
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
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   //  Switch projection mode
   else if (ch == 'p' || ch == 'P')
      mode = 1-mode;
   //  Toggle light movement
   else if (ch == 'm' || ch == 'M')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;
    //  Smooth color model
   else if (ch == '1')
      smooth = 1-smooth;
   //  Local Viewer
   else if (ch == '2')
      local = 1-local;
   else if (ch == '3')
      distance = (distance==2) ? 10 : 2;
   //  Toggle ball increment
   else if (ch == '4')
      inc = (inc==10)?3:10;
   //  Flip sign
   else if (ch == '5')
      one = -one;

   //  Translate shininess power to value (-1 => 0)
   shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(mode?fov:0,asp,dim);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
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
   Project(mode?fov:0,asp,dim);
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
   glutInitWindowSize(600,600);
   glutCreateWindow("Lighting - Edward Zhu");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}