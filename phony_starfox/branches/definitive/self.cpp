#include "self.h"
#include "key.h"
#include "enemy_round.h"
#include "vector.h"
#include "bullet.h"
#include "ground.h"
#include "time_counter.h"
#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

static bool last_key_v = false;
typedef enum{
  DEFAULT,
  U_TURN
} _MOVE_TYPE;

Self::Self(GLdouble a, GLdouble b, GLdouble c, GLdouble i, GLdouble j, GLdouble k){
  pos = make_vector(a, b, c, i, j, k);
  R = 60;
}

Self::~Self(){
}
void Self::move()
{
  static _MOVE_TYPE MOVE_TYPE;

  switch(MOVE_TYPE){
  case U_TURN:
    //uターン
    static int u_turn_phase=0;
    static int each_time= 20;

    switch(u_turn_phase){
    case 0:
      //少しまっすぐ進む
      glTranslated(15.0,0.0,0.0);
      if(each_time>0){each_time--;}
      else{each_time=35;u_turn_phase=1;}
      break;
    case 1:
      //ぐるり  
      glRotated(-5.0,0,1,0);
      glTranslated(10.0,0.0,0.0);
      if(each_time>0) each_time--;
      else { each_time=19; u_turn_phase=2;}
      break;
    case 2:
      //ねじりつつ進む
      glRotated(9, 1,0,0);
      glTranslated(20,0,0);
      if(each_time>0) each_time--;
      else { each_time=20; u_turn_phase=0;MOVE_TYPE=DEFAULT;}
      break;
    }
    break;
  case DEFAULT:
    if(key_left){
      glRotated(5.0, 0,0,1);
    }
    if(key_right){
      glRotated(-5.0, 0,0,1);
    }
    if(key_up){
      glRotated(-5.0, 0,1,0);
    }
    if(key_down){
      glRotated(5.0, 0,1,0);
    }
    if (key_c){
      glTranslated(10,0,0);
    }
    glTranslated(10,0,0);

    if(key_x){
      glRotated(10, 1,0,0);
    }
    if(key_z){
      glRotated(-10, 1,0,0);
    }
    if((coord[12] > FieldSize  / 2 ) || (coord[12] < -FieldSize / 2 ) || (coord[13] > FieldSize  / 2 ) ||  (coord[13] < -FieldSize / 2 ))  MOVE_TYPE=U_TURN;
    break;
  }
  //弾発射
  if(!last_key_v && key_v){
    if(bullets.size() < 10) {
      bullets.push_back(make_bullet(coord));
    }
  }
  last_key_v = key_v;
}

void Self::post_move(){
  //現在のローカル座標系を保存
  glGetDoublev(GL_MODELVIEW_MATRIX,coord);

  //フィールドの端へ行くと反対側へ行く。
  /*
  if(coord[12] > FieldSize  / 2 ) coord[12] -= FieldSize;
  if(coord[12] < -FieldSize / 2 ) coord[12] += FieldSize;
  if(coord[13] > FieldSize  / 2 ) coord[13] -= FieldSize;
  if(coord[13] < -FieldSize / 2 ) coord[13] += FieldSize;
  */
  if(coord[14] < 0              ) coord[14] =0;
  if(coord[14] > FieldSize  / 2 ) coord[14] = FieldSize/2;
  
  
  glLoadMatrixd(coord);

  //posの更新
  renew_pos();

  //カメラの行列をかけ合わせ,設定
  multiply_camera_matrix(); 
}
static GLdouble airframe_vertex[]={
   60.0,  0.0,  0.0,//   0
    5.0, -3.0,  6.0,//   1
    5.0,  3.0,  6.0,//   2
    0.0, -8.0,  0.0,//   3
    0.0,  8.0,  0.0,//   4
   -3.0, -2.0, 10.0,//   5
   -3.0,  2.0, 10.0,//   6
    0.0, -4.0,  4.0,//   7
    0.0,  4.0,  4.0,//   8
  -10.0, -6.0,  0.0,//   9
  -10.0,  6.0,  0.0,//  10
  -10.0, -5.0,  2.5,//  11
  -10.0,  5.0,  2.5,//  12
  -10.0, -5.0, -2.5,//  13
  -10.0,  5.0, -2.5,//  14
  -20.0,-50.0,-20.0,//  15
  -20.0, 50.0,-20.0 //  16
};

static GLubyte airframe_vertexindices1[]={
  //  2,1,3,5,9,11,7,10,6,10,8,4,2,1
  1,0,2,4,8,10,6,9,5,9,7,3,1,0
};//側壁
static GLubyte airframe_vertexindices2[]={
  //  1,4,10,11,5
  0,3,9,10,4
};//底面
static GLubyte airframe_vertexindices3[]={
  //  8,2,6,3,7,9
  7,1,5,2,6,8
};//窓
static GLubyte airframe_vertexindices4[]={
  4,16,12,12,16,14,14,16,4,12,14,4
};//左翼
static GLubyte airframe_vertexindices5[]={
  3,11,15,15,11,13,15,13,3,11,3,13
};//右翼


static GLdouble wing_vertexdata[]={
  50.0, 0.0, 0.0,//0
   0.0,-8.0, 0.0,//1
   0.0, 8.0, 0.0,//2
 -30.0, 0.0,15.0,//3
 -30.0, 0.0,-15.0//4
};//ウィング

static GLubyte wing_vertexindices[]={
  0,3,1, 0,2,3, 0,4,2, 0,1,4, 1,3,2, 1,2,4
};//ウィングindex

void Self::detail()
{
  if(key_right)glRotated(15.0, 1.0,0.0,0.0);
  if(key_left) glRotated(-15.0, 1.0,0.0,0.0);
  GLfloat ambient[]={0.0,1.0,0.0,1.0};
  GLfloat specular[]={0.0,0.5,1.0,1.0};
  GLfloat shininess[]={50.0};
  glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialfv(GL_FRONT,GL_SHININESS,shininess);

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
  glPushMatrix();
  {
    glTranslated(-15.0,0.0,0.0);
    glScaled(5.0,10.0,4.0);
    glColor3f(1.0,0.0,0.0);
    glutSolidSphere(0.8+0.2*cos(time_count),10,10);
  }
  glPopMatrix();
  //glDisableClientState(GL_NORMAL_ARRAY);
  //glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);

  collide();
}

int Self::collide()
{
  for(p_enemy_rounds = enemy_rounds.begin(); p_enemy_rounds != enemy_rounds.end(); p_enemy_rounds++) {
    if((pos - (*p_enemy_rounds).get_pos()).size() < ((*p_enemy_rounds).get_R() + R)) {
      cout << "You lost!" << endl;
      exit(0);
      return 1;
    }
  }
  return 0;
}
