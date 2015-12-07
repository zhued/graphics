/*
 *  Tie Fighter - FINAL PROJECT - Edward Zhu
 *
 *  
 *  Go to the README to learn more about this project!
 * 
 * 
 */
#include "CSCIx229.h"
#include <math.h>

int axes=1;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int th=165;         //  Azimuth of view angle
int ph=25;         //  Elevation of view angle
int fov=60;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=700.0;   //  Size of world


float sco=180;    //  Spot cutoff angle
float Exp=0;      //  Spot exponent
unsigned int texture[18];
unsigned int space[2];
int live_env = 0;
int laser_animation = 20;
int rock_separate = 0;
int trigger_laser = 0;
int collision = 0;

// static double env_offset=8000;

// Light values
int one       =   1;  // Unit value
int distance  =   10;  // Light distance
int inc       =  10;  // Ball increment
int big_inc   =  40;
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  40;  // Ambient intensity (%)
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









// From class examples -  Sky
static void Space(double D)
{
  glPushMatrix();
  glRotated(90, 0, 1, 0);
  glColor3f(1,1,1);
  glEnable(GL_TEXTURE_2D);

  //  Sides
  glBindTexture(GL_TEXTURE_2D,space[1]);
  glBegin(GL_QUADS);
  glTexCoord2f(0,0); glVertex3f(-D,-D,-D);
  glTexCoord2f(1,0); glVertex3f(+D,-D,-D);
  glTexCoord2f(1,1); glVertex3f(+D,+D,-D);
  glTexCoord2f(0,1); glVertex3f(-D,+D,-D);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f(0,0); glVertex3f(+D,-D,-D);
  glTexCoord2f(1,0); glVertex3f(+D,-D,+D);
  glTexCoord2f(1,1); glVertex3f(+D,+D,+D);
  glTexCoord2f(0,1); glVertex3f(+D,+D,-D);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f(0,0); glVertex3f(+D,-D,+D);
  glTexCoord2f(1,0); glVertex3f(-D,-D,+D);
  glTexCoord2f(1,1); glVertex3f(-D,+D,+D);
  glTexCoord2f(0,1); glVertex3f(+D,+D,+D);
  glEnd();

  glBegin(GL_QUADS);
  glTexCoord2f(0,0); glVertex3f(-D,-D,+D);
  glTexCoord2f(1,0); glVertex3f(-D,-D,-D);
  glTexCoord2f(1,1); glVertex3f(-D,+D,-D);
  glTexCoord2f(0,1); glVertex3f(-D,+D,+D);
  glEnd();

     // Top and bottom
  glBindTexture(GL_TEXTURE_2D,space[1]);
  glBegin(GL_QUADS);
  glTexCoord2f(0,0); glVertex3f(+D,+D,-D);
  glTexCoord2f(1,0); glVertex3f(+D,+D,+D);
  glTexCoord2f(1,1); glVertex3f(-D,+D,+D);
  glTexCoord2f(0,1); glVertex3f(-D,+D,-D);

  glTexCoord2f(0,0); glVertex3f(-D,-D,+D);
  glTexCoord2f(1,0); glVertex3f(+D,-D,+D);
  glTexCoord2f(1,1); glVertex3f(+D,-D,-D);
  glTexCoord2f(0,1); glVertex3f(-D,-D,-D);
  glEnd();



  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
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

















static void laserbeam(double x,double y,double z,double s, 
                double rx, double ry, double rz, double angle)
{
  int th;
  //  Save transformation
  glPushMatrix();
  //  Offset, scale and rotate
  glTranslated(x,y,z);
  glRotated(angle, rx, ry, rz);
  glScaled(s,s,s);
  glColor3f(0,0,1);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[10]);
  glBegin(GL_QUAD_STRIP);
  for (th = 0; th <= 360; th += 5)
  {
    glColor3d(0.5, 0.5, 0.5);
    glNormal3d(Cos(th), Sin(th), 0);
    glTexCoord2f(0,th*0.0123);glVertex3d(0.5 * Cos(th), 0.5 * Sin(th), 0);
    glTexCoord2f(1,th*0.0123);glVertex3d(0.5 * Cos(th), 0.5 * Sin(th), 50);
  }
  glEnd();
  glDisable(GL_TEXTURE_2D);
  
  //  Undo transofrmations
  glPopMatrix();
}


