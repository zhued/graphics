/*
 * Lorenz Attractor
 * Edward Zhu
 * 
 *
 *  Many basic functions used throughout transfered 
 *  from gears assignment.
 *
 *  Time it took to complete: 7 hours
 * 
 */
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/glew.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// increase num_steps to draw more of the lorenz attractor
int num_steps = 100000;
float pts[100000][3];

/*  Lorenz Parameters from moodle lorenz.c */
double s  = 10;
double b  = 2.6666;
double r  = 28;

int iter = 0;
int iter_increment = 10;

// Basic function of the lorenz attractor converted to work with
// OpenGL functionalities
static void basic_lorenz(void) {
  int i;
  /*  Coordinates  */
  float x = pts[0][0] = 1;
  float y = pts[0][1] = 1;
  float z = pts[0][2] = 1;
  /*  Time step  */
  float dt = 0.001;

  for (i=0;i<num_steps-1;i++)
  { 
    float dx = s*(y-x);
    float dy = x*(r-z)-y;
    float dz = x*y - b*z;
    x += dt*dx;
    y += dt*dy;
    z += dt*dz;
    
    pts[i+1][0] = x;
    pts[i+1][1] = y;
    pts[i+1][2] = z;
    
  }
}

// Output information onto the screen for the user to interact
// with the animation
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

// base variables of view positions and rotation
static GLfloat view_rotx = 20.0, view_roty = 50.0, view_rotz = 0.0;
static GLfloat view_posz = 50.0;

// draw out the lorenz attractor
void draw() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glPushMatrix();  
    glRotatef(view_rotx, 1.0, 0.0, 0.0);
    glRotatef(view_roty, 0.0, 1.0, 0.0);
    glRotatef(view_rotz, 0.0, 0.0, 1.0);

  glBegin(GL_LINE_STRIP);
  int i = 0;
  
  while( i < iter && i < num_steps ) {
    // creates the next vertex
    glVertex3fv(pts[i++]);
  }
  glEnd();
  
  // Keep adding the interation increment until you reach the 
  // total number of steps allowed, at which, it stops.
  if( iter < num_steps ) {
    if( iter + iter_increment > num_steps ) 
      iter = num_steps;
    else 
      iter+=iter_increment;
  }
  
  glFlush();

  // Instructions 
  output(65, "Use ARROW KEYS to move around");
  output(45, "R: Restart animation            F: Finish animation");
  output(25, "T: Animation speed +         G: Animation speed -");
  output(5, "E: Zoom Z axis +                D: Zoom Z axis -");

  glutSwapBuffers();
  glPopMatrix();
}


// new window size or exposure
// transfered over gears assignment
static void reshape(int width, int height) {
  GLfloat h = (GLfloat) height / (GLfloat) width;

  glViewport(0, 0, (GLint) width, (GLint) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-5.0, 5.0, -h*2, h*2, 1.0, 300.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -60.0);
}

// if nothing is happening, then constantly increment so the line keeps
// drawing
static void idle(void) {
  iter+=iter_increment;
  glutPostRedisplay();
}

// Key strokes that add user interation such as zoom, animation speed
// and restarting or finish the animation
// transfered over gears assignment
// 
// reset - make the iteration count back to zero
// finish - make iter the max number of steps
// speed up and slow down - just change the iter_increment value
// zoom in and out - increase/decrease the z position value
static void key(unsigned char k, int x, int y) {
  switch (k) {
    case 'r':
      iter = 0;
      break;
    case 'f':
      iter = num_steps;
      break;
    case 't':
      iter_increment += 5;
      break;
    case 'g':
      if( iter_increment - 5 >- 0 ) iter_increment -= 5;
      break;
    case 'e':
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      view_posz -= 1;
      gluLookAt(0,0,view_posz,0.0,0.0,0.0,0.0,1.0,0.0);
      break;
    case 'd':
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      view_posz += 1;
      gluLookAt(0,0,view_posz,0.0,0.0,0.0,0.0,1.0,0.0);
      break;
    default:
      return;
  }
  glutPostRedisplay();
}

// change view angle - key presses rotate by 5
// transfered over gears assignment
static void special(int k, int x, int y) {
  switch (k) {
    case GLUT_KEY_UP:
      view_rotx += 5.0;
      break;
    case GLUT_KEY_DOWN:
      view_rotx -= 5.0;
      break;
    case GLUT_KEY_LEFT:
      view_roty += 5.0;
      break;
    case GLUT_KEY_RIGHT:
      view_roty -= 5.0;
      break;
    default:
      return;
  }
  glutPostRedisplay();
}

// Mostly transfered over from gears assignment.
int main(int argc,char* argv[]) {
  basic_lorenz();
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(600, 600);
  glutCreateWindow("Lorenz Attractor By Edward Zhu");

  glutDisplayFunc(draw);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key);
  glutSpecialFunc(special);
  glutIdleFunc(idle);
  
  glutMainLoop();
  return 0;              /* ANSI C requires main to return int. */
}
