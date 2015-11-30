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
double dim=500.0;   //  Size of world


float sco=180;    //  Spot cutoff angle
float Exp=0;      //  Spot exponent
unsigned int texture[18];
unsigned int space;
int live_env = 0;

int pause = 0;
// static double env_offset=8000;

// Light values
int one       =   1;  // Unit value
int distance  =   10;  // Light distance
int inc       =  10;  // Ball increment
int big_inc   =  30;
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

// building correct vectors
// Mathmatics found here:
//    https://www.opengl.org/wiki/Calculating_a_Surface_Normal
static void normal(double x1, double y1, double z1, 
                  double x2, double y2, double z2, 
                  double x3, double y3, double z3){
 // first vector
 double v1x = x2 - x1;
 double v1y = y2 - y1;
 double v1z = z2 - z1;

 // second vector
 double v2x = x3 - x1;
 double v2y = y3 - y1;
 double v2z = z3 - z1;

 // Get Cross Product
 double nx = (v1y*v2z) - (v1z*v2y);
 double ny = (v1z*v2x) - (v1x*v2z);
 double nz = (v1x*v2y) - (v1y*v2x);

 // Set normal for trianlge plane
 glNormal3f(nx,ny,nz);
}

static void Vertex(double th,double ph)
{
   glNormal3d(Sin(th)*Cos(ph+inc),Cos(th)*Cos(ph+inc),Sin(ph+inc));
   glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
}

// void timer(int v) {
//   env_offset -= 50;
//   if (env_offset < -10000.0) {
//     env_offset += 18000.0;
//   }
//   glutPostRedisplay();
//   glutTimerFunc(1, timer, v);
// }

















// From class examples -  Sky
// static void Space(double D)
// {
//    glColor3f(1,1,1);
//    glEnable(GL_TEXTURE_2D);

//    //  Sides
//    glBindTexture(GL_TEXTURE_2D,space);
//    glBegin(GL_QUADS);
//    glTexCoord2f(0.00,0); glVertex3f(-D,-D,-D);
//    glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
//    glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);
//    glTexCoord2f(0.00,1); glVertex3f(-D,+D,-D);

//    glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
//    glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
//    glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);
//    glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);

//    glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
//    glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
//    glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
//    glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);

//    glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
//    glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
//    glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
//    glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
//    glEnd();

//    //  Top and bottom
//    glBindTexture(GL_TEXTURE_2D,sky[1]);
//    glBegin(GL_QUADS);
//    glTexCoord2f(0.0,0); glVertex3f(+D,+D,-D);
//    glTexCoord2f(0.5,0); glVertex3f(+D,+D,+D);
//    glTexCoord2f(0.5,1); glVertex3f(-D,+D,+D);
//    glTexCoord2f(0.0,1); glVertex3f(-D,+D,-D);

//    glTexCoord2f(1.0,1); glVertex3f(-D,-D,+D);
//    glTexCoord2f(0.5,1); glVertex3f(+D,-D,+D);
//    glTexCoord2f(0.5,0); glVertex3f(+D,-D,-D);
//    glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);
//    glEnd();

