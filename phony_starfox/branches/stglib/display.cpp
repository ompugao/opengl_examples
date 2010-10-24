#include <GL/glut.h>
#include "display.h"
#include "object.h"
#include "camera.h"
#include "time_counter.h"
#include <iostream>
using namespace std;

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Camera camera;

//すべてのオブジェクトを描画
void AllDisplay()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  glLoadIdentity();
  camera.set_camera();
  //gluLookAt(1000,1000,5000,0,0,0,0,0,1);
  //glGetDoublev(GL_MODELVIEW_MATRIX,camera_matrix);
  glLoadIdentity();

  //ここに描画するオブジェクトを登録していく
  draw_object();


  //ここからフォント描画処理
  glPushMatrix(); //各種行列を退避
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION); //2Dの並行投影を設定
  glPushMatrix();

  glLoadIdentity();
  gluOrtho2D(0, 400, 0, 400);
  //ここから文字描画

  glPushAttrib(GL_CURRENT_BIT|GL_DEPTH_BUFFER_BIT);  //色、Zバッファの退避
  glDisable(GL_DEPTH_TEST);
  glColor3d(1,0,1);
   
  char str[] = "new strings show command! in display.cpp";

  glColor3f(1.0, 1.0, 1.0);
  glRasterPos2i(0, 0);
  for (int i = 0; i < strlen(str); i++) 
    {
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
    }
    
  glPopAttrib();
    
  glPopMatrix(); 
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  //ここまで登録箇所
  glutSwapBuffers();
}

