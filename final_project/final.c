/*
 *  Tie Fighter - FINAL PROJECT - Edward Zhu
 *
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
#include <math.h>

int axes=1;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=60;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=20.0;   //  Size of world

unsigned int texture[18];
GLuint cockpitTex;

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


// static void normal(double x1, double y1, double z1,
//                 double x2, double y2, double z2,
//                 double x3, double y3, double z3){
//  // Build first vector
//  double v1x = x2 - x1;
//  double v1y = y2 - y1;
//  double v1z = z2 - z1;

//  // Build second vector
//  double v2x = x3 - x1;
//  double v2y = y3 - y1;
//  double v2z = z3 - z1;

//  // Get Cross Product
//  double nx = (v1y*v2z) - (v1z*v2y);
//  double ny = (v1z*v2x) - (v1x*v2z);
//  double nz = (v1x*v2y) - (v1y*v2x);

//  // Set normal for trianlge plane
//  glNormal3f(nx,ny,nz);
// }

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
  ------------------------------------
  TIE FIGHTER BUILD HERE
  ------------------------------------
*/
static void TieFighterBridge(double th, double rx, double ry, double rz)
{
  glPushMatrix();
  glRotated(th,rx,ry,rz);

  glBindTexture(GL_TEXTURE_2D,texture[6]);
  
  /* Main pipe */
  glBegin(GL_QUAD_STRIP);
  for (th = 0; th <= 360; th += 5){
    glNormal3d(0,Sin(th),Cos(th));
    glTexCoord2f(0,th*0.0123+.05); glVertex3d(0, 0.2 * Sin(th), 0.2 * Cos(th));
    glTexCoord2f(1,th*0.0123+.05); glVertex3d(2, 0.2 * Sin(th), 0.2 * Cos(th));
  }
  glEnd();

  /* Wing to main pipe */
  glBegin(GL_QUAD_STRIP);
  for (th = 0; th <= 360; th += 5){
    glNormal3d(0,Sin(th),Cos(th));
    glTexCoord2f(0,th*0.0123+.05); glVertex3d(1.7, 0.2 * Sin(th), 0.2 * Cos(th));
    glTexCoord2f(1,th*0.0123+.05); glVertex3d(2, 0.4 * Sin(th), 0.4 * Cos(th));
  }
  glEnd();

  /* Body to main strip */
  glBegin(GL_QUAD_STRIP);
  for (th = 0; th <= 360; th += 5){
    glNormal3d(0,Sin(th),Cos(th));
    glTexCoord2f(0,th*0.0123+.05); glVertex3d(0, 0.4 * Sin(th), 0.4 * Cos(th));
    glTexCoord2f(1,th*0.0123+.05); glVertex3d(1.4, 0.2 * Sin(th), 0.2 * Cos(th));
  }
  glEnd();


  glPopMatrix();
}

static void TieFighterWing(double x, double y, double z,
                 double rx, double ry, double rz,
                 double th){
  /* WINGS */
  glPushMatrix();

  glTranslated(x,y,z);
  glRotated(th,rx,ry,rz);

  glBindTexture(GL_TEXTURE_2D,texture[9]);
  glBegin(GL_POLYGON);
  glColor3f(0.75,0.75,0.75);
  glNormal3d(1,0,0);
  glTexCoord2f(0.7233,0); glVertex3d(0,-3,-2);
  glTexCoord2f(0.8807,0.4931); glVertex3d(0,0,-3);
  glTexCoord2f(0.7233,1); glVertex3d(0,3,-2);
  glTexCoord2f(0.1491,1); glVertex3d(0,3,2);
  glTexCoord2f(0,0.4931); glVertex3d(0,0,3);
  glTexCoord2f(0.1491,0); glVertex3d(0,-3,2);

  glEnd();
  glPopMatrix();
  /* WINGS END */

  /* BRIDGE BETWEEN WING AND BODY */

  TieFighterBridge(th,rx,ry,rz);
  
  /* BRIDGE END */
   
}


// Main sphere body from class library - 'ball'
static void TieFighter(double x,double y,double z,double s,
                      double rx, double ry, double rz,
                      double th)
{
  int ph;
  //  Save transformation
  glPushMatrix();

  //  Offset, scale and rotate
  glTranslated(x,y,z);
  glScaled(s,s,s);
  glRotated(th,rx,ry,rz);

  glEnable(GL_TEXTURE_2D);
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

  //  White ball
  glColor3d(1,1,1);
  glBindTexture(GL_TEXTURE_2D, texture[1]);

  //  Bands of latitude
  for (ph=-90;ph<90;ph+=inc)
  {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=2*inc)
    {
       glTexCoord2d(th/360.0,ph/180.0+0.5);
       Vertex(th,ph);
       glTexCoord2d(th/360.0,(ph+inc)/180.0+0.5);
       Vertex(th,ph+inc);
    }
    glEnd();
  }

  TieFighterWing(2,0,0, 0,0,0, 0);
  TieFighterWing(-2,0,0, 0,1,0, 180);


  //  Undo transofrmations
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
   
}














































/*
 *  Draw out the scene of the neighboorhood (houses, roads, ground)
 */
static void drawScene(){
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

  // TieFighter(0,0,0,3, 0,0,0, 180);
  TieFighter(0 + Cos(zh), 0 + Sin(zh) * Cos(zh),0,3,   0,0,1,  3*Sin(zh));

}

/*
 *  OpenGL calls this routine to display the scene
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

   //  Draw scene
   drawScene();

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
      th -= 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th += 5;
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
  else if (ch=='d' && dim>0)
    dim -= 2;
  else if (ch=='D' && dim<100)
    dim += 2;
   // //  Light elevation
   // else if (ch=='[')
   //    ylight -= 0.1;
   // else if (ch==']')
   //    ylight += 0.1;
   // //  Ambient level
   // else if (ch=='a' && ambient>0)
   //    ambient -= 5;
   // else if (ch=='A' && ambient<100)
   //    ambient += 5;
   // //  Diffuse level
   // else if (ch=='d' && diffuse>0)
   //    diffuse -= 5;
   // else if (ch=='D' && diffuse<100)
   //    diffuse += 5;
   // //  Specular level
   // else if (ch=='s' && specular>0)
   //    specular -= 5;
   // else if (ch=='S' && specular<100)
   //    specular += 5;
   // //  Emission level
   // else if (ch=='e' && emission>0)
   //    emission -= 5;
   // else if (ch=='E' && emission<100)
   //    emission += 5;
   // //  Shininess level
   // else if (ch=='n' && shininess>-1)
   //    shininess -= 1;
   // else if (ch=='N' && shininess<7)
   //    shininess += 1;
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
   glutCreateWindow("Final - Tie Fighter - Edward Zhu");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);

   texture[6] = LoadTexBMP("textures/img6.bmp");
   texture[9] = LoadTexBMP("textures/wing.bmp");

   // Load all bmps
   // cockpitTex = LoadTexBMP("textures/TIECockpit.bmp");
   texture[1] = LoadTexBMP("textures/TIECockpit.bmp");



   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}