//    glDisable(GL_TEXTURE_2D);
// }



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

  

  // Connected to bridge
  glBindTexture(GL_TEXTURE_2D,texture[9]);
  glBegin(GL_POLYGON);
  glColor3f(0.75,0.75,0.75);
  glNormal3d(-1,0,0);
  glTexCoord2f(0.7233,0); glVertex3d(0,-3,-2);
  glTexCoord2f(0.8807,0.4931); glVertex3d(0,0,-3);
  glTexCoord2f(0.7233,1); glVertex3d(0,3,-2);
  glTexCoord2f(0.1491,1); glVertex3d(0,3,2);
  glTexCoord2f(0,0.4931); glVertex3d(0,0,3);
  glTexCoord2f(0.1491,0); glVertex3d(0,-3,2);
  glEnd();
  // Outwards panel
  glBegin(GL_POLYGON);
  glColor3f(0.75,0.75,0.75);
  glNormal3d(1,0,0);
  glTexCoord2f(0.7233,0); glVertex3d(0.1,-3,-2);
  glTexCoord2f(0.8807,0.4931); glVertex3d(0.1,0,-3);
  glTexCoord2f(0.7233,1); glVertex3d(0.1,3,-2);
  glTexCoord2f(0.1491,1); glVertex3d(0.1,3,2);
  glTexCoord2f(0,0.4931); glVertex3d(0.1,0,3);
  glTexCoord2f(0.1491,0); glVertex3d(0.1,-3,2);
  glEnd();  
  // Fill it in!
  // 
  // TODO: NORMALS AND MAKE SURE THEY ARE FACING RIGHT WAY
  // 
  glBindTexture(GL_TEXTURE_2D,texture[1]);
  glBegin(GL_QUADS);
  glNormal3d(0,+0.3,-1);
  glTexCoord2f(0, 0);   glVertex3f(0.0,3, 2);
  glTexCoord2f(10, 0);  glVertex3f(0.0,0, 3);
  glTexCoord2f(10, 10); glVertex3f(0.1,0, 3);
  glTexCoord2f(0, 10);  glVertex3f(0.1,3, 2);

  glNormal3d(0,-0.3,-1);
  glTexCoord2f(0, 0);   glVertex3f(0.0,0, 3);
  glTexCoord2f(10, 0);  glVertex3f(0.0,-3,2);
  glTexCoord2f(10, 10); glVertex3f(0.1,-3,2);
  glTexCoord2f(0, 10);  glVertex3f(0.1,0, 3);

  glNormal3d(0,-1,0);
  glTexCoord2f(0, 0);   glVertex3f(0.0,-3, 2);
  glTexCoord2f(10, 0);  glVertex3f(0.0,-3,-2);
  glTexCoord2f(10, 10); glVertex3f(0.1,-3,-2);
  glTexCoord2f(0, 10);  glVertex3f(0.1,-3, 2);

  glNormal3d(0,-0.3,-1);
  glTexCoord2f(0, 0);   glVertex3f(0.0,-3, -2);
  glTexCoord2f(10, 0);  glVertex3f(0.0,0,-3);
  glTexCoord2f(10, 10); glVertex3f(0.1,0,-3);
  glTexCoord2f(0, 10);  glVertex3f(0.1,-3, -2);

  glNormal3d(0,+0.3,-1);
  glTexCoord2f(0, 0);   glVertex3f(0.0,0, -3);
  glTexCoord2f(10, 0);  glVertex3f(0.0,3,-2);
  glTexCoord2f(10, 10); glVertex3f(0.1,3,-2);
  glTexCoord2f(0, 10);  glVertex3f(0.1,0, -3);

  glNormal3d(0,+1,0);
  glTexCoord2f(0, 0);   glVertex3f(0.0,3, 2);
  glTexCoord2f(10, 0);  glVertex3f(0.0,3,-2);
  glTexCoord2f(10, 10); glVertex3f(0.1,3,-2);
  glTexCoord2f(0, 10);  glVertex3f(0.1,3, 2);
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
  glPushMatrix();

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
      // glNormal3d(Sin(th)*Cos(ph),Cos(th)*Cos(ph),Sin(ph));
      // glVertex3d(Sin(th)*Cos(ph),Cos(th)*Cos(ph),Sin(ph));

      glTexCoord2d(th/360.0,(ph+inc)/180.0+0.5);
      Vertex(th,ph+inc);
      // glNormal3d(Sin(th)*Cos(ph+inc),Cos(th)*Cos(ph+inc),Sin(ph+inc));
      // glVertex3d(Sin(th)*Cos(ph+inc),Cos(th)*Cos(ph+inc),Sin(ph+inc));
    }
    glEnd();
  }


  /* FRONT WINDOWS */
  glColor3d(0, 0, 0);

  glBegin(GL_QUADS);
  normal(0.2 * Cos(3),0.2 * Sin(3),1, 0.47 * Cos(3),0.47 * Sin(3),1, 0.47 * Cos(42),0.47 * Sin(42),1);
  glVertex3d(0.2 * Cos(3), 0.2 * Sin(3), 1);
  glVertex3d(0.47 * Cos(3), 0.47 * Sin(3), 1);
  glVertex3d(0.47 * Cos(42), 0.47 * Sin(42), 1);
  glVertex3d(0.2 * Cos(42), 0.2 * Sin(42), 1);
  glEnd();

  glBegin(GL_QUADS);
  normal(0.2 * Cos(48),0.2 * Sin(48),1, 0.47 * Cos(48),0.47 * Sin(48),1, 0.47 * Cos(87),0.47 * Sin(87),1);
  glVertex3d(0.2 * Cos(48), 0.2 * Sin(48), 1);
  glVertex3d(0.47 * Cos(48), 0.47 * Sin(48), 1);
  glVertex3d(0.47 * Cos(87), 0.47 * Sin(87), 1);
  glVertex3d(0.2 * Cos(87), 0.2 * Sin(87), 1);
  glEnd();

  glBegin(GL_QUADS);
  normal(0.2 * Cos(93),0.2 * Sin(93),1, 0.47 * Cos(93),0.47 * Sin(93),1, 0.47 * Cos(132),0.47 * Sin(132),1);
  glVertex3d(0.2 * Cos(93), 0.2 * Sin(93), 1);
  glVertex3d(0.47 * Cos(93), 0.47 * Sin(93), 1);
  glVertex3d(0.47 * Cos(132), 0.47 * Sin(132), 1);
  glVertex3d(0.2 * Cos(132), 0.2 * Sin(132), 1);
  glEnd();

  glBegin(GL_QUADS);
  normal(0.2 * Cos(138),0.2 * Sin(138),1, 0.47 * Cos(138),0.47 * Sin(138),1, 0.47 * Cos(177),0.47 * Sin(177),1);
  glVertex3d(0.2 * Cos(138), 0.2 * Sin(138), 1);
  glVertex3d(0.47 * Cos(138), 0.47 * Sin(138), 1);
  glVertex3d(0.47 * Cos(177), 0.47 * Sin(177), 1);
  glVertex3d(0.2 * Cos(177), 0.2 * Sin(177), 1);
  glEnd();

  glBegin(GL_QUADS);
  normal(0.2 * Cos(183),0.2 * Sin(183),1, 0.47 * Cos(183),0.47 * Sin(183),1, 0.47 * Cos(222),0.47 * Sin(222),1);
  glVertex3d(0.2 * Cos(183), 0.2 * Sin(183), 1);
  glVertex3d(0.47 * Cos(183), 0.47 * Sin(183), 1);
  glVertex3d(0.47 * Cos(222), 0.47 * Sin(222), 1);
  glVertex3d(0.2 * Cos(222), 0.2 * Sin(222), 1);
  glEnd();

  glBegin(GL_QUADS);
  normal(0.2 * Cos(228),0.2 * Sin(228),1, 0.47 * Cos(228),0.47 * Sin(228),1, 0.47 * Cos(267),0.47 * Sin(267),1);
  glVertex3d(0.2 * Cos(228), 0.2 * Sin(228), 1);
  glVertex3d(0.47 * Cos(228), 0.47 * Sin(228), 1);
  glVertex3d(0.47 * Cos(267), 0.47 * Sin(267), 1);
  glVertex3d(0.2 * Cos(267), 0.2 * Sin(267), 1);
  glEnd();

  glBegin(GL_QUADS);
  normal(0.2 * Cos(273),0.2 * Sin(273),1, 0.47 * Cos(273),0.47 * Sin(273),1, 0.47 * Cos(312),0.47 * Sin(312),1);
  glVertex3d(0.2 * Cos(273), 0.2 * Sin(273), 1);
  glVertex3d(0.47 * Cos(273), 0.47 * Sin(273), 1);
  glVertex3d(0.47 * Cos(312), 0.47 * Sin(312), 1);
  glVertex3d(0.2 * Cos(312), 0.2 * Sin(312), 1);
  glEnd();

  glBegin(GL_QUADS);
  normal(0.2 * Cos(318),0.2 * Sin(318),1, 0.47 * Cos(318),0.47 * Sin(318),1, 0.47 * Cos(357),0.47 * Sin(357),1);
  glVertex3d(0.2 * Cos(318), 0.2 * Sin(318), 1);
  glVertex3d(0.47 * Cos(318), 0.47 * Sin(318), 1);
  glVertex3d(0.47 * Cos(357), 0.47 * Sin(357), 1);
  glVertex3d(0.2 * Cos(357), 0.2 * Sin(357), 1);
  glEnd();
  // Main
  glBegin(GL_POLYGON);
  glColor3d(0, 0, 0);
  glNormal3d(0,0,1);
  glVertex3d(0.17 * Cos(0), 0.17 * Sin(0), 1);
  glVertex3d(0.17 * Cos(45), 0.17 * Sin(45), 1);
  glVertex3d(0.17 * Cos(90), 0.17 * Sin(90), 1);
  glVertex3d(0.17 * Cos(135), 0.17 * Sin(135), 1);
  glVertex3d(0.17 * Cos(180), 0.17 * Sin(180), 1);
  glVertex3d(0.17 * Cos(225), 0.17 * Sin(225), 1);
  glVertex3d(0.17 * Cos(270), 0.17 * Sin(270), 1);
  glVertex3d(0.17 * Cos(315), 0.17 * Sin(315), 1);
  glVertex3d(0.17 * Cos(315), 0.17 * Sin(315), 1);
  glVertex3d(0.17 * Cos(360), 0.17 * Sin(360), 1);
  glEnd();
  // Border
  glBegin(GL_QUAD_STRIP);
  for (th = 0; th <= 360; th += 5)
  {
    glColor3d(0.5, 0.5, 0.5);
    glNormal3d(Cos(th), Sin(th), 0);
    glVertex3d(0.5 * Cos(th), 0.5 * Sin(th), 1);
    glVertex3d(0.5 * Cos(th), 0.5 * Sin(th), 0.9);
  }
  glEnd();
  // Fill in the cylinder
  glBegin(GL_QUAD_STRIP);
  for (th = 0; th <= 360; th += 5)
  {
    glColor3d(0.7, 0.7, 0.7);
    glNormal3d(Cos(th), Sin(th), 0);
    glVertex3d(0.5 * Cos(th), 0.5 * Sin(th), 0.99);
    glVertex3d(0.1 * Cos(th), 0.1 * Sin(th), 0.999);
  }
  glEnd();
  /* FRONT WINDOWS END */

  /* LASERS FRONT */
  glPushMatrix();
  glTranslated(0.3, -0.6, 0);

  glBindTexture(GL_TEXTURE_2D,texture[5]);
  glBegin(GL_QUAD_STRIP);
  for (th = 0; th <= 360; th += inc)
  {
    glColor3d(0.6, 0.6, 0.6);
    glNormal3d(Cos(th), Sin(th), 0);
    glTexCoord2f(0,th*0.0123+.05);glVertex3d(0.05 * Cos(th), 0.05 * Sin(th), 0.95);
    glTexCoord2f(1,th*0.0123+.05);glVertex3d(0.05 * Cos(th), 0.05 * Sin(th), 0.6);
  }
  glEnd();


  for (ph=-90;ph<90;ph+=inc)
  {
    glColor3d(1, 0, 0);
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=inc)
    {
      glNormal3d(0.08*Sin(th)*Cos(ph), 0.08*Cos(th)*Cos(ph), 1 + 0.08*Sin(ph));
      glVertex3d(0.08*Sin(th)*Cos(ph), 0.08*Cos(th)*Cos(ph), 1 + 0.08*Sin(ph));

      glNormal3d(0.08*Sin(th)*Cos(ph+inc), 0.08*Cos(th)*Cos(ph+inc), 1 + 0.08*Sin(ph+inc));
      glVertex3d(0.08*Sin(th)*Cos(ph+inc), 0.08*Cos(th)*Cos(ph+inc), 1 + 0.08*Sin(ph+inc));
       
    }
    glEnd();
  }

  glPopMatrix();

  glPushMatrix();
  glTranslated(-0.3, -0.6, 0);

  glBindTexture(GL_TEXTURE_2D,texture[5]);
  glBegin(GL_QUAD_STRIP);
  for (th = 0; th <= 360; th += inc)
  {
    glColor3d(0.6, 0.6, 0.6);
    glNormal3d(Cos(th), Sin(th), 0);
    glTexCoord2f(0,th*0.0123+.05);glVertex3d(0.05 * Cos(th), 0.05 * Sin(th), 0.95);
    glTexCoord2f(1,th*0.0123+.05);glVertex3d(0.05 * Cos(th), 0.05 * Sin(th), 0.6);
  }
  glEnd();

  for (ph=-90;ph<90;ph+=inc)
  {
    glColor3d(1, 0, 0);
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=inc)
    {
      glNormal3d(0.08*Sin(th)*Cos(ph), 0.08*Cos(th)*Cos(ph), 1 + 0.08*Sin(ph));
      glVertex3d(0.08*Sin(th)*Cos(ph), 0.08*Cos(th)*Cos(ph), 1 + 0.08*Sin(ph));

      glNormal3d(0.08*Sin(th)*Cos(ph+inc), 0.08*Cos(th)*Cos(ph+inc), 1 + 0.08*Sin(ph+inc));
      glVertex3d(0.08*Sin(th)*Cos(ph+inc), 0.08*Cos(th)*Cos(ph+inc), 1 + 0.08*Sin(ph+inc));
       
    }
    glEnd();
  }
  glPopMatrix();
  /* LASERS FRONT END */

  /* BACK WINDOW */
  glPushMatrix();

  glBegin(GL_QUAD_STRIP);
  for (th = 0; th <= 360; th += inc)
  {
    glColor3d(0.75, 0.75, 0.75);
    glNormal3d(Cos(th), Sin(th), 0);
    glVertex3d(0.55 * Cos(th), 0.55 * Sin(th), -0.8);
    glVertex3d(0.4 * Cos(th), 0.4 * Sin(th), -1.05);
  }
  glEnd();

  glBegin(GL_QUAD_STRIP);
  for (th = 0; th <= 360; th += inc)
  {
    glColor3d(0.6, 0.6, 0.6);
    glNormal3d(Cos(th), Sin(th), 0);
    glVertex3d(0.4 * Cos(th), 0.4 * Sin(th), -1.05);
    glVertex3d(0.2 * Cos(th), 0.2 * Sin(th), -0.8);
  }
  glEnd();
  glBegin(GL_POLYGON);
  glColor3d(0, 0, 0);
  glNormal3d(0,0,-1);
  glVertex3d(0, 0.18 , -1);
  glVertex3d(0.21, 0.1 , -1);
  glVertex3d(0.21, -0.1 , -1);
  glVertex3d(0, -0.18 , -1);
  glVertex3d(-0.21, -0.1 , -1);
  glVertex3d(-0.21, 0.1 , -1);
  glEnd();

  glPopMatrix();
  /* BACK WINDOW & LIGHTS END */


  /* BACK LIGHTS */
  glPushMatrix();
  glTranslated(0.6, 0, -0.75);

  // Boxes for lights
  glBegin(GL_QUADS);
  //  Front
  glColor3f(0.4,0.4,0.4);
  glNormal3f(0,0,+1);
  glVertex3f(-0.1,-0.1, 0.1);
  glVertex3f(+0.1,-0.1, 0.1);
  glVertex3f(+0.1,+0.1, 0.1);
  glVertex3f(-0.1,+0.1, 0.1);
  //  Back
  glColor3f(0.4,0.4,0.4);
  glNormal3f(0,0,-1);
  glVertex3f(+0.1,-0.1,-0.1);
  glVertex3f(-0.1,-0.1,-0.1);
  glVertex3f(-0.1,+0.1,-0.1);
  glVertex3f(+0.1,+0.1,-0.1);
  //  Right
  glNormal3f(0,+1,0);
  glColor3f(0.5,0.5,0.5);
  glVertex3f(+0.1,-0.1,+0.1);
  glVertex3f(+0.1,-0.1,-0.1);
  glVertex3f(+0.1,+0.1,-0.1);
  glVertex3f(+0.1,+0.1,+0.1);
  //  Left
  glColor3f(0.5,0.5,0.5);
  glNormal3f(0,-1,0);
  glVertex3f(-0.1,-0.1,-0.1);
  glVertex3f(-0.1,-0.1,+0.1);
  glVertex3f(-0.1,+0.1,+0.1);
  glVertex3f(-0.1,+0.1,-0.1);
  //  Top
  glColor3f(0,0,0);
  glNormal3f(-1,0,0);
  glVertex3f(-0.1,+0.1,+0.1);
  glVertex3f(+0.1,+0.1,+0.1);
  glVertex3f(+0.1,+0.1,-0.1);
  glVertex3f(-0.1,+0.1,-0.1);
  //  Bottom
  glColor3f(0,0,0);
  glNormal3f(+1,0,0);
  glVertex3f(-0.1,-0.1,-0.1);
  glVertex3f(+0.1,-0.1,-0.1);
  glVertex3f(+0.1,-0.1,+0.1);
  glVertex3f(-0.1,-0.1,+0.1);
  glEnd();

  

  for (ph=-90;ph<90;ph+=inc)
  {
    glColor3d(1, 0.1, 0);
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=inc)
    {
      glNormal3d(0.09*Sin(th)*Cos(ph), 0.09*Cos(th)*Cos(ph), -0.1 + 0.09*Sin(ph));
      glVertex3d(0.09*Sin(th)*Cos(ph), 0.09*Cos(th)*Cos(ph), -0.1 + 0.09*Sin(ph));

      glNormal3d(0.09*Sin(th)*Cos(ph+inc), 0.09*Cos(th)*Cos(ph+inc), -0.1 + 0.09*Sin(ph+inc));
      glVertex3d(0.09*Sin(th)*Cos(ph+inc), 0.09*Cos(th)*Cos(ph+inc), -0.1 + 0.09*Sin(ph+inc));
       
    }
    glEnd();
  } 

  glPopMatrix();


  // Other light
  glPushMatrix();
  glTranslated(-0.6, 0, -0.75);

  // Boxes for lights
  glBegin(GL_QUADS);
  //  Front
  glColor3f(0.4,0.4,0.4);
  glNormal3f(0,0,+1);
  glVertex3f(-0.1,-0.1, 0.1);
  glVertex3f(+0.1,-0.1, 0.1);
  glVertex3f(+0.1,+0.1, 0.1);
  glVertex3f(-0.1,+0.1, 0.1);
  //  Back
  glColor3f(0.4,0.4,0.4);
  glNormal3f(0,0,-1);
  glVertex3f(+0.1,-0.1,-0.1);
  glVertex3f(-0.1,-0.1,-0.1);
  glVertex3f(-0.1,+0.1,-0.1);
  glVertex3f(+0.1,+0.1,-0.1);
  //  Right
  glNormal3f(0,+1,0);
  glColor3f(0.5,0.5,0.5);
  glVertex3f(+0.1,-0.1,+0.1);
  glVertex3f(+0.1,-0.1,-0.1);
  glVertex3f(+0.1,+0.1,-0.1);
  glVertex3f(+0.1,+0.1,+0.1);
  //  Left
  glColor3f(0.5,0.5,0.5);
  glNormal3f(0,-1,0);
  glVertex3f(-0.1,-0.1,-0.1);
  glVertex3f(-0.1,-0.1,+0.1);
  glVertex3f(-0.1,+0.1,+0.1);
  glVertex3f(-0.1,+0.1,-0.1);
  //  Top
  glColor3f(0,0,0);
  glNormal3f(-1,0,0);
  glVertex3f(-0.1,+0.1,+0.1);
  glVertex3f(+0.1,+0.1,+0.1);
  glVertex3f(+0.1,+0.1,-0.1);
  glVertex3f(-0.1,+0.1,-0.1);
  //  Bottom
  glColor3f(0,0,0);
  glNormal3f(+1,0,0);
  glVertex3f(-0.1,-0.1,-0.1);
  glVertex3f(+0.1,-0.1,-0.1);
  glVertex3f(+0.1,-0.1,+0.1);
  glVertex3f(-0.1,-0.1,+0.1);
  glEnd();

  for (ph=-90;ph<90;ph+=inc)
  {
    glColor3d(1, 0.1, 0);
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=inc)
    {
      glNormal3d(0.09*Sin(th)*Cos(ph), 0.09*Cos(th)*Cos(ph), -0.1 + 0.09*Sin(ph));
      glVertex3d(0.09*Sin(th)*Cos(ph), 0.09*Cos(th)*Cos(ph), -0.1 + 0.09*Sin(ph));

      glNormal3d(0.09*Sin(th)*Cos(ph+inc), 0.09*Cos(th)*Cos(ph+inc), -0.1 + 0.09*Sin(ph+inc));
      glVertex3d(0.09*Sin(th)*Cos(ph+inc), 0.09*Cos(th)*Cos(ph+inc), -0.1 + 0.09*Sin(ph+inc));
       
    }
    glEnd();
  } 
  

  glPopMatrix();

  /* BACK LIGHTS END */



  TieFighterWing(2,0,0, 0,0,0, 0);
  TieFighterWing(-2,0,0, 0,1,0, 180);


  //  Undo transofrmations
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
   
}














