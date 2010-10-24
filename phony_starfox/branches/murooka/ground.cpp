#include <GL/glut.h>

#include "ground.h"

Ground ground;

void Ground::draw()
{
	move();
	
	// 地面
	glColor3f(0.2, 0.5, 0.2);
	glBegin(GL_POLYGON);
	for(int i = 0; i < 4; i++) {
		glVertex3f(point[i].x, point[i].y, point[i].z);
	}
	glEnd();

	// 座標軸
	glColor3f(0, 0, 0);
	for(int i = point[0].x; i < point[2].x; i += 50) {
	    glBegin(GL_LINE);
			glVertex3f(i, point[0].y, 1);
			glVertex3f(i, point[2].y, 1);
	    glEnd();
	}
	for(int i = point[0].y; i < point[2].y; i += 50) {
	    glBegin(GL_LINE);
			glVertex3f(point[0].x, i, 1);
			glVertex3f(point[2].x, i, 1);
	    glEnd();
	}
	
	// 山
	glColor3f(0.9, 0.5, 0.5);
	glPushMatrix();
	glTranslatef(200, 200 , 0);
	glutSolidCone(250, 50, 20, 10);
	glPopMatrix();
	
	return;
}
