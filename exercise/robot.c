//robot arm
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

static int shoulder = 0, elbow = 0, camera_x = 0, camera_y = 0, camera_z = 0;
void init()
{
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glShadeModel(GL_FLAT);
}
/*
void idle(){
  glutPostRedisplay();
}
*/

void timer()
{
  glutPostRedisplay();
  glutTimerFunc(50,timer,0);
}
  
void arm_display()
{
  shoulder++;
  elbow++;
  if (shoulder>360) shoulder=0;
  if (elbow>360) elbow=0;
  glPushMatrix();
  glTranslatef(-1.0, 0.0, 0.0);
  glRotatef((GLfloat) shoulder, 0.0, 0.0, 0.0);
  glTranslatef(1.0, 0.0, 0.0);

  glPushMatrix();
  glScalef(2.0, 0.4, 1.0);
  glColor3f(1.0, 0.0, 0.0);
  glutSolidCube(1.0);
  glPopMatrix(); //draw upper arm

  glTranslatef(1.0, -0.2, 0.0);
  glRotatef((GLfloat) elbow, 0.0, 0.0, 1.0);
  glTranslatef(1.0, 0.2, 0.0);

  glPushMatrix();
  glScalef(2.0, 0.4, 1.0);
  glColor3f(0.0, 0.0, 1.0);
  glutSolidCube(1.0);
  glPopMatrix(); //draw lower arm

  glPopMatrix();
  glutSwapBuffers();

}

void display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();
  gluLookAt( camera_x, 4.0, camera_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  arm_display();

}

void reshape(int w, int h){
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65.0, (GLfloat) w/ (GLfloat) h, 1.0, 120.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //glTranslatef(0.0, 0.0, -5.0);
  gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y){
  switch(key){
  default:
    break;
  }
}

void special_key(int key,int x, int y)
{
  switch(key){
  case GLUT_KEY_UP:
    camera_x++;
    break;
  case GLUT_KEY_DOWN:
    camera_x--;
    break;
  case GLUT_KEY_LEFT:
    camera_z++;
    break;
  case GLUT_KEY_RIGHT:
    camera_z--;
    break;
  default:
    break;
  }
}

void mouse(int button, int state, int x, int y)
{
  switch (button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
      /* アニメーション開始*/
    }
    else {
      /* アニメーション停止 */
      exit(0);
    }
    break;
  default:
    break;
  }
}

int main(int argc, char** argv){
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500,500);
  glutInitWindowPosition(100,100);
  glutCreateWindow(argv[0]);
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  //glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutSpecialFunc(special_key);
  glutTimerFunc(50,timer,0);
  glutMainLoop();
  return 0;
}
