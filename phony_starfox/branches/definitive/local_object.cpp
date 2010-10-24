#include "local_object.h"

LocalObject::LocalObject(){
  coord = new GLdouble [16];
  loadIdentity(coord);
}
LocalObject::LocalObject(const LocalObject &_local_object){
  coord = new GLdouble [16];
  copyMatrix(_local_object.coord, coord);
  pos = _local_object.pos;
  R = _local_object.R;
}
LocalObject::~LocalObject(){
  //  delete [] coord;
}

void LocalObject::draw(){
  glLoadIdentity();
  pre_move();
  move();
  post_move();
  detail(); 
}
void LocalObject::pre_move(){
  //初期位置まで動かす
  static bool initialize_flag = false;
  if(!initialize_flag){
    glTranslated(pos.x,pos.y,pos.z);
    glRotated(pos.theta1,0,0,1);
    glRotated(pos.theta2,0,1,0);
    glRotated(pos.theta3,1,0,0);
    //最初のローカル座標系を保存
    glGetDoublev(GL_MODELVIEW_MATRIX,coord);
    initialize_flag = true;
    //cout << "matrix initialized!!" <<endl;
  }
  
  glLoadIdentity();
  glMultMatrixd(coord);//前周期のローカル座標系に移動
}
void LocalObject::post_move(){
  //現在のローカル座標系を保存
  glGetDoublev(GL_MODELVIEW_MATRIX,coord);

  //posの更新
  renew_pos();

  //カメラの行列をかけ合わせ,設定
  multiply_camera_matrix(); 
}

void LocalObject::renew_pos(){
  pos.x = coord[12];
  pos.y = coord[13];
  pos.z = coord[14];
}

void LocalObject::multiply_camera_matrix(){
  GLdouble tmp[16];
  glLoadIdentity();
  multiply(coord,camera_matrix,tmp);
  glLoadMatrixd(tmp);
}

VECTOR LocalObject::get_pos(){
  return pos;
}
double LocalObject::get_R(){
  return R;
}
GLdouble * LocalObject::get_coord(){
  return coord;
}

/* 
 * move()の例
 *
void LocalObject::move(){

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

  glTranslated(speed,0,0);

  if(key_z){
    glRotated(15, 1,0,0);
  }
}
*/
/*
 * detail()の例
 *
void LocalObject::detail(){
  glColor3f(0.0,1.0,0.0);
  glPushMatrix();
  glRotated(90, 1,0,0);
  glutSolidTeapot(60.0);
  glPopMatrix();
}
*/
