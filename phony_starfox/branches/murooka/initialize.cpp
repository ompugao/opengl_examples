#include <GL/glut.h>
#include "initialize.h"
#include "key.h"
#include "camera.h"
#include <cmath>
#include <ctime>

void initialize(void){
  //ディスプレイの設定
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  /* GLUT_DEPTHはデプスバッファ処理のため.
   * glEnable(GL_DEPTH_TEST)をする必要があるが、ここでではなくglutDisplayFuncで
   * 呼ばれる関数(今の場合AllDisplay())に書かないとうまく動かなかった
   */
  glutInitWindowSize(700, 700);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("== Phony Star Fox (only enemy) ==");

  //ライトニング
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);

  //背景色
  glClearColor(0.7, 0.9, 0.8, 1.0);

  glShadeModel(GL_SMOOTH);

  glLoadIdentity();

  key_init();

	// 乱数の初期値のセット
	srand((unsigned) time(NULL));
}

void Reshape(int w, int h){
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(75.0, (GLfloat) w/ (GLfloat) h, 100.0, 5000.0);
	camera.set();
}
