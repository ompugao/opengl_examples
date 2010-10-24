#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <algorithm>
#include "key.h"

extern class Me

const double Ogrespeed = 2.0;

class Ogre{
public:
  void Draw();         //Ogre's shape
  void Follow(Me);       //Meを追いかける計算
  void Collide(Me);
  double x, y, z;          //Ogre's position
  double vx, vy, vz;       //Ogre's velocity
  double v;      //Ogre's movement speed
  Ogre(){
    x = y = z =10;
    vx = vy = vz =0;
    v = Ogrespeed;
  }
  virtual ~Ogre(){}
  void Init();
};

/*
 * Ogreの関数実装
 */

void Ogre::Draw(void){
  glPushMatrix();
  Follow(me);
  glColor3d(1.0, 0.0, 0.0);
  glutSolidCube(3.0);
  glPopMatrix();
  Collide(me);
}

void Ogre::Follow(Me me){
  double tmpv;
  tmpv = sqrt(pow(me.x-this->x,2)+pow(me.y-this->y,2)+pow(me.z-this->z,2)); //thisはいらない
  this->vx = (me.x-this->x)/tmpv * this->v;
  this->vy = (me.y-this->y)/tmpv * this->v;
  this->vz = (me.z-this->z)/tmpv * this->v;

  this->x += this->vx;
  this->y += this->vy;
  this->z += this->vz;
  glTranslated(this->x, this->y, this->z);
}

void Ogre::Collide(Me me){
  if(pow(this->x-me.x,2)+pow(this->y-me.y,2)+pow(this->z-me.z,2)<1){
    exit(0);
  }
}
