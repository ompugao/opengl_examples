#include <GL/glut.h>
#include "enemy2.h"
#include "key.h"

static void set_position_in_field(VECTOR &_position){
	while(_position.x > 500) {
		_position.x -= 1000;
	}
	while(_position.x < -500) {
		_position.x += 1000;
	}
	while(_position.y > 500) {
		_position.y -= 1000;
	}
	while(_position.y < -500) {
		_position.y += 1000;
	}
	while(_position.z > 500) {
		_position.z -= 500;
	}
	while(_position.z < 0) {
		_position.z += 500;
	}
};

void Enemy2::move() 
{
	static VECTOR old_rand_v;
	VECTOR rand_v;

	rand_v.x = rand() % 11 -5;
	rand_v.y = rand() % 11 -5;
	rand_v.z = rand() % 11 -5;
	
	v = old_rand_v + old_rand_v + rand_v;
	old_rand_v = rand_v;
	h_omega = 1000 * RADIAN;
	v_omega = 1000 * RADIAN;

	position = position + v;
	set_position_in_field(position);
	h_angle += h_omega;
	v_angle += v_omega;
	set_direction(h_angle, v_angle);

	return;
}

void Enemy2::draw()
{
	move();

	glPushMatrix();

	glTranslatef(position.x, position.y , position.z);
	glRotated(h_angle, 0, 0, 1);
	glRotated(v_angle, 1, 0, 0);
	glScaled(30 * r, 30 * r, 30 * r);

	glColor3f(1.0, 0.0, 0.0);
	glutSolidDodecahedron();

	glPopMatrix();

	return;
}

Enemy2 enemy2(-200, 200, 100);

