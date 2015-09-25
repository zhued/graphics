 /*
 * Lorenz Attractor
 * Edward Zhu
 * 
 *
 * lorenz calculation based on lorenz.c
 * other functions modified 
 *
 *  Time it took to complete: 7 hours
 *  
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


//  Globals
int th=0;       // view angle
int ph=0;       // view angle
double z=0;     // z variable
double w=1;     // w variable
double dim=2;   // Dimension of orthogonal box
int num_steps = 100000;

float s = 10;
float b = 2.666666;
float r = 28;

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
 *  Display the scene with the Lorenz Attractor
 */
void display()
{
    //  Clear and reset
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
    // Set project and rotation andles
    glOrtho(-1, 1, -1, 1, -10, 10);
    glRotated(ph,1,0,0);
    glRotated(th,0,1,0);
    
    // Draw line string accoring to Lorenz
    glBegin(GL_LINE_STRIP);
    
    float x = 1, y = 1, z = 1;
    int i;    
    float dt = .001;

    // Basic function of the lorenz attractor converted to work with
    // OpenGL functionalities
    for (i = 0; i < num_steps; i++) {    
        
        float dx = s*(y-x);
        float dy = x*(r-z) - y;
        float dz = x*y - b*z;

        x += dt*dx;
        y += dt*dy;
        z += dt*dz;        
        // Make attractor fit in default view
        glVertex4f(x*.05, y*.05, z*.05, w); 
    }

    glEnd();

    // Instructions 
    output(25, "Use ARROW KEYS to move around");
    output(5, "S,B,R: Incease lorenz params        s,b,r: Decrease lorenz params ");
    
    glFlush();
    glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Arrow Keys for changing angles
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   else if (key == GLUT_KEY_UP)
      ph += 5;
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;

   // Angles
   th %= 360;
   ph %= 360;
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
   else if (ch == '0'){
      th = ph = 0;
      s = 10;
      b = 2.666666;
      r = 28;
   }
   //  Decrease s by 1
   else if (ch == 's'){
      s -= 1;
   }
   //  Increase s by 1
   else if (ch == 'S'){
      s += 1;
   }
   //  Decrease b by .1
   else if (ch == 'b'){
      b -= .2;
   }
   //  Increase b by .1
   else if (ch == 'B'){
      b += .2;
   }
   //  Decrease r by 1
   else if (ch == 'r'){
      r -= 1;
   }
   //  Increase r by 1
   else if (ch == 'R'){
      r += 1;
   }
   glutPostRedisplay();
}



/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection box adjusted for the
   //  aspect ratio of the window
   glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
  //  Initialize GLUT and display mores
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

   //  Creates a window
   glutInitWindowSize(500,500);
   glutCreateWindow("Lorenz Attractor - Edward Zhu");
   
   // Displaying everything.
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutMainLoop();
   return 0;
}