#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <GL/glut.h>
#include <queue>

extern GLdouble camera_matrix[16];

class Camera{
 private:
  GLdouble a1,a2,a3,b1,b2,b3,c1,c2,c3;
  std::queue<GLdouble*> self_coords;
  void copy_init_coord(GLdouble*);
  void copy_self_coord(void);
  //行列保存
  void move_camera();
  public:
  Camera(GLdouble a=0,GLdouble b=0,GLdouble c=0);
  //カメラの動きを記入
  void set_camera();
};

#endif //__CAMERA_H__

