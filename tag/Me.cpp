#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <algorithm>
#include "key.h"

extern class Ogre

const double XMAX=30.0;
const double YMAX=30.0;
const double ZMAX=30.0;

class Me{
public:
  void Draw();
  void Move();        //keyによって移動
  double x, y, z;
  double speed;
  Me(){
    x = y  = 10;
    z = 0;
    speed = 3; 
  }
  virtual ~Me(){}
  void Init();
};

/*
 * Meの関数実装
 */

//Me の描画
void Me::Draw(void){
  glPushMatrix();
  Move();
  glColor3d(0.0, 1.0, 0.0);
  glutSolidSphere(1.0,8,8);
  glPopMatrix();
}

//Me の動き
void Me::Move(void){
  if(key_up == true){
    this->y = std::min(this->y + this->speed , YMAX);
  }
  if(key_down == true){
    this->y = std::max(this->y - this->speed , -YMAX);
  }
  if(key_left == true){
    this->x = std::max(this->x - this->speed , -XMAX);
  }
  if(key_right == true){
    this->x = std::min(this->x + this->speed , XMAX);
  }
  if(key_k == true){
    this->z = std::min(this->z + this->speed , ZMAX);
  }
  if(key_m == true){
    this->z = std::max(this->z - this->speed , -ZMAX);
  }

  glTranslated(this->x, this->y, this->z);
}
