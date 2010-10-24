#include "object.h"
#include <GL/glut.h>
#include "key.h"
#include "initialize.h"
#include "time_counter.h"
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void Object::detail_sphere()
{
    glColor4dv(color);
    glutSolidSphere(R, 50, 50);
    //    showHP();
}

void Object::detail_cube()
{
    glColor4dv(color);
    glutSolidCube(R);
    //showHP();
}

// 地面
// detail_array[0][0]が一辺の長さを表す
// v,Rは無効
void Object::detail_ground()
{
  FieldSize = detail_array[0][0];
  glColor4dv(color);
  glLoadMatrixd(camera_matrix);
  glNormal3f(0.0,0.0,1.0);
  glBegin(GL_QUADS);
   glVertex3d(FieldSize/2,-FieldSize/2,0);
   glVertex3d(FieldSize/2,FieldSize/2,0);
   glVertex3d(-FieldSize/2,FieldSize/2,0);
   glVertex3d(-FieldSize/2,-FieldSize/2,0);
  glEnd();
}

void Object::detail_Awing(){
  if(key_right)glRotated(15.0, 1.0,0.0,0.0);
  if(key_left) glRotated(-15.0, 1.0,0.0,0.0);

  glCallList(listAwing);
  glPushMatrix();
  {
    glTranslated(-15.0,0.0,0.0);
    glScaled(5.0,10.0,4.0);
    glColor3f(1.0,0.0,0.0);
    glutSolidSphere(0.8+0.2*cos(time_count),10,10);
  }
  glPopMatrix();
  showHP(0,785);

}

void Object::detail_bulletA(){
  glCallList(listBulletA);
}

// 螺旋 // これ単独では螺旋にはならない // レーザーで使われる予定
void Object::detail_spiral()
{
    glColor4dv(color);
    glutSolidCube(R);
    glPushMatrix();
    {
      glTranslated(0,0.0,20.0);
      glColor3f(1.0,0.0,1.0);
      glutSolidCube(5);
    }
    glPopMatrix();
}

// カーソル
void Object::detail_cursor()
{
  glBegin(GL_LINE_LOOP);
   glVertex3d(0,-R/2,-R/2);
   glVertex3d(0,R/2,-R/2);
   glVertex3d(0,R/2,R/2);
   glVertex3d(0,-R/2,R/2);
  glEnd();
}

void Object::showHP(int x,int y){
  //HP表示
  int windowheight = 800,windowwidth=800;//windowのsizeを指定すること
  if (y > windowheight) y = windowheight;
  if (y < 0) y=0;
  if (x > windowwidth) x = windowwidth;
  if (x < 0) x=0;


  //ここからフォント描画処理
  glPushMatrix(); //各種行列を退避
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION); //2Dの並行投影を設定
  glPushMatrix();

  glLoadIdentity();

  gluOrtho2D(0, windowheight, 0, windowwidth);
  //ここから文字描画

  glPushAttrib(GL_CURRENT_BIT|GL_DEPTH_BUFFER_BIT);  //色、Zバッファの退避
  glDisable(GL_DEPTH_TEST);
  glColor3d(1,0,1);

  char HPbuffer[10];
  sprintf(HPbuffer,"%d",hp);

  char str[] = "HP = ";

  glColor3f(1.0, 1.0, 1.0);
  glRasterPos2i(x , y);
  for (int i = 0; i < strlen(str); i++){
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
  }
  for (int i = 0; i < strlen(HPbuffer); i++){
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, HPbuffer[i]);
  }  
  glPopAttrib();
    
  glPopMatrix(); 
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();


  //これで勘弁して...モウゼンゼンワカラナイ
  /*  char str[] = "HP = ";
  glColor3f(0.0, 1.0, 0.0);
  glRasterPos3i(0, 0, R/2);
  for (int i = 0; i < strlen(str); i++) {
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
  }
  char buffer[10];
  sprintf(buffer,"%d",hp);
  for (int i = 0; i < strlen(buffer); i++) {
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, buffer[i]);
  }
  */

}  