/*
*   FIRE ZEE LAZARS
*     THIS TOOK WAY TOO LONG TO FIGURE OUT HOW TO IMPLEMENT
*/
static void fire_laser() {
 

  glColor3f(1,1,1);
    // Save matrix and adjust position
  glPushMatrix();

  if (trigger_laser == 1)
  {
    if(laser_animation < 3000){
      laser_animation += 100;
      laserbeam(-15 + 50*Cos(zh), -25 + 50*Sin(zh) * Cos(zh),laser_animation , 10, 0,0,0, 0); 
      laserbeam(15 + 50*Cos(zh), -25 + 50*Sin(zh) * Cos(zh),laser_animation , 10, 0,0,0, 0); 
      if (laser_animation == 1520 && collision == 0)
      {
        collision = 1;
        laser_animation = 20;
        trigger_laser = 0;
      }
    } else {
      laser_animation = 20;
      trigger_laser = 0;
    }
  }
  glPopMatrix();



}

static void explosion(double x,double y,double z,double r)
{
   int th,ph;
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=big_inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*big_inc)
      {
        glTexCoord2d(th/180.0,ph/90.0+0.5); 
        Vertex(th,ph);

        glTexCoord2d(th/180.0,(ph+big_inc)/90.0+0.5);
        Vertex(th,ph+big_inc);
      }
      glEnd();
   }
   //  Undo transofrmations
   glPopMatrix();
}

static void rock_texture_c(double x,double y,double z,double s, 
                double rx, double ry, double rz, double angle){
  int th,ph;
  glPushMatrix();
  //  Offset, scale and rotate
  glTranslated(x,y,z);
  glRotated(angle, rx, ry, rz);
  glScaled(s,s,s);
  glColor3f(1,1,1);

  // glColor3f(1,0,0);
  for (ph=-90;ph<90;ph+=big_inc)
  {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=big_inc)
    {
      glTexCoord2d(th/360.0,ph/180.0+0.5);
      glNormal3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), Sin(ph));
      glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), Sin(ph));

      glTexCoord2d(th/360.0,(ph+big_inc)/180.0+0.5);
      glNormal3d(Sin(th)*Cos(ph+big_inc), Cos(th)*Cos(ph+big_inc), Sin(ph+big_inc));
      glVertex3d(Sin(th)*Cos(ph+big_inc), Cos(th)*Cos(ph+big_inc), Sin(ph+big_inc));
       
    }
    glEnd();
  }
  glPopMatrix();
}

