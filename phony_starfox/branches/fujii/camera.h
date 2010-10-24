#include <GL/glut.h>

#ifndef __CAMERA_H__
#define __CAMERA_H__

extern double camera_matrix[16];

class Camera{
 private:
  GLdouble a1,a2,a3,b1,b2,b3,c1,c2,c3;
 public:
  //カメラの動きを記入
  void set_camera();
  //行列保存
  void move_camera();
};

#endif //__CAMERA_H__

