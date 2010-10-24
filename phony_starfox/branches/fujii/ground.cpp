/*
 *地面のクラス実装
 *行列計算はAwing内でglLookAt()をあとで呼び出すため今はNG
 */

#include <GL/glut.h>
#include "ground.h"

void Ground::draw(){
  glColor4d(0.0,0.0,0.0,1.0);
  glBegin(GL_QUADS);
  glVertex3d(0,1000,0);
   glVertex3d(1000,1000,0);
   glVertex3d(1000,0,0);
   glVertex3d(0,0,0);
  glEnd();
  return ;
}