static void rock_texture_s(double x,double y,double z,double s, 
                double rx, double ry, double rz, double angle){
  glPushMatrix();
  //  Offset, scale and rotate
  glTranslated(x,y,z);
  glRotated(angle, rx, ry, rz);
  glScaled(s,s,s);

  glColor3f(1,1,1);
  glBegin(GL_QUADS);
  glNormal3f( 0, 0, 1);
  glTexCoord2f(0.0,0.0); glVertex3f(-0.1,-0.1, 0.1);
  glTexCoord2f(0.1,0.0); glVertex3f(+0.1,-0.1, 0.1);
  glTexCoord2f(0.1,0.1); glVertex3f(+0.1,+0.1, 0.1);
  glTexCoord2f(0.0,0.1); glVertex3f(-0.1,+0.1, 0.1);
  glEnd();
  //  Back
  // glColor3f(0,0,1);
  glBegin(GL_QUADS);
  glNormal3f( 0, 0,-1);
  glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
  glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
  glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
  glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
  glEnd();
  //  Right
  // glColor3f(1,1,0);
  glBegin(GL_QUADS);
  glNormal3f(+1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
  glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
  glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
  glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
  glEnd();
  //  Left
  // glColor3f(0,1,0);
  glBegin(GL_QUADS);
  glNormal3f(-1, 0, 0);
  glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
  glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
  glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
  glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
  glEnd();
  //  Top
  // glColor3f(0,1,1);
  glBegin(GL_QUADS);
  glNormal3f( 0,+1, 0);
  glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
  glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
  glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
  glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
  glEnd();
  //  Bottom
  // glColor3f(1,0,1);
  glBegin(GL_QUADS);
  glNormal3f( 0,-1, 0);
  glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
  glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
  glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
  glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
  glEnd();
  glPopMatrix();
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
  // glColor3f(1,1,1);
  glBindTexture(GL_TEXTURE_2D,texture[5]);

  // add texture to circle
  rock_texture_s(0.5,0.5,0, 0.25, 0,0,0, 0);
  rock_texture_s(-0.5,0.5,0, 0.25, 0,0,0, 0);
  rock_texture_s(0.5,-0.5,0, 0.25, 0,0,0, 0);
  rock_texture_s(-0.5,-0.5,0.4, 0.25, 0,0,0, 0);

  rock_texture_c(0.5,0.4,0, 0.5, 0,0,0, 0);
  rock_texture_c(0.7,0.1,0, 0.5, 0,0,0, 0);
  rock_texture_c(-0.4,-0.4,-0.5, 0.5, 0,0,0, 0);
  rock_texture_c(0.4,-0.4,-0.3, 0.5, 0,0,0, 0);
  rock_texture_c(0.0,0.5,-0.6, 0.5, 0,0,0, 0);
  rock_texture_c(0.0,0.5,0.6, 0.5, 0,0,0, 0);
  rock_texture_c(-0.7,-0.1,0.2, 0.4, 0,0,0, 0);

  for (ph=-90;ph<90;ph+=big_inc)
  {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=big_inc)
    {
      glTexCoord2d(th/360.0,ph/180.0+0.5);
      glNormal3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), Sin(ph));
      glVertex3d(Sin(th)*Cos(ph), Cos(th)*Cos(ph), Sin(ph));

      glTexCoord2d(th/360.0,(ph+big_inc)/180.0+0.5);
      glNormal3d(Sin(th)*Cos(ph+big_inc), Cos(th)*Cos(ph+big_inc), Sin(ph+big_inc));
      glVertex3d(Sin(th)*Cos(ph+big_inc), Cos(th)*Cos(ph+big_inc), Sin(ph+big_inc));
       
    }
    glEnd();
  }

  //  Undo transofrmations
  glPopMatrix();
}

/*
*   EXPLODE ZEE ROCKKKKKK
*/
static void rock_explode() {
  glEnable(GL_TEXTURE_2D);
  // Save matrix and adjust position
  glPushMatrix();

  // rock that will explode
  if (collision == 1)
  {
    if (rock_separate < 3000)
    {
      rock_separate += 20;
      if (rock_separate < 1200)
      {
        explosion(0,0,1400-rock_separate/2,                   0+rock_separate/10);
        explosion(-rock_separate/5,0,1500-rock_separate/2,                   0+rock_separate/5);
        explosion(rock_separate/5,rock_separate/5,1600-rock_separate/2,      0+rock_separate/5);
        explosion(0,rock_separate/5,1500-rock_separate/2,                    0+rock_separate/5);
        explosion(0,-rock_separate/5,1500-rock_separate/2 ,                   0+rock_separate/5);
        explosion(-rock_separate/5,rock_separate/5,1700-rock_separate/2 ,    0+rock_separate/5);
        explosion(rock_separate/5,-rock_separate/10,1400-rock_separate/2 ,    0+rock_separate/5);
      
        // 
        // Lighting on the lights
        // 
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};

        float PositionLight1[] = {0,0,1500, 1.0}; 
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
        // glEnable(GL_LIGHT1);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);

        glLightfv(GL_LIGHT0,GL_POSITION,PositionLight1);
      } else {
        glDisable(GL_LIGHTING);
      }
      
      rock(rock_separate,       rock_separate,    -2*rock_separate+1500,  100, 0,0,0, 0);
      rock(-0.50*rock_separate, 2*rock_separate,  1500,                   50, 0,0,0, 0);
      rock(rock_separate,       -rock_separate,   2*rock_separate+1500,   40, 0,0,0, 0);
      rock(-0.25*rock_separate, -rock_separate,   1500,                   80, 0,0,0, 0);
      rock(-1*rock_separate,    0.1*-rock_separate,1500,                  60, 0,0,0, 0);
      rock(-0.25*rock_separate, -rock_separate,   -3*rock_separate+1500,  20, 0,0,0, 0);
      rock(-0.25*rock_separate, 0.4*rock_separate,   rock_separate+1500,  20, 0,0,0, 0);
    } else {
      collision = 0;
      rock_separate = 0;
    }
  } else {
    
    rock(0 - 100*Sin(zh) * Cos(zh), 0 + 100*Cos(zh), 1500 ,150,   0,0,1,  5*Sin(zh));
  }
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
}


