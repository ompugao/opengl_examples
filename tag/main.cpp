#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <algorithm>
#include "key.h"

static Ogre ogre;
static Me   me;

//すべてのオブジェクトを描画
void AllDisplay(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  ogre.Draw();
  me.Draw();
  glutSwapBuffers();
}


//諸々の初期化
void game_init(void){
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(700, 700);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("onigokko");
  glViewport(0, 0, 300, 300);

  //ライトニング
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);

  //背景色
  glClearColor(0.0, 0.0, 0.0, 0.0);

  glShadeModel(GL_SMOOTH);
  glLoadIdentity();

  key_init();
}

void Reshape(int w, int h){
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(75.0, (GLfloat) w/ (GLfloat) h, 1.0, 120.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(-40,-40,40,  0,0,0,  0,0,1); //カメラの位置etc.
}

void timer(int){
  glutPostRedisplay();
  glutTimerFunc(50,timer,0);
}

int main(int argc, char** argv){
  glutInit(&argc,argv);
  game_init();
  glutDisplayFunc(AllDisplay);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Keyboard_Down);	// キーボード押す
  glutKeyboardUpFunc(Keyboard_Up);	// キーボード離す
  glutSpecialFunc(Specialkey_Down); // 特殊キー　押す
  glutSpecialUpFunc(Specialkey_Up); // 特殊キー　離す
  glutTimerFunc(50,timer,0);
  glutMainLoop();
  return 0;
}
