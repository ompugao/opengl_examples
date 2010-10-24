#include <GL/glut.h>
#include "initialize.h"
#include "key.h"
#include "object.h"
#include "array.h"
#include "detail_arrays.h"
#include <iostream>

using namespace std;

static void object_init();

void initialize(void){
  //ディスプレイの設定
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  /* GLUT_DEPTHはデプスバッファ処理のため.
   * glEnable(GL_DEPTH_TEST)をする必要があるが、ここでではなくglutDisplayFuncで
   * 呼ばれる関数(今の場合AllDisplay())に書かないとうまく動かなかった
   */
  glutInitWindowSize(800, 800);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("== Phony Star Fox ==");

  //ライトニング
  //GLfloat light_position[] ={0.0,0.0,1000.0};
  //glLightfv(GL_LIGHT0,GL_POSITION,light_position);
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
  detail_init();
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

void object_init() // ToDo:登録するオブジェクト数が少ないと当たり判定後のEXPLOSIONをaddするところでSEGVになる // 理由がわからん
{
    // 地面 // 必ず最初に一度だけaddすること
    add_object(STILL, GROUND, 100, 0xc0c0c0, 0, 0, NULL, make_array(5000,0,0));
    // 自機 // 必ず2番目に一度だけaddすること
    add_object(SELF, AWING, 10000, 0xff0000, 150, 15, NULL);
    // カーソル // 必ず3番目に一度だけaddすること
    add_object(FRONT, CURSOR, 10000, 0xff0000, 20, 15, NULL);
    // 以下オブジェクトを追加していく
    
    add_object(LISS, CUBE, 500, 0xffff00, 500, 0.01, NULL, make_array(-100,-100,0), make_array(1,2,0), make_array(1000,1000,0));
    add_object(CIRCLE, CUBE, 100, 0x00f3ee, 500, -5, NULL, make_array(100,100,1000), make_array(1,1,1), make_array(1000,0,0));
//    add_object(CHASE, AWING, 100, 0x2203ee, 100, 15, NULL);
    add_object(CHASE, AWING, 100, 0x220000, 100, 10, NULL);
    
}

//display lists
int listAwing;
int listBulletA;

void detail_init(){
  //Awing

  listAwing=glGenLists(1);
  glNewList(listAwing,GL_COMPILE);

  GLfloat Awing_ambient[]={0.0,1.0,0.0,1.0};
  GLfloat Awing_specular[]={0.0,0.5,1.0,1.0};
  GLfloat Awing_shininess[]={50.0};
  glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,Awing_ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,Awing_specular);
  glMaterialfv(GL_FRONT,GL_SHININESS,Awing_shininess);

  //glEnableClientState(GL_NORMAL_ARRAY);
  //glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3,GL_DOUBLE,0,airframe_vertex);

  glColor3f(0.5,0.6,0.8);
  glDrawElements(GL_TRIANGLE_STRIP,14,GL_UNSIGNED_BYTE,airframe_vertexindices1);//側壁
  glDrawElements(GL_POLYGON,5,GL_UNSIGNED_BYTE,airframe_vertexindices2);//底面 
  glColor3f(0.0,0.0,0.0);
  glDrawElements(GL_TRIANGLE_STRIP,6,GL_UNSIGNED_BYTE,airframe_vertexindices3);//窓
  glColor3f(0.7,0.9,0.8);
  glDrawElements(GL_TRIANGLES,12,GL_UNSIGNED_BYTE,airframe_vertexindices4);//左翼
  glDrawElements(GL_TRIANGLES,12,GL_UNSIGNED_BYTE,airframe_vertexindices5);//右翼  

  glVertexPointer(3,GL_DOUBLE,0,wing_vertexdata);  
  glColor3f(0.0,0.0,1.0);
  glPushMatrix();
  {
    glTranslated(0.0,12.0,0.0);
    glDrawElements(GL_TRIANGLES,18,GL_UNSIGNED_BYTE,wing_vertexindices);
  }//左wing
  glPopMatrix();
  glPushMatrix();
  {
    glTranslated(0.0,-12.0,0.0);
    glDrawElements(GL_TRIANGLES,18,GL_UNSIGNED_BYTE,wing_vertexindices);
  }//右wing
  glPopMatrix();
  //glDisableClientState(GL_NORMAL_ARRAY);
  //glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  glEndList();



  //弾A
  listBulletA=glGenLists(1);
  glNewList(listBulletA,GL_COMPILE);
  GLfloat ambient[]={1.0,0.0,0.0,1.0};
  GLfloat specular[]={1.0,0.5,0.0,1.0};
  GLfloat shininess[]={128.0};
  glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialfv(GL_FRONT,GL_SHININESS,shininess);
  
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3,GL_DOUBLE,0,bulletA_vertexdata);
  glColorPointer(3,GL_DOUBLE,0,bulletA_colordata);
  glNormalPointer(GL_DOUBLE,0,bulletA_normdata);
  glDrawElements(GL_TRIANGLES,8,GL_UNSIGNED_BYTE,bulletA_vertexindices);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  glEndList();
}
