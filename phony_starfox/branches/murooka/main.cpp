#include <GL/glut.h>
#include "key.h" 
#include "initialize.h"
#include "display.h"

static void timer(int){
  glutPostRedisplay();
  glutTimerFunc(50,timer,0);
}

int main(int argc, char** argv){
  glutInit(&argc,argv);
  initialize();                     //諸々の初期化
  glutDisplayFunc(AllDisplay);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Keyboard_Down);	// キーボード押す
  glutKeyboardUpFunc(Keyboard_Up);	// キーボード離す
  glutSpecialFunc(Specialkey_Down); // 特殊キー　押す
  glutSpecialUpFunc(Specialkey_Up); // 特殊キー　離す
  glutTimerFunc(50,timer,0);        //50ms以下で１周期
  glutMainLoop();
  return 0;
}

