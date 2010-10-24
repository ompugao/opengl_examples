#include <GL/glut.h>
#include "initialize.h"
#include "key.h"

void initialize(void){
  //ディスプレイの設定
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  /* GLUT_DEPTHはデプスバッファ処理のため.
   * glEnable(GL_DEPTH_TEST)をする必要があるが、ここでではなくglutDisplayFuncで
   * 呼ばれる関数(今の場合AllDisplay())に書かないとうまく動かなかった
   */
  glutInitWindowSize(700, 700);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("== Phony Star Fox ==");

  //ライトニング
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);

  //背景色
  glClearColor(1.0, 1.0, 1.0, 0.0);

  glShadeModel(GL_SMOOTH);
  glLoadIdentity();

  key_init();
}

void Reshape(int w, int h){
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(75.0, (GLfloat) w/ (GLfloat)h, 1.0, 3000.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(500,500,250, 0,0,0, 0,0,1);

}
