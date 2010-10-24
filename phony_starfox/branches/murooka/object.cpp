#include <GL/glut.h>
#include "object.h"
#include "key.h"

const double RADIAN = 3.1415 / 180;

void Object::set_position(double _x, double _y, double _z)
{
	position.x = _x;
	position.y = _y;
	position.z = _z;

	return;
}

void Object::set_direction(double _h_angle, double _v_angle)
{
	h_angle = _h_angle;
	v_angle = _v_angle;
	VECTOR init_direction(cos(h_angle), sin(h_angle), sin(v_angle));
	direction = init_direction;

	return;
}

void Object::set_r(double _r)
{
	r = _r;
	
	return;
}

VECTOR Object::get_pos()
{
	return position;
}

double Object::get_r()
{
	return r;
}