/*
*   lots of rocks flying around
*/
static void space_env() {

  glEnable(GL_TEXTURE_2D);

  // Save matrix and adjust position
  glPushMatrix();

  // close rocks
  rock(400,-300,8000-live_env , 50, 0,0,0, 90);
  rock(300,300,6000-live_env , 100, 0,0,0, 90);
  rock(-300,-300,7000-live_env , 100, 0,0,0, 0);
  rock(-300,-300,11000-live_env , 100, 0,0,0, 0);
  rock(-500,-600,12000-live_env , 50, 0,0,0, 0);
  rock(700,-600,11000-live_env , 200, 0,0,0, 90);
  rock(700,800,15000-live_env , 200, 0,0,0, 90);

  // faraway rocks
  rock(-1000,-6000,12000-live_env , 50, 0,0,0, 0);
  rock(-1800,-600,1000-live_env , 200, 0,0,0, 90);
  rock(2000,800,6000-live_env , 200, 0,0,0, 0);
  rock(-2000,400,8000-live_env , 200, 0,0,0, 90);
  rock(2000,400,5000-live_env , 200, 0,0,0, 0);

  // inverse rocks
  rock(700,-300,-8000+live_env , 50, 0,0,0, 0);
  rock(-700,300,-4000+live_env , 200, 0,0,0, 180);
  rock(0,-800,-12000+live_env , 100, 0,0,0, 0);

  // farway inverse
  rock(800,300,-1000+live_env , 50, 0,0,0, 90);
  rock(-700,300,-4000+live_env , 200, 0,0,0, 0);
  rock(0,-800,-12000+live_env , 100, 0,0,0, 0);


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

  space_env();

  // Used same concept similar to the circling ball of light in example from class
  // TieFighter(0,0,0,3, 0,0,0, 0);
  TieFighter(0 + 50*Cos(zh), 0 + 50*Sin(zh) * Cos(zh),0,50,   0,0,1,  5*Sin(zh));

  rock_explode();
  fire_laser();
  

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

   Space(10*dim);

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
   Print("Angle=%d,%d  Dim=%.1f FOV=%d", th,ph,dim,fov);
   glWindowPos2i(5,25);
   Print("Press SPACEBAR to shoot lasers! ARROW KEYS to move around!");

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
  // laser_animation = fmod(500*t, 1000);
  // if(laser_animation > 1000){
  //   laser_animation = 0;
  // }
  // laser_animation += 1;

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
    trigger_laser = 1;

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
  glutInitWindowSize(800,700);
  glutCreateWindow("Final - Tie Fighter - Edward Zhu");
  //  Set callbacks
  glutDisplayFunc(display);
  // glutTimerFunc(100, timer, 0);
  glutReshapeFunc(reshape);
  glutSpecialFunc(special);
  glutKeyboardFunc(key);
  glutIdleFunc(idle);


  // Load all bmps
  space[0] = LoadTexBMP("textures/space_tb.bmp");
  space[1] = LoadTexBMP("textures/space_lr.bmp");

  texture[1] = LoadTexBMP("textures/body_metal.bmp");
  texture[2] = LoadTexBMP("textures/fire.bmp");
  texture[5] = LoadTexBMP("textures/cast_iron.bmp");
  texture[6] = LoadTexBMP("textures/metal.bmp");
  texture[9] = LoadTexBMP("textures/wing.bmp");
  texture[10]= LoadTexBMP("textures/laser.bmp");

   



  //  Pass control to GLUT so it can interact with the user
  ErrCheck("init");
  glutMainLoop();
  return 0;
}
