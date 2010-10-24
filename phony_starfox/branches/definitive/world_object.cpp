#include "world_object.h"

WorldObject::WorldObject(){
  coord = new GLdouble [16];
  for(int i=0;i<=15;i++){
    if(i%5==0) coord[i]=1;
    else coord[i]=0;
  }
}
WorldObject::WorldObject(const WorldObject &_world_object){
  coord = new GLdouble [16];
  copyMatrix(_world_object.coord, coord);
  pos = _world_object.pos;
  R = _world_object.R;
}
WorldObject::~WorldObject(){
  delete [] coord;
}

void WorldObject::pre_move(){
  glLoadIdentity();
}
void WorldObject::post_move(){
  glLoadIdentity();
  glLoadMatrixd(camera_matrix);
  glTranslated(pos.x,pos.y,pos.z);
  glRotated(pos.theta1,0,0,1);
  glRotated(pos.theta2,0,1,0);
  glRotated(pos.theta3,1,0,0);
  glGetDoublev(GL_MODELVIEW_MATRIX,coord);
}
void WorldObject::draw(){
  pre_move();
  move();
  post_move();
  detail();
}

VECTOR WorldObject::get_pos(){
  return pos;
}
GLdouble WorldObject::get_R(){
  return R;
}
