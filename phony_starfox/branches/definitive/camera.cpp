#include "camera.h"
#include "matrix.h"
#include <GL/glut.h>
#include <queue>
#include "self.h"

GLdouble camera_matrix[16];

static const int CYCLES = 30;//何周期前のselfの位置にカメラを置くか

Camera::Camera(GLdouble x,GLdouble y,GLdouble z){
  GLdouble tmp2[16];
  for(int i=0;i<=15;i++){
    if(i==12)      tmp2[i]=x;
    else if(i==13) tmp2[i]=y;
    else if(i==14) tmp2[i]=z;
    else tmp2[i] = 0;
  }
  for(int i=0;i < CYCLES ;i++){
    copy_init_coord(tmp2);
  }
}

void Camera::copy_init_coord(GLdouble *tmp2){
  GLdouble *tmp = new GLdouble [16];
  copyMatrix(tmp2,tmp);
  self_coords.push(tmp);
}

void Camera::copy_self_coord(void){
  GLdouble *tmp = new GLdouble [16];
  copyMatrix(self.get_coord(),tmp);
  self_coords.push(tmp);
}

void Camera::set_camera(){
  /*
  static bool initialize_flag = false;
  if(!initialize_flag){
    for(int i=0;i < CYCLES ;i++){
      copy_self_coord();
    }
    initialize_flag = true;
  }
  */
  GLdouble *tmp;
  tmp = self_coords.front();

  a1 = tmp[12];
  a2 = tmp[13];
  a3 = tmp[14]+60;
  b1 = self.get_coord()[12];
  b2 = self.get_coord()[13];
  b3 = self.get_coord()[14];
  //c1 = self.get_coord()[8];
  //c2 = self.get_coord()[9];
  //c3 = self.get_coord()[10];
  c1=0;
  c2=0;
  c3=1;
  //c1=tmp[8];
  //c2=tmp[9];
  //c3=tmp[10];
  delete [] self_coords.front();
  self_coords.pop();
  
  copy_self_coord();
  move_camera();
}

void Camera::move_camera(){
  glLoadIdentity();
  gluLookAt(a1,a2,a3,b1,b2,b3,c1,c2,c3);
  glGetDoublev(GL_MODELVIEW_MATRIX,camera_matrix);
  glLoadIdentity();
}