static void rock(double x,double y,double z,double s, 
                double rx, double ry, double rz, double angle)
{
  int th,ph;
  //  Save transformation
  glPushMatrix();
  //  Offset, scale and rotate
  glTranslated(x,y,z);
  glRotated(angle, rx, ry, rz);
  glScaled(s,s,s);
  glColor3f(1,1,1)
  ;
  for (ph=-90;ph<90;ph+=big_inc)
  {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=big_inc)
    {
      glNormal3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), Sin(ph));
      glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), Sin(ph));

      glNormal3d(Sin(th)*Cos(ph+big_inc), Cos(th)*Cos(ph+big_inc), Sin(ph+big_inc));
      glVertex3d(Sin(th)*Cos(ph+big_inc), Cos(th)*Cos(ph+big_inc), Sin(ph+big_inc));
       
    }
    glEnd();
  }
  //  Undo transofrmations
  glPopMatrix();
}

/*
*   lots of rocks flying around
*/
static void space_env() {

  glEnable(GL_TEXTURE_2D);

  // Save matrix and adjust position
  glPushMatrix();



  rock(200,-300,8000-live_env , 50, 0,0,0, 0);
  rock(200,300,6000-live_env , 100, 0,0,0, 0);
  rock(-200,-300,7000-live_env , 100, 0,0,0, 0);
  rock(-200,-300,16000-live_env , 100, 0,0,0, 0);
  rock(-500,-600,12000-live_env , 50, 0,0,0, 0);
  rock(700,-600,11000-live_env , 200, 0,0,0, 0);
  rock(0700,800,18000-live_env , 200, 0,0,0, 0);
  


  // int i;

  // rock(200,300,env_offset , 50);
  // rock(200,300,3000+env_offset , 50);

  // Builds space_env in panels for animation
  // for (i = 10000; i >= -10000; i-=1000) {
    // printf("%d\n", i);
  // for (i = 100; i >= -100; i-=10) {

    // rock(200,300,i , 50);
    // rock(-250,-360,10+i+live_env , 10);

    // rock(-50,50,900+i+live_env , 10);
    // rock(-50,-50,10+i+live_env , 10);

    // rock(-10,-50+i+live_env,10 , 10);

    // TieFighter(10,50,10+i+live_env,3, 0,0,0, 0);
    // TieFighter(10,50,1000+i+live_env,3, 0,0,0, 0);
  // }


   glPopMatrix();

   glDisable(GL_TEXTURE_2D);

}



















