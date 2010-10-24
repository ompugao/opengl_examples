/*
 *地面のクラス実装
 */

#include <GL/glut.h>
#include "ground.h"
#include "camera.h"

// フィールドの縦と横の長さ、高さはこの半分
const GLdouble FieldSize = 5000;

void Ground::draw(){
  glLoadIdentity();
  glColor4d(0.5,0.5,0.5,1.0);
  glLoadMatrixd(camera_matrix);
  glNormal3f(0.0,0.0,1.0);
  glBegin(GL_QUADS);
   glVertex3d(FieldSize/2,-FieldSize/2,0);
   glVertex3d(FieldSize/2,FieldSize/2,0);
   glVertex3d(-FieldSize/2,FieldSize/2,0);
   glVertex3d(-FieldSize/2,-FieldSize/2,0);
  glEnd();

	glColor4d(0.5, 0.5, 0.5, 1.0);
	for(int i = 0; i < FieldSize/2; i += 50) {
	    glBegin(GL_LINE);
			glVertex3f(i, -FieldSize/2, 1);
			glVertex3f(i, FieldSize/2, 1);
	    glEnd();
	}
	for(int i = 0; i > -FieldSize/2; i -= 50) {
	    glBegin(GL_LINE);
			glVertex3f(i, -FieldSize/2, 1);
			glVertex3f(i, FieldSize/2, 1);
	    glEnd();
	}
	for(int i = 0; i < FieldSize/2; i += 50) {
	    glBegin(GL_LINE);
			glVertex3f(-FieldSize/2, i, 1);
			glVertex3f(FieldSize/2, i, 1);
	    glEnd();
	}
	for(int i = 0; i > -FieldSize/2; i -= 50) {
	    glBegin(GL_LINE);
			glVertex3f(-FieldSize/2, i, 1);
			glVertex3f(FieldSize/2, i, 1);
	    glEnd();
	}

  return ;
}
