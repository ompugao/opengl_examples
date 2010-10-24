#include <GL/glut.h>
#include "key.h" 
#include "initialize.h"
#include "display.h"
#include "time_counter.h"

int time_count = 0;

void timer(int){
  glutPostRedisplay();
  time_count++;
  glutTimerFunc(40,timer,0);
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
  glutTimerFunc(40,timer,0);        //50ms以下で１周期
  glutMainLoop();
  return 0;
}
