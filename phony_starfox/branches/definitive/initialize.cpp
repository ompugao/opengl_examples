#include <GL/glut.h>
#include "initialize.h"
#include "key.h"
#include "enemy_round.h"

static void object_init();

void initialize(void){
  //ディスプレイの設定
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  /* GLUT_DEPTHはデプスバッファ処理のため.
   * glEnable(GL_DEPTH_TEST)をする必要があるが、ここでではなくglutDisplayFuncで
   * 呼ばれる関数(今の場合AllDisplay())に書かないとうまく動かなかった
   */
  glutInitWindowSize(1000, 1000);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("== Phony Star Fox ==");

  //ライトニング
  GLfloat light_position[] ={0.0,0.0,1000.0};
  glLightfv(GL_LIGHT0,GL_POSITION,light_position);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  /*
  glEnable(GL_FOG);
  {
    GLfloat fogColor[4] = {0.1,0.1,0.1,1.0};
    glFogi(GL_FOG_MODE,GL_EXP);
    glFogfv(GL_FOG_COLOR,fogColor);
    glFogf(GL_FOG_DENSITY,0.35);
    glHint(GL_FOG_HINT,GL_DONT_CARE);
    glFogf(GL_FOG_START,1.0);
    glFogf(GL_FOG_END,5.0);
  }
  //fogの色
  glClearColor(0.0,0.5,0.2,1.0);
  */

  //背景色
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_NORMALIZE);
  glLoadIdentity();

  key_init();
  
  object_init();
}

void Reshape(int w, int h){
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(75.0, (GLfloat) w/ (GLfloat)h, 1.0, 30000.0);
  //glOrtho(-5000.0,5000.0,-5000.0,5000.0,-2500.0,10000.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(500,500,250, 0,0,0, 0,0,1);

}

static void object_init()
{
  enemy_rounds.push_back(make_enemy_round(100, 100, 500, 300));
  enemy_rounds.push_back(make_enemy_round(500, 500, 600, 500));
}
