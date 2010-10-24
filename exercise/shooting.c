#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


static double camera_x = 0, camera_y = 0;// camera_z = 0;


//ターゲット描画

void target_display()
{
  double target_x;
  static int t;
  t++;
  target_x = 5 * sin(0.05*t);
  glPushMatrix();
  glTranslatef(target_x, 0.0, -10.0);
  glScalef(0.3, 2.0, 0.3);
  glColor3f(1.0,0.0,0.0);
  glutSolidCube(1.0);
  glPopMatrix();
}

//すべて描画

void display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glLoadIdentity();
  gluLookAt(camera_x, camera_y, 5.0, camera_x, camera_y+4.0 , -10.0, 0.0, 1.0, 0.0);
  target_display();
  glutSwapBuffers();
}

void reshape(int w, int h){
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65.0, (GLfloat) w/ (GLfloat) h, 1.0, 120.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y){
  switch(key){
  case 27:
    exit(0);
    break;
  default:
    break;
  }
}

//特殊キーを扱う

void special_key(int key,int x, int y)
{
  switch(key){
  case GLUT_KEY_UP:
    camera_y = camera_y + 0.1;
    break;
  case GLUT_KEY_DOWN:
    camera_y = camera_y - 0.1;
    break;
  case GLUT_KEY_LEFT:
    camera_x = camera_x - 0.1;
    break;
  case GLUT_KEY_RIGHT:
    camera_x = camera_x + 0.1;
    break;
  default:
    break;
  }
}

//マウスを扱う

void mouse(int button, int state, int x, int y)
{
  switch (button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
      /* アニメーション停止 */
      exit(0);
    }
    break;
  default:
    break;
  }
}

void timer()
{
  glutPostRedisplay();
  glutTimerFunc(50,timer,0);
}

void init()
{
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glShadeModel(GL_FLAT);
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
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutSpecialFunc(special_key);
  glutTimerFunc(50,timer,0);
  glutMainLoop();
  return 0;
}
