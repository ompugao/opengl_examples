#include <GL/glut.h>
#include "enemy1.h"
#include "key.h"

void Enemy1::move() 
{
	v = ZERO_VECTOR;
	h_omega = 100 * RADIAN;
	v_omega = 0;
	
	position = position + v;
	h_angle += h_omega;
	v_angle += v_omega;
	set_direction(h_angle, v_angle);

	return;
}

void Enemy1::draw()
{
	move();

	glPushMatrix();

	glRotated(h_angle, 0, 0, 1);
	glScaled(1, 1, 0.5);
	glTranslatef(position.x, position.y , position.z);
	glRotated(45, 1, 0, 0);
	glRotated(90, 0, 1, 0);

	glColor3f(0.2, 0.8, 0.3);
	glutSolidCone(50 * r, 100 * r, 4, 4);

	glPopMatrix();

	return;
}

Enemy1 enemy1(0, -200, 200);