/*
 *  Draw out the scene of the neighboorhood (houses, roads, ground)
 */
static void drawScene(){
  // 
  // Lighting on the lights
  // 
  // float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
  // float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
  // float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
  // // float Emission[]  = {0.3, 0.2, 0.2, 0.0};
  // //  Light position
  // // float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
  // glColor3f(1,1,1);
  // // float PositionLight1[]  = {-2.5, 0 ,-2, 1.0};
  // // ball(PositionLight1[0],PositionLight1[1],PositionLight1[2] , 0.1);
  // // float PositionLight2[]  = {2.5, 0 ,-2, 1.0};
  // // ball(PositionLight2[0],PositionLight2[1],PositionLight2[2] , 0.1);
  // float PositionLight1[]  = {5, 0 ,10, 5.0};
  // ball(PositionLight1[0],PositionLight1[1],PositionLight1[2] , 0.1);

  // //  OpenGL should normalize normal vectors
  // glEnable(GL_NORMALIZE);
  // //  Enable lighting
  // glEnable(GL_LIGHTING);
  // //  Location of viewer for specular calculations
  // glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
  // //  glColor sets ambient and diffuse color materials
  // glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
  // glEnable(GL_COLOR_MATERIAL);
  // //  Enable light 0
  // glEnable(GL_LIGHT0);
  // // glEnable(GL_LIGHT1);
  // //  Set ambient, diffuse, specular components and position of light 0
  // glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
  // glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
  // glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);

  // // glLightfv(GL_LIGHT1,GL_AMBIENT ,Ambient);
  // // glLightfv(GL_LIGHT1,GL_DIFFUSE ,Diffuse);
  // // glLightfv(GL_LIGHT1,GL_SPECULAR,Specular);

  // glLightfv(GL_LIGHT0,GL_POSITION,PositionLight1);
  // // // glLightfv(GL_LIGHT1,GL_POSITION,PositionLight2);

  // 
  // END Lighting on the lights
  // 

  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);

  space_env();

  // Used same concept similar to the circling ball of light in example from class
  // TieFighter(0,0,0,3, 0,0,0, 0);
  TieFighter(0 + 50*Cos(zh), 0 + 50*Sin(zh) * Cos(zh),0,50,   0,0,1,  5*Sin(zh));

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

   // Space(3.5*dim);

   //  Perspective - set eye position
   if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);


   //  Draw scene
   drawScene();

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
     th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");

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

   live_env = fmod(2000*t, 20000);

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
  //  Toggle lighting
  else if (ch == 'l' || ch == 'L')
    light = 1-light;
  // //  Switch projection mode
  // else if (ch == 'p' || ch == 'P')
  //   mode = 1-mode;
  //  Toggle  movement
  else if (ch == 'm' || ch == 'M')
    move = 1-move;
  //  Move TIE Fighter
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
    dim -= 10;
  else if (ch=='D' && dim<2000)
    dim += 10;
  else if (ch==' ')
    dim -= 10;
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
   //   // Smooth color model
   // else if (ch == '1')
   //    smooth = 1-smooth;
   // //  Local Viewer
   // else if (ch == '2')
   //    local = 1-local;
   // else if (ch == '3')
   //    distance = (distance==2) ? 10 : 2;
   // //  Toggle ball increment
   // else if (ch == '4')
   //    inc = (inc==10)?3:10;
   // //  Flip sign
   // else if (ch == '5')
   //    one = -one;

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
   glutInitWindowSize(700,700);
   glutCreateWindow("Final - Tie Fighter - Edward Zhu");
   //  Set callbacks
   glutDisplayFunc(display);
   // glutTimerFunc(100, timer, 0);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);


   // Load all bmps
   // space = LoadTexBMP("textures/space.bmp");
   texture[1] = LoadTexBMP("textures/body_metal.bmp");
   texture[5] = LoadTexBMP("textures/cast_iron.bmp");
   texture[6] = LoadTexBMP("textures/metal.bmp");
   texture[9] = LoadTexBMP("textures/wing.bmp");

   



   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
